CC =  /opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/bin/arm-openwrt-linux-gcc

export STAGING_DIR =

ROOT_PATH = ../..

GST_PATH = ${ROOT_PATH}/gst

GSTSRC_PATH = ${ROOT_PATH}/gstreamer-1.12.4

GLIB_PATH = ${ROOT_PATH}/glib-2.58.1


CFLAGS ?= -O3 -g0 -std=c99 -I$(LVGL_DIR)/

CFLAGS += -I${GST_PATH}/include/ 

CFLAGS += -I${GSTSRC_PATH}/

CFLAGS += -I${GLIB_PATH}/

CFLAGS += -I${GLIB_PATH}/glib/

LDFLAGS ?= 

LDFLAGS += -L${ROOT_PATH}/ -L${GST_PATH}/lib -L${ROOT_PATH}/omf/cstream

LDFLAGS +=  -lffi -lglib-2.0 -lgmodule-2.0 -lgobject-2.0 \
			-lgstbase-1.0 -lgstreamer-1.0 

BIN = test_streaming_src

MAINSRC = test_streaming_src.c

OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))

COBJS = $(CSRCS:.c=$(OBJEXT))

MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)

OBJS = $(AOBJS) $(COBJS)

all: default

%.o: %.c

	@$(CC)  $(CFLAGS) -c $(INCLUDES) $< -o $@

default: $(AOBJS) $(COBJS) $(MAINOBJ)

	$(CC) -o $(BIN) $(MAINOBJ) $(AOBJS) $(COBJS) $(LDFLAGS)

clean:

	rm -f $(LVGL_OUTPUT_DIR)/*.o *.o

