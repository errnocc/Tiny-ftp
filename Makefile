CC=gcc
CFLAGS= -c -w
LDFLAGS=-lm -g -w
SOURCES=server.c handles.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ftp
DEBUG ?= 1

ifeq ($(DEBUG),1)
	CFLAGS += -g
else
	CFLAGS += -o2
endif

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *.o ftp
