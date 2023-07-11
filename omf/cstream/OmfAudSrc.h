#include "omf_api.h"
#include "omfcstream.h"

EXTERNC void* OmfAudSrcCreate();
EXTERNC BOOL OmfAudSrcDestory(void* hd);
EXTERNC BOOL OmfAudSrcStatusUp(void* hd, OmfStateC state);
EXTERNC BOOL OmfAudSrcStatusDown(void* hd, OmfStateC state);
EXTERNC BOOL OmfAudSrcGetFrame(void* hd, OmfFrameC_t* frame);
EXTERNC BOOL OmfAudSrcSetMicSamples(void* hd, unsigned int samples);
EXTERNC BOOL OmfAudSrcSetSampleRate(void* hd, unsigned int samplerate);
EXTERNC BOOL OmfAudSrcSetChannel(void* hd, unsigned int channel);
EXTERNC BOOL OmfAudSrcSetCodec(void* hd, const char* codec);
EXTERNC BOOL OmfAudSrcSetAEC(void* hd, const char* para);
EXTERNC BOOL OmfAudSrcSetANS(void* hd, const char* para);
EXTERNC BOOL OmfAudSrcSetALC(void* hd, const char* para);
EXTERNC BOOL OmfAudSrcSetPreRecord(void* hd, int group);
EXTERNC BOOL OmfAudSrcSetPreRecordPipe(void* hd, const char* pipename);
EXTERNC BOOL OmfAudSrcSetSharedEncoder(void* hd, int group);
EXTERNC BOOL OmfAudSrcSetCache(void* hd, unsigned cache);
