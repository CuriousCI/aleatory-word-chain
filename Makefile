# CFLAGS = -Wall -Wextra -ansi -Wpedantic
CFLAGS = -ansi -Wall -Wextra -Wpedantic -Wunused-result -Wunused-value
SHELL = /bin/zsh
SRC = ./src/*.c
OBJ = $(SRC:.c=.o)
EXEC = codex

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)

# %.o: %.c
#  $(CC) $(CFLAGS) -c $< -o $@
.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(OBJ) $(EXEC)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f dmenu dmenu_path dmenu_run stest $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/dmenu
	chmod 755 $(DESTDIR)$(PREFIX)/bin/dmenu_path
	chmod 755 $(DESTDIR)$(PREFIX)/bin/dmenu_run
	chmod 755 $(DESTDIR)$(PREFIX)/bin/stest
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < dmenu.1 > $(DESTDIR)$(MANPREFIX)/man1/dmenu.1
	sed "s/VERSION/$(VERSION)/g" < stest.1 > $(DESTDIR)$(MANPREFIX)/man1/stest.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/dmenu.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/stest.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/dmenu\
		$(DESTDIR)$(PREFIX)/bin/dmenu_path\
		$(DESTDIR)$(PREFIX)/bin/dmenu_run\
		$(DESTDIR)$(PREFIX)/bin/stest\
		$(DESTDIR)$(MANPREFIX)/man1/dmenu.1\
		$(DESTDIR)$(MANPREFIX)/man1/stest.1


# CC = gcc
# CFLAGS = -Wall -Wextra -g
# LDFLAGS =
#
# SRCS = main.c utils.c
# OBJS = $(SRCS:.c=.o)
# EXEC = myprogram
#
# .PHONY: all clean
#
# all: $(EXEC)
#
# $(EXEC): $(OBJS)
#  $(CC) $(LDFLAGS) $^ -o $@
#
# %.o: %.c
#  $(CC) $(CFLAGS) -c $< -o $@
#
# clean:
#  rm -f $(OBJS) $(EXEC)



build:
	gcc ./src/*.c -o codex $(CFLAGS)

help:
	gcc ./src/*.c -o codex $(CFLAGS) && ./codex --help

lotr:
	gcc ./src/*.c -o codex $(CFLAGS) && time cat ./tests/lotr | ./codex --csv > output

divina:
	gcc ./src/*.c -o codex $(CFLAGS) && time cat ./tests/test_5 | ./codex --csv -p -o output

one:
	gcc ./src/*.c -o codex $(CFLAGS) && time cat ./tests/test_1 | ./codex --csv -p > output

generate:
	gcc ./src/*.c -o codex $(CFLAGS) && time cat output | ./codex --text -n 0 > text

generateX:
	gcc ./src/*.c -o codex $(CFLAGS) && time cat output | ./codex --text -n 10000 > text

generateY:
	gcc ./src/*.c -o codex $(CFLAGS) && time cat output | ./codex --text -n 100000 -p > text

generateZ:
	gcc ./src/*.c -o codex $(CFLAGS) && time cat output | ./codex --text -n 1000000 > text

generateE:
	gcc ./src/*.c -o codex $(CFLAGS) && time cat output | ./codex --text -n 100000000 > text
