CC=	arm-angstrom-linux-gnueabi-gcc

bees:
	echo "Build either dsp-stream.doff or mpu-stream"

dsp-stream.bc: dsp-stream.c
	llvm-gcc -x c -O2 -I../../dsp_include -I../../mpu_include dsp-stream.c -c -emit-llvm -o dsp-stream.bc

dsp-stream.s: dsp-stream.bc
	llc -f -O2 -march tms320c64x dsp-stream.bc -o dsp-stream.s

dsp-stream.o: dsp-stream.c
	env CROSS_COMPILE=tic64x- llvmc -Wllc,-march,tms320c64x -hosttools -I../../dsp_include -I../../mpu_include -c dsp-stream.c -o dsp-stream.o

dsp-stream.doff: dsp-stream.o
	tic64x-ld dsp-stream.o -o dsp-stream.doff --oformat=doff-c64x -r

mpu-stream: mpu-stream.c
	$(CC) mpu-stream.c -o $@ -I../../mpu_include -L../../mpu_lib -lbridge

clean:
	-rm dsp-stream.bc dsp-stream.s dsp-stream.o dsp-stream.doff

.PHONY: clean
