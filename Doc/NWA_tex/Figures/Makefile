BINDIR=../../../bin

PRINTDOT=$(BINDIR)/print-dot.exe

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
      star-complex                  \
      reverse-of-figure-1           \
      determinize                   \
      determinize-result            \
      complement-of-determinize     \
      pds-equivalent


all: figure-1.png union-other.png union-result.png

%.dot: %.nwa
	$(PRINTDOT) -o $@ $<

%.png: %.dot
	$(DOT) -Tpng -o $@ $<


union-result.nwa: figure-1.nwa union-other.nwa
intersection-simple-result.nwa: figure-1.nwa intersection-simple-other.nwa
intersection-complex-result.nwa: intersection-complex-first.nwa ntersection-complex-second.nwa
concat-result.nwa: figure-1.nwa concat-other.nwa
star-simple-result.nwa: star-simple.nwa
star-complex-result.nwa: star-complex.nwa
reverse-of-figure-1.nwa: figure-1.nwa
determinize-result.nwa: determinize.nwa
complement-of-determinize.nwa: determinize.nwa
