CXX=g++
CXXBASE=-ggdb -Wall -W -Werror -Wformat=2
INCLUDE=-I../
CXXFLAGS=$(CXXBASE) $(INCLUDE)
TESTS=t1 t2 t3 t4

all: $(TESTS)

.PHONY: clean
clean:
	rm -f *.o *~ core $(TESTS)

t1: t1.cpp ../Examples/Reach/Reach.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lwali -L../build

t2: t2.cpp ../Examples/Reach/Reach.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lwali -L../build

t3: t3.cpp ../Examples/Reach/Reach.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lwali -L../build

t4: t4.cpp ../Examples/Reach/Reach.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lwali -L../build
