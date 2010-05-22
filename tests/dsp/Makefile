CC = env CROSS_COMPILE=tic64x- llvmc
CFLAGS = -Wllc,-march,tms320c64x -hosttools -I../../dsp_include
LDFLAGS = --oformat=doff-c64x,-r

CFLAGS += -W -Wall -Wextra -Werror -ansi -pedantic

OBJS = start.doff retval.doff voidcall.doff
TESTS = ${OBJS:.doff=.xml}

.SUFFIXES: .c .doff .xml

.c.doff:
	$(CC) sr_test.o $< -o $@ $(CFLAGS) -Wl,$(LDFLAGS)

.c.xml:
	$(CC) $< -o $@ -E -DGENERATE_XML -Wp,-P

all: $(OBJS) tests.xml tgz

sr_test.o: sr_test.s
	tic64x-as sr_test.s -o sr_test.o

tests.xml: $(TESTS)
	echo "<testlist>" > tests.xml
	cat $(TESTS) >> tests.xml
	echo "</testlist>" >> tests.xml

tgz: $(OBJS) tests.xml
	tar -czf tests.tgz $(OBJS) tests.xml

clean:
	-rm *.doff
	-rm *.xml

.PHONEY: clean
