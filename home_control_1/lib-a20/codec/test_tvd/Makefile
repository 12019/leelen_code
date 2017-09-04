CC = arm-linux-gnueabi-gcc
CPP = arm-linux-gnueabi-g++

CFLAGS = -I../decDemo1/render -I../include -I../include/include_vdecoder -I../include/include_system \
	-I../include/include_platform/CHIP_A20/disp

LFLAGS = -L../lib/A20 -lvdecoder -lvecore

all: main.cpp
	$(CC) $(CFLAGS) test_h264decode.cpp  $(LFLAGS) -o test_h264decode
	#cp test_h264decode /mnt/hgfs/shared_folder
	
clean:
	rm test_h264decode
