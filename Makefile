all:	trax

CXXFLAGS = -Wall

SRCS = trax.cc move.cc validation.cc
OBJS = $(SRCS:%.cc=%.o)

.SUFFIXES: .cc

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

trax:	$(OBJS)
	$(CXX) $(CXXFLAGS) -o trax $(OBJS) $(LDFLAGS)

all:	trax

clean:
	-rm -rf *.o *~ core trax
