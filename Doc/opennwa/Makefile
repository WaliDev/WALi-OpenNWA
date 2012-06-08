opennwa.pdf: *.tex *.bib Figures/*.tex Example/example-abbrev.cpp
	pdflatex -shell-escape opennwa
	bibtex opennwa
	pdflatex -shell-escape opennwa
	pdflatex -shell-escape opennwa
