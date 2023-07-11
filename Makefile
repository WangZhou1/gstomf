#PATH := ./elements

SRC := $(wildcard ./elements/*.c)
OBJS := $(patsubst %.c, %.o, $(SRC))
TARGET := libgstomfelements.so

CC = /opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/bin/arm-openwrt-linux-gcc

CFLAGS := 
CFLAGS += -shared
CFLAGS += -fPIC
CFLAGS += -mcpu=cortex-a7
CFLAGS += -DHAVE_CONFIG_H

CFLAGS += -I. 
CFLAGS += -I./gst/include/ 
CFLAGS += -I/home/wang.zhou/project/v57/gds/ES2/bsp4_glibc/staging_dir/target-arm-linux_glibc/usr/include/gstreamer-1.0/
CFLAGS += -I/home/wang.zhou/project/v57/gds/ES2/bsp4_glibc/staging_dir/target-arm-linux_glibc/usr/include/glib-2.0/

CFLAGS += -I./omf/include/ 
CFLAGS += -I./omf/include/streaming/ 
CFLAGS += -I./omf/cstream/ 

LDFLAGS :=
LDFLAGS += -L./gst/lib/
LDFLAGS += -L./omf/lib/
LDFLAGS += -L./omf/cstream/

LFLAGS :=
LFLAGS += -lgstreamer-1.0 -lgstbase-1.0 -lglib-2.0 -lgobject-2.0 -lffi -lgmodule-2.0
LFLAGS += -lgstomfcstream

$(TARGET):$(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LFLAGS) $(SRC) -o $(TARGET)

.PHONY:clean

clean:
	rm $(obj) -f