DOC=manual
#BIB=bib
TEXFILES=*.tex
default: pdf

.SUFFIXES: .tex
.SUFFIXES: .tex

.PHONY: pdf pdf ps psquick all clean tarfile
clean:
	rm -f $(DOC).aux $(DOC).bbl $(DOC).cb \
	$(DOC).blg $(DOC).log $(DOC).dvi $(DOC).pdf $(DOC).ps $(DOC).out \
	*.aux *.log *.dvi *.blg *.out *.cb2

ps: $(DOC).tex *.tex
	latex $(DOC) && bibtex $(DOC) && latex $(DOC) && latex $(DOC) && dvips $(DOC)

psquick:
	latex $(DOC) && latex $(DOC) && dvips $(DOC)

pdf: $(DOC).tex
	pdflatex $(DOC) && bibtex $(DOC) && pdflatex $(DOC) && pdflatex $(DOC)

pdfquick: $(DOC).tex
	pdflatex $(DOC) 

all: ps pdf

tarfile:
	tar zcvf $(DOC).tar.gz Makefile $(TEXFILES) *.sty *.bst *.cls $(BIB).bib $(DOC).ps
