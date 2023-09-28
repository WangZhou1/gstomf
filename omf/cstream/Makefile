
SRC := $(wildcard *.cc)
OBJS := $(patsubst %.cc, %.o, $(SRC))
TARGET := libgstomfcstream.so
STAGING_DIR := /home/wang.zhou/project/v57/gds/ES2/bsp4_glibc/staging_dir/

CC = /opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/bin/arm-openwrt-linux-gcc
CXX = /opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/bin/arm-openwrt-linux-g++

CFLAGS := 
CFLAGS += -shared -fPIC
CFLAGS += -Os -pipe -mcpu=cortex-a7 
#CFLAGS += -mfpu=neon-vfpv4 -fno-caller-saves -fno-plt
CFLAGS += -std=c++11 -Wreturn-type
CFLAGS += -znow -zrelro -Wl,--no-as-needed,--copy-dt-needed-entries
#CFLAGS += -mfloat-abi=hard  -Wformat -Werror=format-security -D_FORTIFY_SOURCE=1 -Wl,-z,now -Wl,-z,relro

CFLAGS += -I. 
CFLAGS += -I../include
CFLAGS += -I../include/streaming
CFLAGS += -I../include/core
CFLAGS += -I/opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/usr/include 
CFLAGS += -I/opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/include 

LDFLAGS :=
LDFLAGS += -L$(STAGING_DIR)/target-arm-linux_glibc/usr/lib/
LDFLAGS += -L/opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/lib/    
LDFLAGS += -L/opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/usr/lib/

LFLAGS :=
LFLAGS += -lipc_core -lpluginpcm -lomfapicxx -lomfapi -lomfapistreaming -lpluginapistreaming
LFLAGS += -lplugindev -lpluginshm -lpluginshmbase -lpluginbase -lfwbase -lfwcore -lfwporting
LFLAGS += -lpluginav
#LFLAGS += -lstdc++ -lpthread

$(TARGET):$(SRC)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(LFLAGS) $(SRC) -o $(TARGET)

.PHONY:clean

clean:
	rm $(obj) -f
