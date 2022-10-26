CC = gcc
# CFLAGS = -pg
EXE = pi.out
FILE = pi.c

# $(CC) $(CFLAGS) $(FILE) -o $(EXE)
all:
	$(CC) -o $(EXE) $(FILE) -lpthread

# %.o:%.cpp
# 	$(CC) -c $^ -o $@

.PHONY: clean
clean:
	rm $(EXE)
