#IDIR = include
CC=g++

CFLAGS= -std=c++14 -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -O3 -DNDEBUG

#ODIR=.
LIBDIR = lib

LIBS= -fopenmp

_DEPS = nasteroids-seq.cpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = nasteroids-seq.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


nasteroids-seq: nasteroids-seq.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

nasteroids-par: nasteroids-par.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 