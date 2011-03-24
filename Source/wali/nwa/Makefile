OPENFST_DIR=../../../../openfst/openfst-1.1

CXXFLAGS=-I../.. -I$(OPENFST_DIR)/src/include -g

LDFLAGS=-lwali -L../../../lib64 -g
#LDFLAGS+=-lfst -L$(OPENFST_DIR)/src/lib/.libs


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
