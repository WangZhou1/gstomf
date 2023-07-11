#include "omf_api.h"
#include "omfcstream.h"

EXTERNC void* OmfPcmSrcCreate();
EXTERNC BOOL OmfPcmSrcDestory(void* hd);
EXTERNC BOOL OmfPcmSrcStatusUp(void* hd, OmfStateC state);
EXTERNC BOOL OmfPcmSrcStatusDown(void* hd, OmfStateC state);
EXTERNC BOOL OmfPcmSrcGetFrame(void* hd, OmfFrameC_t* frame);
EXTERNC BOOL OmfPcmSrcSetSampleRate(void* hd, unsigned int samplerate);
EXTERNC BOOL OmfPcmSrcSetChannel(void* hd, unsigned int channel);
EXTERNC BOOL OmfPcmSrcSetAEC(void* hd, const char* para);
EXTERNC BOOL OmfPcmSrcSetANS(void* hd, const char* para);
EXTERNC BOOL OmfPcmSrcSetPreRecord(void* hd, unsigned group);
EXTERNC BOOL OmfPcmSrcSetPreRecordPipe(void* hd, const char* pipename);
EXTERNC BOOL OmfPcmSrcSetCache(void* hd, unsigned cache);
