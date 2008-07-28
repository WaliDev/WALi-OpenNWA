CXX=g++
CXXBASE=-g -Wall 
CXXFLAGS=$(CXXBASE)
CXXSFX=.cpp
WALI_DIR=../../..
INCLUDE=-I. -I$(WALI_DIR)
LIBS=-L$(WALI_DIR)/build -lwali
SRCS=ExReach$(CXXSFX) ../Reach$(CXXSFX)
OBJS=$(SRCS:$(CXXSFX)=.o)
BINARY=ExReach

all: $(BINARY)

%.d: %$(CXXSFX)
	set -e; $(CXX) -M $(CXXFLAGS) $(INCLUDE) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

$(CXXSFX).o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(BINARY): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)


#include $(SRCS:$(CXXSFX)=.d) 

.PHONY: clean
clean:
	rm -rf *.d *.o ../*.o *~ core $(BINARY)
