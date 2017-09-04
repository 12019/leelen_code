CC = arm-linux-gnueabi-gcc
CPP = arm-linux-gnueabi-g++
STRIP = arm-linux-gnueabi-strip
AR = arm-linux-gnueabi-ar
CEDARX_CHIP_VERSION = A20

TARGET=test_jpeg
BUILDPATH=.

SRCDIRS:=.
			
CFLAGS =-Wall -O0 -g -ldl 

INCLUDES:=$(foreach dir,$(SRCDIRS),-I$(dir)) \
        -I../linux-codec/include \
        -I../linux-codec/lib \
        -I../linux-codec/include/include_system \
        -I../linux-codec/include/include_vencoder \
        -I../linux-codec/include/include_camera \
        -I../linux-codec/include/include_platform/CHIP_$(CEDARX_CHIP_VERSION)/disp \
        -I../linux-codec/include/include_platform/CHIP_$(CEDARX_CHIP_VERSION)

SRCCS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))

LIBOBJ=$(addprefix $(BUILDPATH)/, $(addsuffix .o, $(basename $(SRCCS)))) 

LDFLAGS=-lpthread -lvencoder  -ldl -L../linux-codec/lib/$(CEDARX_CHIP_VERSION)

all: $(TARGET)

$(BUILDPATH)/%.o:%.c
	$(CC) $(CFLAGS) ${INCLUDES} -o $@ -c $<
		    
$(TARGET):$(LIBOBJ)
	$(CPP) $(LDFLAGS) -o $@ $^ 

clean:
	rm -f $(TARGET) $(LIBOBJ)

