PROG = BottomUp  BottomUpWithSolution  TopDown

CC = g++
LD = g++
CFLAGS = -g -Wall

OBJS = BottomUp.o  TopDown.o BottomUpWithSolution

all: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LDLIBS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o $(PROG)
	
