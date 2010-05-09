CC = env CROSS_COMPILE=tic64x- llvmc
CFLAGS = -Wllc,-march,tms320c64x -hosttools -I../../dsp_include
LDFLAGS = --oformat=doff-c64x

CFLAGS += -W -Wall -Wextra -Werror -ansi -pedantic

OBJS = start.doff
TESTS = ${OBJS:.doff=.xml}

.SUFFIXES: .c .doff .xml

.c.doff:
	$(CC) $< -o $@ $(CFLAGS) -Wl,$(LDFLAGS)

.c.xml:
	$(CC) $< -o $@ -E -Wp,-P

all: $(OBJS) tests.xml

tests.xml: $(TESTS)
	echo "<testlist>" | cat $(TESTS) | echo "</testlist>" > test.xml

clean:
	rm *.doff

.PHONEY: clean
