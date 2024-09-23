CC := gcc
# CFLAGS := -ansi -Wall -Wextra -Wpedantic -Wunused-result -Wunused-value
SRC := src

VERSION := 1.0 

# Paths

PREFIX := /usr/local
MANPREFIX := $(PREFIX)/share/man

all: awc

awc : $(SRC)/main.o $(SRC)/awc/util.o $(SRC)/awc/csv.o $(SRC)/awc/text.o $(SRC)/collections/map.o $(SRC)/collections/vec.o
	$(CC) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -f $(SRC)/*.o $(SRC)/collections/*.o $(SRC)/awc/*.o awc

# DESTDIR can be specified if you don't want to use the default install location

install: all 
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f awc $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/awc
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < awc.1 > $(DESTDIR)$(MANPREFIX)/man1/awc.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/awc.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/awc\
		$(DESTDIR)$(MANPREFIX)/man1/awc.1
