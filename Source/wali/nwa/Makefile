CXXFLAGS=-I../.. -g
LDFLAGS=-lwali -L../../../lib64 -g
LDFLAGS+=-lfst -L../../../../openfst/openfst-1.1/src/lib/.libs


run-nwa-tests: NWA-tests
	echo "Running tests:"
	./NWA-tests

run-parser-tests: NwaParserTests
	echo "Running tests:"
	./NwaParserTests 3

NwaParserTests: NwaParserMain.o wali
	$(CXX) $(LDFLAGS) -o $@ $< $(LOADLIBES) $(LDLIBS)

NWA-tests: NWA-tests.o wali
	$(CXX) $(LDFLAGS) -o $@ $< $(LOADLIBES) $(LDLIBS)


NwaParserMain.o: NwaParserMain.cpp
NWA-tests.o: NWA-tests.cpp

.PHONY: wali
wali:
	cd ../../.. &&	scons -j4
