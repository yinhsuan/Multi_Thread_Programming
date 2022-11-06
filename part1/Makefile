CXX=gcc
CFLAGS=-fPIC -std=c99 -g3 -O3 -I. -I${INC} -mavx2 -march=native -Wall -Wextra -pedantic -Wshadow
EXE=pi.out
FILE=pi.c

INC=./SIMDxorshift/include

OBJ= ./SIMDxorshift/src/simdxorshift128plus.o ./SIMDxorshift/src/xorshift128plus.o

all: ${EXE}

${EXE}: $(OBJ) $(FILE)
	${CXX} ${CFLAGS} $^ -o $@ -lpthread

%.o:%.c
	${CXX} ${CFLAGS} -c $^ -o $@

.PHONY: clean

clean:
	rm -rf ${EXE} *.out ${OBJ}