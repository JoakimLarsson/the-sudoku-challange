
CC=g++
CFLAGS=-c -DBENCH 
LDFLAGS= 

REF_SOURCES=main.c solve.c
REF_OBJECTS=$(REF_SOURCES:.c=.o)
REF_EXE=reference
REF_CLEAN=$(REF_EXE) $(REF_OBJECTS)

CLEAN_LIST = $(REF_CLEAN)

ref: $(REF_OBJECTS)
	$(CC) $(REF_OBJECTS) -o reference $(LDFLAGS) 

clean:
	@- $(RM) $(CLEAN_LIST)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@