CC := arm-linux-gnueabi-gcc
CPP := arm-linux-gnueabi-g++

CFLAGS := -Irender -I../include -I../include/include_vdecoder \
	-I../include/include_platform/CHIP_A20/disp -I../include/include_system
LFLAGS := -L../lib/A20 -lvdecoder -lvecore

all: test_render

test_render.o: test_render.cpp
	$(CPP) $(CFLAGS) test_render.cpp -c -o test_render.o

render.o: render/render.c
	$(CC) $(CFLAGS) render/render.c -c -o render.o

test_render: test_render.o render.o
	$(CPP) test_render.o render.o $(LFLAGS) -o test_render
	
clean:
	rm *.o
	rm test_render
