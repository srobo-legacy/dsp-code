CC = clang -ccc-host-triple tms320c64x-unknown-unknown
AS = tic64x-as
LD = tic64x-ld
AR = tic64x-ar
RANLIB = tic64x-ranlib

CFLAGS = -I../../dsp_include -I../../mpu_include

sr_hacks.a: sr_hacks.o
	$(AR) -c -r sr_hacks.a sr_hacks.o

sr_hacks.o: sr_hacks.c trap.o asm_utils.o isr.o
	$(CC) $(CFLAGS) sr_hacks.c trap.o asm_utils.o isr.o -o sr_hacks.o -Wl,-r

trap.o: trap.c
	$(CC) $(CFLAGS) trap.c -c -o trap.o

asm_utils.o: asm_utils.s
	$(AS) asm_utils.s -o asm_utils.o

isr.o: isr.s
	$(AS) isr.s -o isr.o

clean:
	-rm *.o
	-rm sr_hacks.a

.PHONY: clean
