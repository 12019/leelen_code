CC = arm-none-linux-gnueabi-gcc
CPP = arm-none-linux-gnueabi-g++
STRIP = arm-none-linux-gnueabi-strip
AR = arm-none-linux-gnueabi-ar
CEDARX_CHIP_VERSION = A20

TARGET=decodeDemo
BUILDPATH=.

SRCDIRS:=. 
			
CFLAGS =-Wall -O0 -g -ldl

DATA_SRC=file

ifeq ($(DATA_SRC), file)
SRCDIRS+=./file_parser
CFLAGS+=-DFILE_PARSER 
else
SRCDIRS+=./stream_parser 
CFLAGS+=-DSTREAM_PARSER 
endif
			  
INCLUDES:=$(foreach dir,$(SRCDIRS),-I$(dir)) \
	-I../include \
	-I../include/include_system \
	-I../include/include_vdecoder \
	-I../include/include_platform/CHIP_$(CEDARX_CHIP_VERSION) \
	-I../include/include_platform/CHIP_$(CEDARX_CHIP_VERSION)/disp

SRCCS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))

LIBOBJ=$(addprefix $(BUILDPATH)/, $(addsuffix .o, $(basename $(SRCCS)))) 

LDFLAGS=-lpthread -lvdecoder -lvecore -ldl -L../lib/$(CEDARX_CHIP_VERSION)

all: $(TARGET)

$(BUILDPATH)/%.o:%.c
	$(CC) $(CFLAGS) ${INCLUDES} -o $@ -c $<
		    
$(TARGET):$(LIBOBJ)
	$(CPP) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET) $(LIBOBJ)

