(TeX-add-style-hook "python2"
 (lambda ()
    (LaTeX-add-labels
     "ex1"
     "ex2")
    (TeX-run-style-hooks
     "hyperref"
     "listings"
     "color"
     "vmargin"
     "tikz"
     "amsmath"
     "graphicx"
     "latex2e"
     "scrartcl10"
     "scrartcl"
     "article"
     "10pt")))

