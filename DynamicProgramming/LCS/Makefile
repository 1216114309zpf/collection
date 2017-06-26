PROG = BottomUp  BottomUpWithSolution  TopDown

CC = g++
LD = g++
CFLAGS = -g -Wall

OBJS = BottomUp.o   BottomUpWithSolution.o TopDown.o

all: $(PROG)

%: %.o
	$(LD)  -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o $(PROG)
	
