# doxygen-awesome-css (vendored)

File copiato dal progetto upstream, non modificato:

- upstream: https://github.com/jothepro/doxygen-awesome-css
- tag: `v2.5.0` (2026-09-12)
- file: `doxygen-awesome.css` (MIT, vedi `LICENSE`)
- compatibilita': Doxygen 1.9.6-1.18.0; il progetto pinna Doxygen 1.18.0.

Aggiornare significa sostituire `doxygen-awesome.css` con quello del nuovo tag,
aggiornare `VERSION` e questo README, e riprovare `make docs` con
`-DDOCS_AWESOME_CSS=ON` (e `OFF`, per il fallback).

Il tema si disattiva dalla cache CMake:

```bash
cmake --preset mx94evk -DDOCS_AWESOME_CSS=OFF && make docs
```
