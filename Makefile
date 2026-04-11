EE_BIN = pong.elf
EE_OBJS = main.o
# EE_LIBS = -lgraph
EE_LIBS = -lgskit -ldmakit
EE_CFLAGS = -I. -I/usr/local/ps2dev/gsKit/include
EE_LDFLAGS = -L/usr/local/ps2dev/gsKit/lib

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
