CC=	arm-angstrom-linux-gnueabi-gcc
CFLAGS=	-I../../mpu_include -L../../mpu_lib
DSP_CC=	clang -ccc-host-triple tms320c64x-unknown-unknown
DSP_CFLAGS= -I../../dsp_include -I../../mpu_include

all: ping.doff ping

dsp.o: dsp.c
	$(DSP_CC) $(DSP_CFLAGS) -c dsp.c -o dsp.o

ping.doff: dsp.o
	tic64x-ld dsp.o -o ping.doff --oformat=doff-c64x -r

ping: ping.c drive_dsp.c
	$(CC) $(CFLAGS) ping.c drive_dsp.c -o $@ -lbridge

clean:
	-rm dsp.o ping.doff ping

.PHONY: clean
