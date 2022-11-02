CC = g++
EXE = pi.out
FILE = pi.c
INS = ./SIMDxorshift/src/xorshift128plus.c ./SIMDxorshift/src/simdxorshift128plus.c

# minimalist makefile
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h
ifeq ($(DEBUG),1)
CFLAGS = -fPIC  -std=c99 -ggdb -mavx2 -march=native -Wall -Wextra -pedantic -Wshadow -fsanitize=undefined  -fno-omit-frame-pointer -fsanitize=address
else
CFLAGS = -fPIC -std=c99 -O3 -mavx2  -march=native -Wall -Wextra -pedantic -Wshadow
endif # debug

HEADERS= SIMDxorshift/include/simdxorshift128plus.h  SIMDxorshift/include/xorshift128plus.h

OBJECTS= simdxorshift128plus.o xorshift128plus.o

# all: $(OBJECTS) fillarray shuffle

$(EXE): $(HEADERS)  $(OBJECTS) $(FILE)
	$(CC) $(INS) $(CFLAGS) -o $(EXE) $(FILE) $(OBJECTS) -Iinclude -flto

xorshift128plus.o: ./SIMDxorshift/src/xorshift128plus.c $(HEADERS)
	$(CC) $(CFLAGS) -c ./SIMDxorshift/src/xorshift128plus.c -Iinclude

simdxorshift128plus.o: ./SIMDxorshift/src/simdxorshift128plus.c $(HEADERS)
	$(CC) $(CFLAGS) -c ./SIMDxorshift/src/simdxorshift128plus.c -Iinclude



clean:
	rm -f $(OBJECTS) fillarray shuffle