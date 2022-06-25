CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-O2 -std=c++17
LDFLAGS=
LDLIBS=-lsfml-graphics -lsfml-window -lsfml-system

SRCS=island.cc support.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: island

island: $(OBJS)
	$(CXX) $(LDFLAGS) -o island $(OBJS) $(LDLIBS)

island.o: island.cc support.hh

support.o: support.hh support.cc

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) island
