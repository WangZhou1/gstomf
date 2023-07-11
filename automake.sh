#!/bin/bash

cd ./omf/cstream/ && make && cd -
make
cp *.so ~/project/v57/gds/ES2/bsp4_glibc/target/linux/icatchtek/v57/devices/icatch/rootfs/usr/lib/gstreamer-1.0/
cp ./omf/cstream/*.so ~/project/v57/gds/ES2/bsp4_glibc/target/linux/icatchtek/v57/devices/icatch/rootfs/usr/lib/ 

#cd ./omf/cstream/ 

#/opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/bin/arm-openwrt-linux-gcc -I. -I../include -I../include/streaming \
#-I../include/core -ldl -L./ -L../lib/ -L/opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/lib/ -lgstomfcstream -lipc_core \
#-lpluginpcm -lomfapicxx -lomfapi -lomfapistreaming -lpluginapistreaming -lplugindev -lpluginshm -lpluginshmbase -lpluginbase -lfwbase -lfwcore -lfwporting test.c -o test

#cp test ~/project/v57/gds/ES2/bsp4_glibc/target/linux/icatchtek/v57/devices/icatch/rootfs/usr/lib/
#cd -


