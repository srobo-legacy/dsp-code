CC = env CROSS_COMPILE=tic64x- llvmc
CFLAGS = -Wllc,-march,tms320c64x -hosttools -I../../dsp_include
LDFLAGS = --oformat=doff-c64x

CFLAGS += -W -Wall -Wextra -Werror -ansi -pedantic

OBJS = start.doff

.SUFFIXES: .c .doff

.c.doff:
	$(CC) $< -o $@ $(CFLAGS) -Wl,$(LDFLAGS)

all: $(OBJS)

clean:
	rm *.doff

.PHONEY: clean
