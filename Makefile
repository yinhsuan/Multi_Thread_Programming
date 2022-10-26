CC = g++
CFLAGS = -pg
EXE = pi.out
FILE = pi.cpp

all:
	$(CC) $(CFLAGS) $(FILE) -o $(EXE)

# %.o:%.cpp
# 	$(CC) -c $^ -o $@

.PHONY: clean
clean:
	rm $(EXE)
