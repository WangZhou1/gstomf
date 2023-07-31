#include "omf_api.h"
#include "omfcstream.h"

EXTERNC void* OmfYuvSrcCreate();
EXTERNC BOOL  OmfYuvSrcDestory(void* hd);
EXTERNC BOOL  OmfYuvSrcStatusUp(void* hd, OmfStateC state);
EXTERNC BOOL  OmfYuvSrcStatusDown(void* hd, OmfStateC state);
EXTERNC BOOL  OmfYuvSrcGetFrame(void* hd, OmfFrameC_t* frame);
EXTERNC BOOL  OmfYuvSrcSelectSensor(void* hd, int id);
EXTERNC BOOL  OmfYuvSrcSetWidth(void* hd, int width);
EXTERNC BOOL  OmfYuvSrcSetHeight(void* hd, int height); 
EXTERNC BOOL  OmfYuvSrcSetInterlaced(void* hd, BOOL interlace);
EXTERNC const char* OmfYuvSrcGetMediaInfo(void* hd);