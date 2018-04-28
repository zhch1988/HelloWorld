#EXTRA_CFLAGS+=-I../include
INCDIR=../include

ifeq ($(debug),y)
DEBUGFLAG=-g
OBJS=main.debug.o
all:test_debug
else
OBJS=main.o
all:test
endif

.PHONY:all debug
test:$(OBJS)
	gcc -o $@ $^
test_debug:$(OBJS)
	gcc -o $@ $^
ifeq ($(debug),y)
%.debug.o:%.c
else
%.o:%.c
endif
	gcc $(DEBUGFLAG) -I$(INCDIR) -c $^ -o $@
clean:
	rm -f *.o test test_debug
