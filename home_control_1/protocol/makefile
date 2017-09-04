CC = gcc
ARM_CC =/home/tt/david/work/share/A20_V3.0_MerriiLinux/out/dragonboard/common/buildroot/external-toolchain/bin/arm-linux-gnueabi-gcc 

TARGET = libleelenudp.a

INCLUDEPATH += -I ./core 

LIBS += -lpthread

SOURCES += ./core/*.c \
	./*.c 

OBJECTS += ./*.o \

all:
	$(CC) -c $(SOURCES) $(INCLUDEPATH) $(LIBS)
	ar crv $(TARGET) $(OBJECTS)

arm:
	$(ARM_CC) -c $(SOURCES) $(INCLUDEPATH) $(LIBS)
	ar rc $(TARGET) $(OBJECTS)

test:
	$(CC) -g -o test $(SOURCES) $(INCLUDEPATH) $(LIBS)

clean:
	rm $(TARGET)
	rm $(OBJECTS)
