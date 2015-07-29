(TeX-add-style-hook "test"
 (lambda ()
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

