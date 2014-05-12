
BUILD_DIR=build
CC=g++
#CFLAGS=-c -DBENCH -I $(BUILD_DIR)
CFLAGS=-c -DSQLITE -DBENCH -I $(BUILD_DIR)
LDFLAGS= -lsqlite3

.PHONY: clean ref solver yass cmankey

all: pef reference solver yass cmankey

# Plugin solver
PEF_SOURCES=main.c sqlite3.c
PEF_OBJECTS=$(PEF_SOURCES:.c=.o)
PEF_EXE=psolv
PEF_CLEAN=$(PEF_OBJECTS)

pef: $(PEF_OBJECTS)
	$(CC) $(PEF_OBJECTS) -o $(PEF_EXE) $(LDFLAGS) -ldl

# Reference solver
REF_SOURCES=main.c solve.c sqlite3.c
REF_OBJECTS=$(REF_SOURCES:.c=.o)
REF_EXE=reference
REF_CLEAN=$(REF_OBJECTS)

ref: $(REF_OBJECTS)
	$(CC) $(REF_OBJECTS) -o $(REF_EXE) $(LDFLAGS) 

# Reference implementation (my stuff)
reference:
	make -f Makefile.reference

# Yass
yass:
	make -f Makefile.yass

# Sudoku Solver
solver:
	make -f Makefile.solver

# Code Mankeys Sudoku solver
cmankey:
	make -f Makefile.cmankey
	make -f Makefile.cmankey-lilo

CLEAN_LIST = $(REF_CLEAN)

clean:
	$(RM) -r $(CLEAN_LIST)
	make -f Makefile.solver clean
	make -f Makefile.yass clean
	make -f Makefile.reference clean
	make -f Makefile.cmankey clean
	make -f Makefile.cmankey-lilo clean

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@