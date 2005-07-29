CXX=libtool g++
CXXBASE=-ggdb -Wall -W -Werror -Wformat=2
INCLUDE=-I../Source -I..
CXXFLAGS=$(CXXBASE) $(INCLUDE)
TESTS=t1 t2 t3 t4

all: $(TESTS)

.PHONY: clean
clean:
	rm -f *.o *.lo *.dot *.xml *~ core $(TESTS)

.SUFFIXES: .lo .cpp

.cpp.lo:
	$(CXX) $(CXXFLAGS) -o $@ -c $^

t1: t1.lo ../Examples/Reach/Reach.lo
	$(CXX) $(CXXFLAGS) -o $@ $^ -lwali

t2: t2.lo ../Examples/Reach/Reach.lo
	$(CXX) $(CXXFLAGS) -o $@ $^ -lwali

t3: t3.lo ../Examples/Reach/Reach.lo
	$(CXX) $(CXXFLAGS) -o $@ $^ -lwali

t4: t4.lo ../Examples/Reach/Reach.lo
	$(CXX) $(CXXFLAGS) -o $@ $^ -lwali
