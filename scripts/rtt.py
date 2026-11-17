#!/usr/bin/env python3
"""Console RTT dei core secondari da terminale (make rtt / make rtt-<app>), fuori dal debug VS Code.

Il control block RTT e' uno solo, condiviso da M33S, M7_0 e M7_1 (fw/common/rtt_shared.h) a 0x204D0000,
un canale per core (0 m33s, 1 m70, 2 m71). Una connessione J-Link (collegata a M7_1 di default, stesso
indirizzo per tutti i core), senza reset ne' halt, legge i canali e li stampa.

Una connessione J-Link per processo: la sonda ne accetta al massimo 5 in totale, quindi con 3 GDB server
aperti restano 2 slot (in debug l'RTT passa dal rttConfig della sessione, senza aggiungerne).

Durante il debug in VS Code l'RTT lo legge Cortex-Debug (rttConfig, un canale per sessione): non usare
questa console sugli stessi canali, due lettori dello stesso canale si rubano i dati.

Input: senza --channel, "<core>: testo" va al canale di quel core ("<core>:" da solo cambia il default);
con --channel, ogni riga va a quel core. Ctrl-C per uscire.
"""
import argparse
import os
import select
import sys
import time

import pylink

CHANNELS = {"m33s": 0, "m70": 1, "m71": 2}
ADDR = "0x204D0000"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--channel", choices=sorted(CHANNELS), default=os.environ.get("RTT_CHANNEL") or None,
                    help="solo il canale di questo core (default: tutti)")
    ap.add_argument("--core", default=os.environ.get("RTT_CORE", "M7_1"),
                    help="core a cui si collega il J-Link (M33_1 | M7_0 | M7_1)")
    ap.add_argument("--speed", type=int, default=4000)
    args = ap.parse_args()
    channels = {args.channel: CHANNELS[args.channel]} if args.channel else CHANNELS

    jl = pylink.JLink()
    # Un solo tentativo: se la sonda e' occupata o le 5 connessioni sono esaurite, meglio un errore
    # chiaro e subito che un retry lungo (l'utente decide quale sessione chiudere).
    try:
        jl.open()
        # Le dialog box della DLL (es. "connect under reset") in un terminale non grafico restano
        # appese e connect() non torna mai: sempre disabilitate.
        jl.disable_dialog_boxes()
    except Exception as e:
        msg = str(e)
        if "maximum number of connections" in msg:
            sys.exit("ERRORE: tutte le 5 connessioni della sonda sono occupate.\n"
                     "Chiudi una sessione (make rtt, GDB server, RTT Viewer/J-Scope) e riprova.")
        sys.exit("ERRORE: J-Link non disponibile: %s\n"
                 "Riprova tra un secondo; se persiste, controlla 'make usb-status'." % msg)
    jl.set_tif(pylink.enums.JLinkInterfaces.SWD)
    jl.set_speed(args.speed)
    jl.exec_command("SetRTTAddr %s" % ADDR)
    jl.connect("MIMX94398_%s" % args.core, speed=args.speed, verbose=False)  # attach: niente reset/halt

    print("==> RTT %s via %s (control block %s). %s Ctrl-C per uscire" %
          (args.channel or "di tutti i core", args.core, ADDR,
           "Input: righe di testo." if args.channel else "Input: '<core>: testo'."), flush=True)

    # Il control block lo crea il primo core che arriva a board_init(): con WAIT_DEBUGGER si aspetta
    waiting_said = False
    while True:
        try:
            jl.rtt_start(int(ADDR, 0))
            ok = False
            for _ in range(10):
                try:
                    if jl.rtt_get_num_up_buffers() >= len(CHANNELS):
                        ok = True
                        break
                except pylink.errors.JLinkRTTException:
                    pass
                time.sleep(0.05)
            if ok:
                break
            jl.rtt_stop()
        except (pylink.errors.JLinkException, ValueError):
            pass
        if not waiting_said:
            print("in attesa del control block RTT (core in attesa del debugger?)", flush=True)
            waiting_said = True
        time.sleep(0.5)

    target = args.channel or "m70"
    stdin_open = True
    read_error_said = False
    try:
        while True:
            for ch in channels.values():
                try:
                    data = jl.rtt_read(ch, 4096)
                except Exception as e:
                    # pylink puo' sollevare ValueError per codici che non conosce (es. -11 quando il
                    # core e' fermo): meglio una riga di errore che un traceback.
                    if not read_error_said:
                        print("\n[errore RTT: %s; continuo]" % e, file=sys.stderr, flush=True)
                        read_error_said = True
                    continue
                if data:
                    sys.stdout.write(bytes(data).decode("utf-8", "replace"))
            sys.stdout.flush()
            if stdin_open:
                r, _, _ = select.select([sys.stdin], [], [], 0)
                if r:
                    line = sys.stdin.readline()
                    if not line:
                        stdin_open = False
                        continue
                    name, sep, text = line.partition(":")
                    if not args.channel and sep and name.strip() in CHANNELS:
                        target = name.strip()
                        line = text.lstrip()
                    if line.strip():
                        try:
                            jl.rtt_write(CHANNELS[target], list(line.encode()))
                        except Exception as e:
                            print("\n[errore RTT in scrittura: %s]" % e, file=sys.stderr, flush=True)
            time.sleep(0.01)
    except KeyboardInterrupt:
        pass
    finally:
        try:
            jl.rtt_stop()
            jl.close()
        except Exception:
            pass


if __name__ == "__main__":
    main()
