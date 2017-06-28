PROG = Iterative Recursive

CC = g++
LD = g++
CFLAGS = -g -Wall


all: $(PROG)

%: %.o
	$(LD)  -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o $(PROG)
	
