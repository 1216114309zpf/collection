PROG = BottomUp  ExtendedBottomUP MemoizedTopDown ExtendedMemoizedTopDown  TopDown

CC = g++
LD = g++
CFLAGS = -g -Wall

OBJS = BottomUp  ExtendedBottomUP MemoizedTopDown ExtendedMemoizedTopDown  TopDown

all: $(PROG)

%: %.o
	$(LD)  -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o $(PROG)
	
