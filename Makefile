all:	trax trax-httpd

CXXFLAGS = -Wall

SRCS = trax.cc move.cc trace.cc validation.cc
OBJS = $(SRCS:%.cc=%.o)

.SUFFIXES: .cc

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

trax-httpd: trax-httpd.cc
	$(CXX) $(CXXFLAGS) -o trax-httpd $(LDFLAGS) trax-httpd.cc

trax:	$(OBJS)
	$(CXX) $(CXXFLAGS) -o trax $(OBJS) $(LDFLAGS)

all:	trax

clean:
	-rm -rf *.o *~ core trax trax-httpd
