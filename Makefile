CC = gcc
CFLAGS = -pg
EXE = pi.out
FILE = pi.c

all:
	$(CC) $(CFLAGS) $(FILE) -o $(EXE)

# %.o:%.cpp
# 	$(CC) -c $^ -o $@

.PHONY: clean
clean:
	rm $(EXE)
