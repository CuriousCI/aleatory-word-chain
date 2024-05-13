CFLAGS := -Wall -Wextra

SRC_DIR := src

SHELL := /bin/zsh

# codex -h
# codex --help

# codex -c
# codex --csv 

# codex -c -p
# codex --csv --parallel

# codex -g 10
# codex --generate 10
# codex -g 10 -w hello
# codex --generate 10 --word hello

# codex -g 10 -w hello -p
# codex --generate 10 --word hello --parallel


build:
	gcc ./src/*.c -o codex

help:
	gcc ./src/*.c -o codex && ./codex --help

lotr:
	gcc ./src/*.c -o codex && time cat ./tests/lotr | ./codex --csv > output

divina:
	gcc ./src/*.c -o codex && time cat ./tests/test_5 | ./codex --csv > output

generate:
	gcc ./src/*.c -o codex && time cat output | ./codex --generate 0 > text

generateX:
	gcc ./src/*.c -o codex && time cat output | ./codex --generate 10000 > text

generateY:
	gcc ./src/*.c -o codex && time cat output | ./codex --generate 100000 > text

generateZ:
	gcc ./src/*.c -o codex && time cat output | ./codex --generate 1000000 > text

generateE:
	gcc ./src/*.c -o codex && time cat output | ./codex --generate 100000000 > text
