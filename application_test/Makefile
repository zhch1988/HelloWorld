OBJS=main.o
#EXTRA_CFLAGS+=-I../include
INCDIR=../include

#.PHONY:all

all: test
test:main.o
	gcc -o test $(OBJS)
main.o:main.c
	gcc -I$(INCDIR) -c main.c
clean:
	rm -f $(OBJS) test
