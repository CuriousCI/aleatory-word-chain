CFLAGS := -Wall -Wextra

SRC_DIR := src

# clean:

# test

# TODO: run tests on files

run1:
	gcc ./src/*.c && cat ./tests/test_5 | ./a.out 1 > output

run2:
	gcc ./src/*.c && cat output | ./a.out 2 100000

run3:
	gcc ./src/*.c && cat ./tests/test_2 | ./a.out 1 | ./a.out 2 100000

bench:
	gcc ./src/*.c && ./a.out


test:
	gcc ./src/*.c && ./a.out > output

# benchmark with time, and create file with date
