CC = arm-linux-gnueabi-gcc
CPP = arm-linux-gnueabi-g++
STRIP = arm-linux-gnueabi-strip
AR = arm-linux-gnueabi-ar
CEDARX_CHIP_VERSION = A20

TARGET=encodeDemo2
BUILDPATH=.

SRCDIRS:=.\
		Camera \
		render
			
CFLAGS =-Wall -O0 -g -ldl -march=armv7-a -mtune=cortex-a8 -mfpu=neon -ftree-vectorize -ffast-math -mfloat-abi=softfp 

INCLUDES:=$(foreach dir,$(SRCDIRS),-I$(dir)) \
	-I../include \
	-I../lib \
	-I../include/include_system \
	-I../include/include_vencoder \
	-I../include/include_vdecoder \
	-I../include/include_camera \
	-I../include/include_platform/CHIP_$(CEDARX_CHIP_VERSION)/disp \
	-I../include/include_platform/CHIP_$(CEDARX_CHIP_VERSION)

SRCCS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))

LIBOBJ=$(addprefix $(BUILDPATH)/, $(addsuffix .o, $(basename $(SRCCS)))) 

LDFLAGS=-lpthread -lvdecoder -lvecore -lvencoder -ldl -L../lib/$(CEDARX_CHIP_VERSION)

all: $(TARGET)

$(BUILDPATH)/%.o:%.c
	$(CC) $(CFLAGS) ${INCLUDES} -o $@ -c $<
		    
$(TARGET):$(LIBOBJ)
	$(CPP) $(LDFLAGS) -o $@ $^ 

clean:
	rm -f $(TARGET) $(LIBOBJ)

