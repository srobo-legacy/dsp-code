CC = clang -ccc-host-triple tms320c64x-unknown-unknown
AS = tic64x-as
LD = tic64x-ld
AR = tic64x-ar
RANLIB = tic64x-ranlib

CFLAGS = -I../../dsp_include -I../../mpu_include

srhacks.a: srhacks.o
	$(AR) -c -r srhacks.a srhacks.o
	cp srhacks.a ..

srhacks.o: srhacks.c trap.o asm_utils.o isr.o
	$(CC) $(CFLAGS) srhacks.c trap.o asm_utils.o isr.o -o srhacks.o

trap.o: trap.c
	$(CC) $(CFLAGS) trap.c -c -o trap.o

asm_utils.o: asm_utils.s
	$(AS) asm_utils.s -o asm_utils.o

isr.o: isr.s
	$(AS) isr.s -o isr.o

clean:
	-rm *.o
	-rm srhacks.a

.PHONY: clean
