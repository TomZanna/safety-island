#!/usr/bin/env python3
"""clang-tidy sui sorgenti C di fw/ usando il compile_commands.json delle app.

Seleziona una entry per file (le tre app compilano gli stessi sorgenti con
define diverse), esclude la libreria FSoE vendored e invoca clang-tidy con il
target ARM e il toolchain GCC in uso.

Uso: lint.py [--build DIR] [--tool EXE] [--fix] [--no-werror] [-j N]
Ambiente: SI_BUILD_DIR, SI_CLANG_TIDY, SI_ARMGCC_DIR.
"""
import argparse
import concurrent.futures
import json
import os
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def die(message: str) -> None:
    print(f"ERRORE: {message}", file=sys.stderr)
    sys.exit(1)


def find_tool(value: str) -> str:
    path = value or "clang-tidy-19"
    if Path(path).is_file():
        return path
    found = shutil.which(path)
    if found:
        return found
    die("clang-tidy assente: sudo apt install clang-tidy-19")
    return ""  # irraggiungibile


def compiler_of(entry: dict) -> str:
    if entry.get("arguments"):
        return str(entry["arguments"][0])
    command = entry.get("command", "")
    return str(shlex.split(command)[0]) if command else ""


def arm_gcc_root(entries: dict[Path, dict]) -> str:
    env = os.environ.get("SI_ARMGCC_DIR", "")
    if env:
        return str(Path(env).resolve())
    for entry in entries.values():
        compiler = compiler_of(entry)
        if "arm-none-eabi-gcc" in compiler:
            return str(Path(compiler).resolve().parent.parent)
    return ""


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", default=os.environ.get("SI_BUILD_DIR", "build/mx94evk"))
    parser.add_argument("--tool", default=os.environ.get("SI_CLANG_TIDY", "clang-tidy-19"))
    parser.add_argument("--fix", action="store_true", help="applica i fix di clang-tidy")
    parser.add_argument("--no-werror", action="store_true", help="non trasformare i warning in errore")
    parser.add_argument("-j", "--jobs", type=int, default=min(8, os.cpu_count() or 1))
    args = parser.parse_args()

    build = Path(args.build)
    if not build.is_absolute():
        build = (ROOT / build).resolve()
    database = build / "compile_commands.json"
    if not database.is_file():
        die(f"{database} assente: esegui 'make fw'")

    tool = find_tool(args.tool)
    try:
        entries = json.loads(database.read_text())
    except (OSError, json.JSONDecodeError) as exc:
        die(f"compile_commands.json non leggibile: {exc}")

    fw_root = ROOT / "fw"
    fsoe_root = fw_root / "common" / "fsoe"
    selected: dict[Path, dict] = {}
    for entry in entries:
        try:
            source = Path(entry["file"]).resolve()
        except (KeyError, OSError):
            continue
        if fw_root not in source.parents or fsoe_root in source.parents:
            continue
        selected.setdefault(source, entry)
    if not selected:
        die("nessun sorgente fw/ nel compile_commands.json: esegui 'make fw'")

    gcc_root = arm_gcc_root(selected)
    extra = [
        "--extra-arg=--target=arm-none-eabi",
        "--extra-arg=-Wno-unused-command-line-argument",
        "--extra-arg=-Wno-unknown-warning-option",
        "--extra-arg=-Wno-address-of-packed-member",
        # I warning del SDK/newlib non sono nostri: restano attivi i check
        # clang-tidy, ma il compilatore non inonda il report.
        "--extra-arg=-Wno-everything",
    ]
    if gcc_root:
        extra.append(f"--extra-arg=--gcc-toolchain={gcc_root}")
        newlib = Path(gcc_root) / "arm-none-eabi" / "include"
        if newlib.is_dir():
            extra.append(f"--extra-arg=-isystem{newlib}")
    if args.fix:
        extra.append("--fix")
    elif not args.no_werror:
        extra.append("--warnings-as-errors=*")

    base = [
        tool,
        "-p",
        str(build),
        "--quiet",
        "--use-color=0",
        "--header-filter=^(.*/)?fw/(common|safe|fieldbus)/",
        "--system-headers=0",
        *extra,
    ]

    def run(source: Path) -> tuple[Path, int, str]:
        proc = subprocess.run(
            [*base, str(source)],
            cwd=ROOT,
            text=True,
            capture_output=True,
            check=False,
        )
        raw = proc.stdout + proc.stderr
        lines = [
            line
            for line in raw.splitlines()
            if "warnings generated" not in line and not line.startswith("Error while processing")
        ]
        return source, proc.returncode, "\n".join(lines).strip()

    results = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, args.jobs)) as pool:
        futures = {pool.submit(run, source): source for source in selected}
        for future in concurrent.futures.as_completed(futures):
            results.append(future.result())
    results.sort(key=lambda item: str(item[0]))

    reports = build / "reports"
    reports.mkdir(parents=True, exist_ok=True)
    report = reports / "clang-tidy.txt"
    failed = 0
    with report.open("w", encoding="utf-8") as out:
        for source, code, output in results:
            if output:
                print(f"===== {source.relative_to(ROOT)} =====")
                print(output)
                out.write(f"===== {source.relative_to(ROOT)} =====\n{output}\n")
            if code != 0:
                failed += 1

    print(f"clang-tidy: {len(selected)} file, {failed} con finding (report: {report})")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
