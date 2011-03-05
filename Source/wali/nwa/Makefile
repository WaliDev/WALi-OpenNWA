CXXFLAGS=-I../.. -m32 -g
LDFLAGS=-lwali -L../../../lib -m32 -g

NwaParserTests: NwaParserMain.o wali
	$(CXX) $(LDFLAGS) -o $@ $< $(LOADLIBES) $(LDLIBS)

NwaParserMain.o: NwaParserMain.cpp

.PHONY: wali
wali:
	cd ../../.. &&	scons arch=32 -j4