EE_BIN = pong.elf
EE_OBJS = main.o game.o tools/pad.o tools/render.o
EE_LIBS = -lgskit -ldmakit -lpad -ldebug
EE_CFLAGS = -I. -I./tools -I$(PS2DEV)/gsKit/include
EE_LDFLAGS = -L$(PS2DEV)/gsKit/lib

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
