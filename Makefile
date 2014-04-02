
BUILD_DIR=build
CC=g++
#CFLAGS=-c -DBENCH -I $(BUILD_DIR)
CFLAGS=-c -DSQLITE -DBENCH -I $(BUILD_DIR)
LDFLAGS= -lsqlite3

all: ref solver yass

# Reference solver
REF_SOURCES=main.c solve.c
REF_OBJECTS=$(REF_SOURCES:.c=.o)
REF_EXE=reference
REF_CLEAN=$(REF_OBJECTS)
ref: $(REF_OBJECTS)
	$(CC) $(REF_OBJECTS) -o $(REF_EXE) $(LDFLAGS) 

# YASS
YASS_SOURCES=main.c yass-1.0.c $(BUILD_DIR)/src/solver.cc
YASS_TMP=$(YASS_SOURCES:.c=.o)
YASS_OBJECTS=$(YASS_TMP:.cc=.o)
YASS_ARCHIVE:=yass-src.tar.gz
YASS_EXE=yass
YASS_CLEAN=$(YASS_OBJECTS) 

yass: yass-src.tar.gz $(YASS_OBJECTS) 
	$(CC) $(YASS_OBJECTS) -o $(YASS_EXE) $(LDFLAGS) 
	rm -r $(BUILD_DIR)

yass-src.tar.gz:
	wget -N -O $(YASS_ARCHIVE) "http://downloads.sourceforge.net/project/yasudokusolver/yasudokusolver/yass-1.0/yass-1.0.tar.gz?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fyasudokusolver%2Ffiles%2Fyasudokusolver%2F&ts=1395329568&use_mirror=heanet"
	tar xzvf $(YASS_ARCHIVE)
	mv yass-1.0 $(BUILD_DIR); cd $(BUILD_DIR); ./configure; make; cd ..

# Sudoku Solver
SS_SOURCES=main.c sudoku-0.3.c $(BUILD_DIR)/src/game.cc $(BUILD_DIR)/src/block3x3.cc
SS_TMP=$(SS_SOURCES:.c=.o)
SS_OBJECTS=$(SS_TMP:.cc=.o)
SS_ARCHIVE:=solver-src.tar.gz
SS_EXE=solver
SS_CLEAN=$(SS_OBJECTS) 
CFLAGS+= -I $(BUILD_DIR)/include

solver: solver-src.tar.gz $(SS_OBJECTS)
	$(CC) $(SS_OBJECTS) -o $(SS_EXE) $(LDFLAGS)
	rm -r $(BUILD_DIR)

solver-src.tar.gz:
	wget -N -O $(SS_ARCHIVE) "http://robert.rsa3.com/pmwiki/uploads/Projects/sudoku-0.3.tar.gz"
	tar xzvf $(SS_ARCHIVE)
	patch -p0 < sudoku-0.3.patch
	mv sudoku-0.3 $(BUILD_DIR); cd $(BUILD_DIR); ./configure; make; cd ..

CLEAN_LIST = $(REF_CLEAN) $(YASS_CLEAN) $(SS_CLEAN) $(BUILD_DIR) $(SS_ARCHIVE) $(YASS_ARCHIVE)

clean:
	@- $(RM) -r $(CLEAN_LIST)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@