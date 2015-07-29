(TeX-add-style-hook "presentation"
 (lambda ()
    (LaTeX-add-labels
     "eq:1"
     "eq:2")
    (TeX-run-style-hooks
     "natbib"
     "verbatim"
     "tikz"
     "times"
     "minted"
     "listings"
     "color"
     "array"
     "latex2e"
     "beamer12"
     "beamer"
     "12pt")))

