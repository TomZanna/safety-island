#!/usr/bin/env python3
"""RTT dei core secondari: hub con UNA connessione J-Link, terminali per core via TCP locale.

Il control block RTT e' uno solo, condiviso da M33S, M7_0 e M7_1 (fw/common/rtt_shared.h) a 0x204D0000,
un canale per core (0 m33s, 1 m70, 2 m71). La sonda J-Link accetta al massimo 5 connessioni
contemporanee (provato): per lasciarne ai GDB server (debug multi-core) l'RTT usa una sola connessione,
l'hub, che serve ogni canale su una porta TCP locale. I terminali (make rtt-<app>, task VS Code) si
collegano all'hub, non alla sonda.

  rtt.py hub [--core M7_1]         J-Link + porte 127.0.0.1:19100 (m33s), 19101 (m70), 19102 (m71)
  rtt.py term <core>|all           terminale (input incluso; con "all" righe di tutti i core e input
                                   "<core>: testo"). Avvia l'hub se non c'e'.

Piu' terminali sullo stesso canale ricevono tutti l'output (l'hub lo distribuisce).
"""
import argparse
import fcntl
import os
import random
import select
import socket
import subprocess
import sys
import time

CHANNELS = {"m33s": 0, "m70": 1, "m71": 2}
BASE_PORT = int(os.environ.get("RTT_BASE_PORT", "19100"))
CTL_PORT = BASE_PORT + 99  # "hub vivo?"
ADDR = "0x204D0000"


def port_of(name):
    return BASE_PORT + CHANNELS[name]


def hub_alive():
    try:
        socket.create_connection(("127.0.0.1", CTL_PORT), timeout=0.3).close()
        return True
    except OSError:
        return False


def listen(port):
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(("127.0.0.1", port))
    s.listen()
    return s


# --------------------------------------------------------------------------------------------- hub
def run_hub(args):
    import pylink

    jl = pylink.JLink()
    # La DLL rifiuta aperture della sonda nello stesso istante di un altro programma: si riprova
    for attempt in range(20):
        try:
            jl.open()
            break
        except pylink.errors.JLinkException as e:
            if attempt == 19:
                sys.exit("J-Link non disponibile: %s (max 5 connessioni alla sonda)" % e)
            time.sleep(0.5 + random.random())
    jl.set_tif(pylink.enums.JLinkInterfaces.SWD)
    jl.set_speed(args.speed)
    jl.exec_command("SetRTTAddr %s" % ADDR)
    jl.connect("MIMX94398_%s" % args.core, speed=args.speed, verbose=False)  # attach: niente reset/halt
    # Porte subito (i terminali si collegano e risultano pronti); il control block lo crea il primo core
    # che arriva a board_init(): con WAIT_DEBUGGER i core aspettano il debugger, quindi si attende qui.
    listeners = {listen(port_of(n)): ch for n, ch in CHANNELS.items()}
    ctl = listen(CTL_PORT)
    clients = {}  # socket -> canale
    print("==> RTT hub via %s (control block %s), porte %d-%d" %
          (args.core, ADDR, BASE_PORT, BASE_PORT + len(CHANNELS) - 1), flush=True)
    rtt_ready = False
    next_try = 0.0

    def try_rtt():
        try:
            jl.rtt_start(int(ADDR, 0))
            for _ in range(10):
                try:
                    if jl.rtt_get_num_up_buffers() >= len(CHANNELS):
                        return True
                except pylink.errors.JLinkRTTException:
                    pass
                time.sleep(0.05)
            jl.rtt_stop()
        except pylink.errors.JLinkException:
            pass
        return False

    try:
        while True:
            r, _, _ = select.select(list(listeners) + [ctl] + list(clients), [], [], 0.01)
            for s in r:
                if s is ctl:
                    ctl.accept()[0].close()
                elif s in listeners:
                    c, _ = s.accept()
                    clients[c] = listeners[s]
                else:
                    data = s.recv(256)
                    if not data:
                        clients.pop(s)
                        s.close()
                    elif rtt_ready:
                        jl.rtt_write(clients[s], list(data))
            if not rtt_ready:
                if time.monotonic() >= next_try:
                    rtt_ready = try_rtt()
                    if rtt_ready or next_try == 0.0:
                        print("control block trovato" if rtt_ready else
                              "in attesa del control block RTT (core in attesa del debugger?)", flush=True)
                    next_try = time.monotonic() + 0.5
                continue
            for ch in CHANNELS.values():
                data = jl.rtt_read(ch, 4096)
                if not data:
                    continue
                out = bytes(data)
                for c, cch in list(clients.items()):
                    if cch == ch:
                        try:
                            c.sendall(out)
                        except OSError:
                            clients.pop(c)
                            c.close()
    except KeyboardInterrupt:
        pass
    finally:
        try:
            jl.rtt_stop()
            jl.close()
        except Exception:
            pass


