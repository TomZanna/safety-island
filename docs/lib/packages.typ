// Pin dei package typst esterni: UNICO posto da modificare per aggiornarli.
// I capitoli non importano mai "@preview/...": importano lib/style.typ, che importa questo file.
//
// Richiede typst >= 0.15.1 (vedi docs/CMakeLists.txt): cetz 0.5.x e codly 2.x vogliono >= 0.15.
//
//   package          pin      min typst   perche'
//   cetz             0.5.3    0.15.0      figure: architettura, memoria, MU18, sequenza, stati
//   codly            2.0.0    0.15.0      listati con numeri di riga, badge file, estratti dai sorgenti
//   codly-languages  0.1.10   -           nomi/icone dei linguaggi (C)
//   hydra            0.6.3    0.12.0      intestazione corrente pari/dispari
//   glossarium       0.5.10   0.13.0      glossario degli acronimi
//
// Primo build: i package si scaricano dal registry in ~/.cache/typst/packages/preview/.
// Per build offline: copiare quella directory e passare -DDOCS_PACKAGE_PATH=<dir> (vedi CMakeLists).
#import "@preview/cetz:0.5.3": canvas, draw
#import "@preview/codly:2.0.0" as codly
#import "@preview/codly-languages:0.1.10": codly-languages
#import "@preview/hydra:0.6.3": hydra
#import "@preview/glossarium:0.5.10": make-glossary, register-glossary, print-glossary, gls, glspl

// Re-export esplicito: e' cio' che i capitoli possono usare.
#let cetz-canvas = canvas
#let cetz-draw = draw
#let code-new = codly.new
#let code-theme = codly.theme
#let code-langs = codly.lang-set_
#let code-numbering = codly.number-set_
#let code-languages = codly-languages
#let heading-here = hydra
#let glossary-setup = make-glossary
#let glossary-register = register-glossary
#let glossary-print = print-glossary
#let term = gls
#let terms = glspl
