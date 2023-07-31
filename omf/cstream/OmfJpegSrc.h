#include "omf_api.h"
#include "omfcstream.h"

EXTERNC void* OmfJpegSrcCreate();
EXTERNC BOOL  OmfJpegSrcDestory(void* hd);
EXTERNC BOOL  OmfJpegSrcStatusUp(void* hd, OmfStateC state);
EXTERNC BOOL  OmfJpegSrcStatusDown(void* hd, OmfStateC state);
EXTERNC BOOL  OmfJpegSrcGetFrame(void* hd, OmfFrameC_t* frame);
EXTERNC BOOL  OmfJpegSrcSelectSensor(void* hd, int id);
EXTERNC BOOL  OmfJpegSrcSetWidth(void* hd, int width);
EXTERNC BOOL  OmfJpegSrcSetHeight(void* hd, int height);
EXTERNC BOOL  OmfJpegSrcSetQP(void* hd, int qp);
EXTERNC BOOL  OmfJpegSrcSetPreRecord(void* hd, unsigned group); 
EXTERNC const char* OmfJpegSrcGetMediaInfo(void* hd);