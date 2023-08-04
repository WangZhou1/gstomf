#include "omf_api.h"
#include "omfcstream.h"

EXTERNC void* OmfH265SrcCreate();
EXTERNC BOOL  OmfH265SrcDestory(void* hd);
EXTERNC BOOL  OmfH265SrcStatusUp(void* hd, OmfStateC state);
EXTERNC BOOL  OmfH265SrcStatusDown(void* hd, OmfStateC state);
EXTERNC BOOL  OmfH265SrcGetFrame(void* hd, OmfFrameC_t* frame);
EXTERNC BOOL  OmfH265SrcSelectSensor(void* hd, int id);
EXTERNC BOOL  OmfH265SrcSetWidth(void* hd, int width);
EXTERNC BOOL  OmfH265SrcSetHeight(void* hd, int height);
EXTERNC BOOL  OmfH265SrcSetGop(void* hd, int gop);
EXTERNC BOOL  OmfH265SrcSetGopType(void* hd, int goptype);
EXTERNC BOOL  OmfH265SrcSetFrameRate(void* hd, int fr);
EXTERNC BOOL  OmfH265SrcSetBitRate(void* hd, int br);
EXTERNC BOOL  OmfH265SrcSetCodec(void* hd, const char* codec);
EXTERNC BOOL  OmfH265SrcSetPreRecord(void* hd, int group);
EXTERNC BOOL  OmfH265SrcSetPreRecordPipe(void* hd, const char* pipename);
EXTERNC BOOL  OmfH265SrcSetSharedEncoder(void* hd, int group);
EXTERNC BOOL  OmfH265SrcSetCache(void* hd, unsigned cache);
EXTERNC BOOL  OmfH265SrcSetLowBandWidth(void* hd, BOOL en);
EXTERNC BOOL  OmfH265SrcGetMediaInfo(void* hd, const char** pmedia);

