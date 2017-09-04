CC = arm-linux-gnueabi-gcc

CFLAGS = -I../include \
	-I../include/include_system \
	-I../include/include_vdecoder \
	-I../include/include_platform/CHIP_A20/disp \


LFLAGS = -L../lib/A20 \
	-lvecore -lcdxalloc -lvdecoder -lvencoder

all:
	$(CC) $(CFLAGS) render.c test_tvdecoder.cpp $(LFLAGS) -o test_tvdecoder
	$(CC) $(CFLAGS) render.c test_tvdecoder0.cpp $(LFLAGS) -o test_tvdecoder0
clean:
	rm test_tvdecoder test_tvdecoder2
