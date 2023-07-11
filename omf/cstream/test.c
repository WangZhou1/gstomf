#include "omfcstream.h"
#include "OmfPcmSrc.h"


/*

/opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/bin/arm-openwrt-linux-gcc -I. -I../include -I../include/streaming \
-I../include/core -ldl -L./ -L../lib/ -L/opt/openwrt_toolchain/arm-openwrt-icatchtek-bsp4-glibc/lib/ -lgstomfcstream -lipc_core \
-lpluginpcm -lomfapicxx -lomfapi -lomfapistreaming -lpluginapistreaming -lplugindev -lpluginshm -lpluginshmbase -lpluginbase -lfwbase -lfwcore -lfwporting test.c -o test


*/

int main(void){
	void* hd = OmfPcmSrcCreate();
	OmfPcmSrcStatusUp(hd, OMF_STATE_READY);
	OmfPcmSrcStatusUp(hd, OMF_STATE_PLAYING);

	int cnt = 100;
	while(cnt--){
		OmfPcmSrcGetFrame(hd);
		usleep(10);
	}
	
	
	OmfPcmSrcStatusDown(hd, OMF_STATE_READY);
	OmfPcmSrcStatusDown(hd, OMF_STATE_NULL);
	
	OmfPcmSrcDestory(hd);
	
	return 0;
}
