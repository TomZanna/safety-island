#!/usr/bin/env python3
"""Verifica il report MISRA di cppcheck contro la baseline delle deviazioni.

Il report e' quello prodotto da scripts/misra.sh: contiene TUTTI i finding,
anche quelli gia' accettati, cosi' il confronto funziona nei due sensi.

- senza --check stampa il riepilogo per regola e le eventuali differenze;
- con --check esce 1 se c'e' un finding NON in baseline (regressione) oppure
  una riga di baseline che non corrisponde piu' a un finding (deviazione
  obsoleta: il codice e' cambiato e la riga va rivalutata, non copiata).

Uso: misra_check.py [--report FILE] [--suppressions FILE] [--check]
Ambiente: SI_BUILD_DIR (default build/mx94evk).
"""
import argparse
import os
import re
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

# --template di misra.sh: "{file}:{line}:{column}: {severity}: {id}: {message}"
FINDING = re.compile(
    r"^(?P<file>[^:]+):(?P<line>\d+):(?P<column>\d+): [^:]+: "
    r"(?P<id>misra-c2012-[\d.]+):"
)


def die(message: str) -> None:
    print(f"ERRORE: {message}", file=sys.stderr)
    sys.exit(1)


def rule_key(rule_id: str) -> tuple[int, ...]:
    return tuple(int(part) for part in rule_id.replace("misra-c2012-", "").split("."))


def relative(path: str) -> str:
    """Path relativo alla radice del repo, se possibile."""
    candidate = Path(path)
    if candidate.is_absolute():
        try:
            return str(candidate.resolve().relative_to(ROOT))
        except ValueError:
            return str(candidate)
    return str(candidate)


def parse_findings(path: Path) -> list[tuple[str, str, int]]:
    try:
        lines = path.read_text(encoding="utf-8").splitlines()
    except OSError as exc:
        die(f"report MISRA non leggibile ({path}): {exc}")
    findings = []
    for line in lines:
        match = FINDING.match(line)
        if match:
            findings.append((match["id"], relative(match["file"]), int(match["line"])))
    return findings


def parse_suppressions(path: Path) -> list[tuple[str, str, int]]:
    try:
        lines = path.read_text(encoding="utf-8").splitlines()
    except OSError as exc:
        die(f"baseline non leggibile ({path}): {exc}")
    suppressions = []
    for line in lines:
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        try:
            rule_id, rest = line.split(":", 1)
            filename, lineno = rest.rsplit(":", 1)
            suppressions.append((rule_id, relative(filename), int(lineno)))
        except ValueError:
            die(f"riga non valida in {path}: '{line}' (formato <id>:<file>:<line>)")
    return suppressions


def show(finding: tuple[str, str, int], with_source: bool) -> str:
    rule_id, filename, lineno = finding
    text = f"  {filename}:{lineno}: {rule_id}"
    if with_source:
        try:
            source = (ROOT / filename).read_text(encoding="utf-8").splitlines()
            text += f"\n      {source[lineno - 1].strip()}"
        except (OSError, IndexError):
            pass
    return text


def main() -> int:
    build = Path(os.environ.get("SI_BUILD_DIR", "build/mx94evk"))
    if not build.is_absolute():
        build = (ROOT / build).resolve()

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--report", type=Path, default=build / "reports" / "misra.txt")
    parser.add_argument(
        "--suppressions", type=Path, default=ROOT / "quality" / "misra-suppressions.txt"
    )
    parser.add_argument("--check", action="store_true", help="fallisce su regressioni o righe obsolete")
    args = parser.parse_args()

    findings = parse_findings(args.report)
    baseline = parse_suppressions(args.suppressions)

    baseline_set = set(baseline)
    if len(baseline) != len(baseline_set):
        duplicates = [item for item, count in Counter(baseline).items() if count > 1]
        die(
            "baseline con righe duplicate: "
            + ", ".join(f"{rule}:{file}:{line}" for rule, file, line in duplicates)
        )

    finding_set = set(findings)
    new = sorted(finding_set - baseline_set)
    stale = sorted(baseline_set - finding_set)

    counts = Counter(rule_id for rule_id, _, _ in findings)
    print(
        f"MISRA: {len(findings)} finding, {len(baseline_set)} deviazioni in baseline "
        f"({len(new)} nuove, {len(stale)} obsolete)"
    )
    for rule_id, count in sorted(counts.items(), key=lambda item: rule_key(item[0])):
        print(f"  {rule_id.replace('misra-c2012-', ''):>5}  {count:>3}")

    if new:
        print(f"\n{len(new)} finding non in baseline:", file=sys.stderr)
        for finding in new:
            print(show(finding, True), file=sys.stderr)
    if stale:
        print(f"\n{len(stale)} deviazioni obsolete:", file=sys.stderr)
        for finding in stale:
            print(show(finding, False), file=sys.stderr)

    if args.check and (new or stale):
        print(
            "\nCorreggi i finding nuovi oppure, dopo la review, aggiorna "
            "quality/misra-suppressions.txt con il motivo della deviazione.",
            file=sys.stderr,
        )
        return 1

    if args.check:
        print("MISRA: baseline allineata (0 finding nuovi, 0 deviazioni obsolete)")

    return 0


if __name__ == "__main__":
    sys.exit(main())
