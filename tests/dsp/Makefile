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
	$(CC) $< -o $@ -E -DGENERATE_XML -Wp,-P

all: $(OBJS) tests.xml tgz

tests.xml: $(TESTS)
	echo "<testlist>" > tests.xml
	cat $(TESTS) >> tests.xml
	echo "</testlist>" >> tests.xml

tgz: $(OBJS) tests.xml
	tar -czf tests.tgz tests.xml

clean:
	rm *.doff
	rm *.xml

.PHONEY: clean
