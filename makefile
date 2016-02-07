OUTPUT= simulator
MAINFILE= main.c
HELPFILES= grid.c sim.c
HEADERS= grid.h sim.h
CC=gcc

CFLAGS =-Wall -g 

all: $(OUTPUT)

$(OUTPUT): $(MAINFILE) $(HELPFILES) $(HEADERS)
	$(CC) -o $(OUTPUT) $(CFLAGS) $(MAINFILE) $(HELPFILES) 

clean:
	-@rm -f $(OUTPUT)
