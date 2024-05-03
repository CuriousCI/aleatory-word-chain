CFLAGS := -Wall -Wextra

SRC_DIR := src

# clean:

# test

# TODO: run tests on files

# tree:
# 	gcc ./src/*.c && ./a.out 

tree:
	gcc ./src/*.c 

exec:
	gcc ./src/*.c

lotr:
	cat lotr | ./a.out 1 > output
# lotr:
# 	cat ./tests/lotr | ./a.out 1 > output

divcomm:
	cat ./tests/test_5 | ./a.out 1 > output

build:
	gcc ./src/*.c

test_2:
	cat ./tests/test_2 | ./a.out 1 > output

# test_2:
# 	gcc ./src/*.c && cat ./tests/test_2 | ./a.out 1 > output

run1:
	gcc ./src/*.c && cat ./tests/test_5 | ./a.out 1 > output

run2:
	gcc ./src/*.c && cat output | ./a.out 2 100000

run3:
	cat output | ./a.out 2 10000 > out
	# gcc ./src/*.c && cat ./tests/test_5 | ./a.out 1 | ./a.out 2 10000

run4:
	gcc ./src/*.c && cat ./tests/lotr | ./a.out 1 > output

run5:
	gcc ./src/*.c && cat ./tests/lotr | ./a.out 1 | ./a.out 2 100000 > out

buil:
	gcc ./src/*.c && cat ./tests/lotr | ./a.out 1 > output

geny:
	gcc ./src/*.c && cat output | ./a.out 2 1 > out

gen:
	gcc ./src/*.c -g && cat output | ./a.out 2 100000 > out

genx:
	gcc ./src/*.c && cat output | ./a.out 2 1000000 > out

bench:
	gcc ./src/*.c && ./a.out

# cargo build, cargo run, etc...

test:
	gcc ./src/*.c && ./a.out > output

# benchmark with time, and create file with date
