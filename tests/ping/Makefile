CC=	arm-angstrom-linux-gnueabi-gcc
DSP_CC=	clang -ccc-host-triple tms320c64x-unknown-unknown

all: ping.doff ping

dsp.o: dsp.c
	$(DSP_CC) -I../../dsp_include -I../../mpu_include -c dsp.c -o dsp.o

ping.doff: dsp.o
	tic64x-ld dsp.o -o ping.doff --oformat=doff-c64x -r

ping: ping.c drive_dsp.c
	$(CC) ping.c drive_dsp.c -o $@ -I../../mpu_include -L../../mpu_lib -lbridge

clean:
	-rm dsp.o ping.doff ping

.PHONY: clean
