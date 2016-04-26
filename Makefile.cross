CFLAGS = -Wall -fPIC
INCLUDES := -I./
LDFLAGS :=
LIBS :=

CROSS_COMPILE := aarch64-linux-gnu-
CC := $(CROSS_COMPILE)gcc

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

NAME := nx-drm-allocator
LIB_TARGET := lib$(NAME).so
APP_TARGET := test_$(NAME)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $^

$(LIB_TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -shared -Wl,-soname,$(TARGET) -o $@ $^ $(LIBS)

all: $(LIB_TARGET)

install: $(LIB_TARGET)
	cp nx-drm-allocator.h ../sysroot/include
	cp $^ ../sysroot/lib

.PHONY: clean

clean:
	rm -f *.o
	rm -f $(LIB_TARGET)
	rm -f $(APP_TARGET)
