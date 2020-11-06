#----- CC and CFLAGS
CC = g++
# CPPFLAGS = -O3 -fno-stack-protector -pedantic -Wall -Werror
CPPFLAGS = -O3 -fno-stack-protector

LIB = -lm -lc -lrt -lpthread

UNITFILES =

#------------ EXECUTABLE FILES ---------------
all: rainfall_seq rainfall_pt

rainfall_seq:
	$(CC) $(CPPFLAGS) -o rainfall_seq rainfall_seq.cpp $(LIB)

rainfall_pt:
	$(CC) $(CPPFLAGS) -o rainfall_seq rainfall_seq.cpp $(LIB)

clean:
	rm -f rainfall_seq rainfall_pt *.o
