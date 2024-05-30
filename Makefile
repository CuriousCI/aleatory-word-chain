# compilation
CC := gcc
CFLAGS := -ansi -Wall -Wextra -Wpedantic -Wunused-result -Wunused-value
SRC := src

# codex version
VERSION := 1.0 

# paths
PREFIX := /usr/local
MANPREFIX := $(PREFIX)/share/man

all: codex

codex : $(SRC)/main.o $(SRC)/codex/util.o $(SRC)/codex/csv.o $(SRC)/codex/text.o $(SRC)/collections/map.o $(SRC)/collections/vec.o
	$(CC) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -f $(SRC)/*.o $(SRC)/collections/*.o $(SRC)/codex/*.o codex

# DESTDIR can be specified if you don't want to use the default install location

install: all 
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f codex $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/codex
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < codex.1 > $(DESTDIR)$(MANPREFIX)/man1/codex.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/codex.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/codex\
		$(DESTDIR)$(MANPREFIX)/man1/codex.1


# Some tests

help: all
	./codex --help

lotr: all
	cat ./tests/lotr | ./codex --csv > output

divina: all
	cat ./tests/divina | ./codex --csv -o output -p

one: all
	cat ./tests/test_1 | ./codex --csv -p > output

generate: divina
	cat output | ./codex --text -n 0 > text

generateX: lotr
	cat output | ./codex --text -n 10000 > text

generateY: lotr
	cat output | ./codex --text -n 100000 -p > text

generateZ: one
	cat output | ./codex --text -n 1000000 > text

generateE: one
	cat output | ./codex --text -n 100000000 > text
