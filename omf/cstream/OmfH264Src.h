#include "omf_api.h"
#include "omfcstream.h"

EXTERNC void* OmfH264SrcCreate();
EXTERNC BOOL  OmfH264SrcDestory(void* hd);
EXTERNC BOOL  OmfH264SrcStatusUp(void* hd, OmfStateC state);
EXTERNC BOOL  OmfH264SrcStatusDown(void* hd, OmfStateC state);
EXTERNC BOOL  OmfH264SrcGetFrame(void* hd, OmfFrameC_t* frame);
EXTERNC BOOL  OmfH264SrcSelectSensor(void* hd, int id);
EXTERNC BOOL  OmfH264SrcSetWidth(void* hd, int width);
EXTERNC BOOL  OmfH264SrcSetHeight(void* hd, int height);
EXTERNC BOOL  OmfH264SrcSetGop(void* hd, int gop);
EXTERNC BOOL  OmfH264SrcSetGopType(void* hd, int goptype);
EXTERNC BOOL  OmfH264SrcSetFrameRate(void* hd, int fr);
EXTERNC BOOL  OmfH264SrcSetBitRate(void* hd, int br);
EXTERNC BOOL  OmfH264SrcSetCodec(void* hd, const char* codec);
EXTERNC BOOL  OmfH264SrcSetPreRecord(void* hd, int group);
EXTERNC BOOL  OmfH264SrcSetPreRecordPipe(void* hd, const char* pipename);
EXTERNC BOOL  OmfH264SrcSetSharedEncoder(void* hd, int group);
EXTERNC BOOL  OmfH264SrcSetCache(void* hd, unsigned cache);
EXTERNC BOOL  OmfH264SrcSetLowBandWidth(void* hd, BOOL en);
EXTERNC BOOL  OmfH264SrcGetMediaInfo(void* hd, const char** pmedia);

