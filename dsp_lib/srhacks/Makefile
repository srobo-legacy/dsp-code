CC = clang -ccc-host-triple tms320c64x-unknown-unknown
AS = tic64x-as

CFLAGS = -I../dsp_include -I../mpu_include

srhacks.o: srhacks.c asm_utils.h
	$(CC) srhacks.c -c -o srhacks.o

asm_util.o: asm_util.s
	$(AS) asm_utils.s -c -o asm_utils.o

clean:
	rm *.o

.PHONY: clean
