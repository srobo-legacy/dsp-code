CC=	arm-angstrom-linux-gnueabi-gcc

bees:
	echo "Build either dsp-stream.doff or mpu-stream"

dsp-stream.o: dsp-stream.c
	env CROSS_COMPILE=tic64x- llvmc -Wllc,-march,tms320c64x -hosttools -I../../dsp_include -I../../mpu_include -c dsp-stream.c -o dsp-stream.o

dsp-stream.doff: dsp-stream.o
	tic64x-ld dsp-stream.o -o dsp-stream.doff --oformat=doff-c64x -r

mpu-stream: mpu-stream.c
	$(CC) mpu-stream.c -o $@ -I../../mpu_include -L../../mpu_lib -lbridge
