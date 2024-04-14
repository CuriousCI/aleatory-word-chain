CFLAGS := -Wall -Wextra

SRC_DIR := src

# clean:

# test

run1:
	gcc ./src/*.c && ./a.out 1 > output

run2:
	gcc ./src/*.c && ./a.out 2

bench:
	gcc ./src/*.c && ./a.out


test:
	gcc ./src/*.c && ./a.out > output

# benchmark with time, and create file with date
