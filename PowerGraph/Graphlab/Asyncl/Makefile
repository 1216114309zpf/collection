PROG = launcher vertex
CC = g++
LD = g++
CFLAGS = -g -Wall


OBJS = launcher.o vertex.o

all : $(PROG)

$(PROG): $(OBJS)
	$(LD) -o $@ $<             #here it must be $< or $^ instead of $(OBJS)


%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o $(PROG)
	