def ensure_hub(args):
    if hub_alive():
        return
    log = os.environ.get("RTT_HUB_LOG", "/tmp/safety-island-rtt-hub.log")
    # Lock: piu' terminali avviati insieme (task VS Code) non devono creare piu' hub
    fd = os.open(log + ".lock", os.O_CREAT | os.O_RDWR)
    fcntl.flock(fd, fcntl.LOCK_EX)
    try:
        if hub_alive():
            return
        with open(log, "w") as f:
            p = subprocess.Popen([sys.executable, "-I", os.path.abspath(__file__), "hub", "--core", args.core],
                                 stdout=f, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL,
                                 start_new_session=True)
        pidfile = os.environ.get("RTT_HUB_PID")
        if pidfile:
            with open(pidfile, "w") as f:
                f.write(str(p.pid))
        for _ in range(200):
            if hub_alive():
                return
            if p.poll() is not None:
                sys.exit("hub RTT non partito:\n" + open(log).read()[-800:])
            time.sleep(0.1)
        sys.exit("hub RTT non pronto (log: %s)" % log)
    finally:
        fcntl.flock(fd, fcntl.LOCK_UN)
        os.close(fd)


# ---------------------------------------------------------------------------------------- terminale
def run_term(args):
    ensure_hub(args)
    names = list(CHANNELS) if args.which == "all" else [args.which]
    socks = {socket.create_connection(("127.0.0.1", port_of(n))): n for n in names}
    print("==> RTT %s (hub :%d). %s Ctrl-C per uscire" %
          ("di tutti i core" if args.which == "all" else args.which, port_of(names[0]),
           "Input: '<core>: testo'." if args.which == "all" else "Input: righe di testo."), flush=True)
    target = names[0] if args.which != "all" else "m70"
    stdin_open = True
    try:
        while socks:
            r, _, _ = select.select(list(socks) + ([sys.stdin] if stdin_open else []), [], [])
            for s in r:
                if s is sys.stdin:
                    line = sys.stdin.readline()
                    if not line:
                        stdin_open = False
                        continue
                    name, sep, text = line.partition(":")
                    if args.which == "all" and sep and name.strip() in CHANNELS:
                        target = name.strip()
                        line = text.lstrip()
                    if line.strip():
                        for so, n in socks.items():
                            if n == target:
                                so.sendall(line.encode())
                else:
                    data = s.recv(4096)
                    if not data:  # hub chiuso (es. make run): il terminale si chiude
                        socks.pop(s)
                        s.close()
                        continue
                    sys.stdout.write(data.decode("utf-8", "replace"))
                    sys.stdout.flush()
    except KeyboardInterrupt:
        pass


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    h = sub.add_parser("hub")
    t = sub.add_parser("term")
    t.add_argument("which", choices=sorted(CHANNELS) + ["all"])
    for p in (h, t):
        p.add_argument("--core", default=os.environ.get("RTT_CORE", "M7_1"),
                       help="core a cui si collega l'hub (M33_1 | M7_0 | M7_1)")
        p.add_argument("--speed", type=int, default=4000)
    args = ap.parse_args()
    run_hub(args) if args.cmd == "hub" else run_term(args)


if __name__ == "__main__":
    main()
