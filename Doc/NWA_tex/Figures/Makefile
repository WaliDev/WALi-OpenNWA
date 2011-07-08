BINDIR=../../../bin64

PRINTDOT=$(BINDIR)/nwa-print-dot.exe
UNION=$(BINDIR)/nwa-union.exe
INTERSECT=$(BINDIR)/nwa-intersect.exe
CONCAT=$(BINDIR)/nwa-concatenate.exe
STAR=$(BINDIR)/nwa-star.exe
REVERSE=$(BINDIR)/nwa-reverse.exe
DETERMINIZE=$(BINDIR)/nwa-determinize.exe
COMPLEMENT=$(BINDIR)/nwa-complement.exe

DOT=dot
DOT2TEX=dot2tex

FILES=figure-1                      \
      union-other                   \
      union-result                  \
      intersection-simple-other     \
      intersection-simple-result    \
      intersection-complex-first    \
      intersection-complex-second   \
      intersection-complex-result   \
      concat-other                  \
      concat-result                 \
      star-simple                   \
      star-simple-result            \
      star-complex                  \
      star-complex-result           \
      reverse-of-figure-1           \
      determinize                   \
      determinize-result            \
      complement-of-determinize     \
      pds-equivalent                \
      pre-pds-nwa


all: $(addsuffix .png,$(FILES))  $(addsuffix .tex,$(FILES))

%.dot: %.nwa
	$(PRINTDOT) -o $@ $<

%.png: %.dot
	$(DOT) -Tpng -o $@ $<

%.tex: %.dot
	$(DOT2TEX) --preproc --usepdflatex --nominsize --prog neato $< | $(DOT2TEX) -o $@ --figonly --nominsize --prog neato
	#--autosize --t raw

union-result.nwa: figure-1.nwa union-other.nwa
	$(UNION) -o $@ $^

intersection-simple-result.nwa: figure-1.nwa intersection-simple-other.nwa
	$(INTERSECT) -o $@ $^
intersection-complex-result.nwa: intersection-complex-first.nwa intersection-complex-second.nwa
	$(INTERSECT) -o $@ $^

concat-result.nwa: figure-1.nwa concat-other.nwa
	$(CONCAT) -o $@ $^

star-simple-result.nwa: star-simple.nwa
	$(STAR) -o $@ $^
star-complex-result.nwa: star-complex.nwa
	$(STAR) -o $@ $^

reverse-of-figure-1.nwa: figure-1.nwa
	$(REVERSE) -o $@ $^

determinize-result.nwa: determinize.nwa
	$(DETERMINIZE) -o $@ $^

complement-of-determinize.nwa: determinize-result.nwa
	$(COMPLEMENT) -o $@ $^

clean:
	-rm $(addsuffix .png,$(FILES))  $(addsuffix .tex,$(FILES)) $(addsuffix .dot,$(FILES))
