CC = arm-angstrom-linux-gnueabi-gcc
CFLAGS += -O0 -ggdb -I ../../mpu_include $(shell xml2-config --cflags)
LDFLAGS += -L ../../mpu_lib -lbridge $(shell xml2-config --libs)

SOURCES = dsptest.c dspman.c run.c
HEADERS = dsptest.h

dsptest: $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) -o $@ $(CFLAGS) $(LDFLAGS)
