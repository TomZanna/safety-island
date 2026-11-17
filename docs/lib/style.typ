// Stile del documento e helper condivisi dai capitoli.
// I capitoli fanno solo: #import "lib/style.typ": *  (o "../lib/style.typ" da ch/ e ap/)
// Nota: in Typst i nomi importati NON sono riesportati, quindi cio' che serve a capitoli e main.typ
// viene ri-esportato qui sotto con un #let esplicito.
#import "packages.typ": term as pkg-term, terms as pkg-terms, cetz-canvas as pkg-canvas, cetz-draw as pkg-draw, glossary-setup as pkg-glossary-setup, glossary-register as pkg-glossary-register, glossary-print as pkg-glossary-print, code-new as pkg-code-new, code-theme as pkg-code-theme, code-langs as pkg-code-langs, code-languages as pkg-code-languages, heading-here as pkg-heading-here

#let term = pkg-term
#let terms = pkg-terms
#let cetz-canvas = pkg-canvas
#let cetz-draw = pkg-draw
#let glossary-setup = pkg-glossary-setup
#let glossary-register = pkg-glossary-register
#let glossary-print = pkg-glossary-print
#let code-new = pkg-code-new

// ------------------------------------------------------------------ pagina --
#let accent = rgb("#1f4e79")
#let muted = luma(110)
#let rule-col = luma(205)

#let document-setup(body) = {
  set document(title: "Safety Island — documento tecnico", author: "safety-island")
  set page(
    paper: "a4",
    margin: (x: 2.6cm, y: 2.8cm),
    numbering: "1",
    header: context {
      let chap = pkg-heading-here(1)
      let sec = pkg-heading-here(2)
      if chap != none and page.numbering != none and counter(page).get().first() > 1 {
        set text(size: 8pt, fill: muted)
        grid(
          columns: (1fr, 1fr),
          align(left)[#chap],
          align(right)[#sec],
        )
        v(-0.35em)
        line(length: 100%, stroke: 0.4pt + rule-col)
      }
    },
  )
  set text(size: 10.5pt, lang: "it", hyphenate: true)
  set par(justify: true, leading: 0.62em)
  set heading(numbering: "1.1")
  show heading.where(level: 1): it => {
    pagebreak(weak: true)
    v(0.3em)
    set text(size: 19pt, weight: "medium", fill: accent)
    it
    v(0.2em)
    line(length: 100%, stroke: 0.8pt + accent)
    v(0.4em)
  }
  show heading.where(level: 2): it => {
    v(0.5em)
    set text(size: 13.5pt, weight: "medium")
    it
    v(0.15em)
  }
  show heading.where(level: 3): it => {
    v(0.3em)
    set text(size: 11.5pt, weight: "medium", style: "italic")
    it
  }
  // Listati: codly 2 con tema chiaro e linguaggi.
  show raw.where(block: true): code-new
  show: pkg-code-theme("github-light")
  show: pkg-code-langs(languages: pkg-code-languages)
  body
}

// ------------------------------------------------------------------ blocchi --
#let nota(body) = block(
  width: 100%,
  inset: (x: 0.8em, y: 0.6em),
  fill: rgb("#eef4fb"),
  stroke: (left: 2.5pt + accent),
  radius: 2pt,
  above: 0.8em,
  below: 0.8em,
  text(size: 9.8pt, body),
)

#let attenzione(body) = block(
  width: 100%,
  inset: (x: 0.8em, y: 0.6em),
  fill: rgb("#fdf3e7"),
  stroke: (left: 2.5pt + rgb("#b45309")),
  radius: 2pt,
  above: 0.8em,
  below: 0.8em,
  text(size: 9.8pt, body),
)

// Tabella con lo stile del documento: header ripetuto, righe sottili.
#let si-table(columns: auto, header: none, rows: (), size: 9.2pt) = {
  set text(size: size)
  let head = if header == none or header == () { () } else { (table.header(..header),) }
  table(
    columns: columns,
    stroke: 0.4pt + rule-col,
    inset: (x: 0.5em, y: 0.35em),
    align: left,
    ..head,
    ..rows.flatten(),
  )
}

// Listato preso dal sorgente vero: se cambia il file, cambia il PDF.
// `range` e' l'intervallo di righe (1-based, inclusivo); il nome mostrato lo ricava
// codly dal percorso (in codly 2 `file` non e' un parametro con nome).
#let si-listing(caption: none, src: none, lines: none, above: 0.8em) = figure(
  kind: raw,
  caption: caption,
  placement: none,
  supplement: [Listato],
  {
    code-new(
      path(src),
      range: lines,
    )
  },
)
