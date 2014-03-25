
CC=g++
CFLAGS=-c -DBENCH 
LDFLAGS= 

REF_SOURCES=main.c solve.c
REF_OBJECTS=$(REF_SOURCES:.c=.o)
REF_EXE=reference
REF_CLEAN=$(REF_EXE) $(REF_OBJECTS)

YASS_SOURCES=main.c yass-1.0.c yass-src/src/solver.cc
YASS_TMP=$(YASS_SOURCES:.c=.o)
YASS_OBJECTS=$(YASS_TMP:.cc=.o)
YASS_ARCHIVE:=yass-src.tar.gz
CFLAGS+=-I yass-src
YASS_EXE=yass
YASS_CLEAN=$(YASS_EXE) $(YASS_OBJECTS) $(YASS_ARCHIVE)

CLEAN_LIST = $(REF_CLEAN) $(YASS_CLEAN)

ref: $(REF_OBJECTS)
	$(CC) $(REF_OBJECTS) -o $(REF_EXE) $(LDFLAGS) 

yass: yass-src.tar.gz $(YASS_OBJECTS) 
	$(CC) $(YASS_OBJECTS) -o $(YASS_EXE) $(LDFLAGS) 

yass-src.tar.gz:
	wget -N -O $(YASS_ARCHIVE) "http://downloads.sourceforge.net/project/yasudokusolver/yasudokusolver/yass-1.0/yass-1.0.tar.gz?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fyasudokusolver%2Ffiles%2Fyasudokusolver%2F&ts=1395329568&use_mirror=heanet"
	tar xzvf $(YASS_ARCHIVE)
	mv yass-1.0 yass-src; cd yass-src; ./configure; make; cd ..

clean:
	@- $(RM) $(CLEAN_LIST)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@