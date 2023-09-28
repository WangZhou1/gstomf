#ifndef __OMFCSTREAM_H__
#define __OMFCSTREAM_H__

#include <string.h>

#ifndef BOOL
#define BOOL char
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define dbgPrintf(s) printf("[%s(%d)]%s\n", __FUNCTION__, __LINE__, s);

#define returnIfC0(v) if((v)) return;
#define returnIfC1(v, c) if((c)){dbgPrintf(#c); return (v);}

#define OBJECT_CONVERT(hd, CLASS) ((CLASS*)(hd))

typedef enum {
  OMF_STATE_NULL                = 1,
  OMF_STATE_READY               = 2,
  //GST_STATE_PAUSED              = 3,
  OMF_STATE_PLAYING             = 4
} OmfStateC;

typedef struct OmfFrameC_s{
	int index;
	void*data;
	int size;
	int iskeyframe;
	long long pts_ns;
	void (*free)(void*free_data);
	//void*free_data;
	//void*pkt;
} OmfFrameC_t;

#endif