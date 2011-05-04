CXX=g++

OPENFST_DIR=../../../../openfst/openfst-1.1

CXXFLAGS=-I../.. -I$(OPENFST_DIR)/src/include -g

LDFLAGS=-lwali -L../../../lib64 -g
#LDFLAGS+=-lfst -L$(OPENFST_DIR)/src/lib/.libs

all: run-selector-tests

#run-nwa-tests: NWA-tests
#	echo "Running tests:"
#	./NWA-tests

run-selector-tests: test_selectors
	echo "Running tests"
	./test_selectors 3

run-parser-tests: NwaParserTests
	echo "Running tests:"
	./NwaParserTests 3

NwaParserTests: NwaParserMain.o wali
	$(CXX) $(LDFLAGS) -o $@ $< $(LOADLIBES) $(LDLIBS)

#NWA-tests: NWA-tests.o wali
#	$(CXX) $(LDFLAGS) -o $@ $< $(LOADLIBES) $(LDLIBS)

test_selectors: test_selectors.o wali
	$(CXX) $(LDFLAGS) -o $@ $< $(LOADLIBES) $(LDLIBS)

test_selectors.o: query/details/test_selectors.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

NwaParserMain.o: NwaParserMain.cpp
#NWA-tests.o: NWA-tests.cpp

.PHONY: wali
wali:
	cd ../../.. &&	scons -j4
