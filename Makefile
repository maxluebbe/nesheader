CC= gcc
CFLAGS=-Wall -O

all: nesheader.c
	$(CC) nesheader.c -o nesheader $(CFLAGS)

.PHONY: clean
clean:
	rm nesheader