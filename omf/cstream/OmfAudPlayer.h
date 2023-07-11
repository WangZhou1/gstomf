#include "omf_api.h"
#include "omfcstream.h"

EXTERNC void* OmfAudPlayerCreate();
EXTERNC BOOL OmfAudPlayerDestory(void* hd);
EXTERNC BOOL OmfAudPlayerStatusUp(void* hd, OmfStateC state);
EXTERNC BOOL OmfAudPlayerStatusDown(void* hd, OmfStateC state);
EXTERNC BOOL OmfAudPlayerPushFrame(void* hd, OmfFrameC_t* frame);
EXTERNC BOOL OmfAudPlayerSetSampleRate(void* hd, unsigned int samplerate);
EXTERNC BOOL OmfAudPlayerSetChannel(void* hd, unsigned int channel);
EXTERNC BOOL OmfAudPlayerSetMediaInfo(void* hd, const char* media);
EXTERNC BOOL OmfAudPlayerSetCodecOnLinux(void* hd, BOOL en);
EXTERNC BOOL OmfAudPlayerSetLiveDelayLimit(void* hd, const char* limit);

