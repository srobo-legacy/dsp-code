CC = env CROSS_COMPILE=tic64x- llvmc
CFLAGS = -Wllc,-march,tms320c64x -hosttools -I../../dsp_include
LDFLAGS = --oformat=doff-c64x -r

CFLAGS += -W -Wall -Wextra -Werror -ansi -pedantic -Wl,-r

OBJS = start.doff retval.doff voidcall.doff argcall.doff \
	add.doff sub.doff mul.doff shr.doff shl.doff shru.doff \
	and.doff or.doff xor.doff \
	addi.doff subi.doff andi.doff ori.doff xori.doff \
	cmplt.doff cmpgt.doff cmplte.doff cmpgte.doff cmpeq.doff cmpneq.doff \
	addi.doff printf.doff varprintf.doff

TESTS = ${OBJS:.doff=.xml}

.SUFFIXES: .c .doff .xml

.c.o:
	$(CC) $< -o $@ $(CFLAGS)

.s.o:
	$(CC) $< -o $@ $(CFLAGS)

.o.doff:
	tic64x-ld sr_test.o $< -o $@ $(LDFLAGS)

.c.xml:
	$(CC) $< -o $@ -E -DGENERATE_XML -Wp,-P

all: sr_test.o $(OBJS) tests.xml tgz

$(OBJS): sr_test.o

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
	-rm *.o
	-rm *.xml

.PHONEY: clean
