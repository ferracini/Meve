SRCDIR = src
ODIR = build
BINDIR = bin

_DEPS = mevenum.h mevemath.h meve.h
DEPS = $(patsubst %,$(SRCDIR)/%,$(_DEPS))

_OBJ = mevemath.o meve.o test.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Specify compiler
CC=gcc
CFLAGS=-pedantic -Wall -O2 -lm

# Link the object files into a binary
$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	@mkdir -p $(BINDIR)
	@mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# Compile the source files into object files
$(BINDIR)/meve: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o

cleanall:
	rm -f $(BINDIR)/meve
	rm -f $(ODIR)/*.o