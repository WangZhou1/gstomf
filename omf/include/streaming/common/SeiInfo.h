//
// Created by jg.wang on 2019/7/4.
//
#pragma once

#include <string>
namespace omf {
	namespace api {
		namespace streaming {
			namespace common {
				typedef struct{
					void* ppayload;
					uint32 status;
					uint8 nalUnitType;/* h265 need set, h264 don't care */
				} seiObj_t;

				typedef struct{
					uint32 streamId;
					uint32 frameNum;
					uint32 sliceType;
					uint32 bufferMax;
					uint32 bufferUsed;
					struct timeval framePts;
				} seiInfo_t;

				typedef struct{
					uint32 objNum;
					seiObj_t **ppobj;
					uint32 (*pfn)(const seiInfo_t*,seiObj_t*);
					seiInfo_t info; /*information for host*/
				} seiCb_t;

			}
			/* h264 customer nal sei - callback status */
			typedef enum {
				H264_SEI_CB_STATUS_ADD_TO_STREAM			= (1<<0),
				H264_SEI_CB_STATUS_BLOCK_UPD_CALLBACK	= (1<<1),
				H264_SEI_CB_STATUS_BLOCK_STREAM_ID_0		= (1<<2),
				H264_SEI_CB_STATUS_BLOCK_STREAM_ID_1		= (1<<3),
				H264_SEI_CB_STATUS_BLOCK_SLICE_P	 		= (1<<4),
				H264_SEI_CB_STATUS_BLOCK_SLICE_B	 		= (1<<5),
				H264_SEI_CB_STATUS_BLOCK_SLICE_I			= (1<<6),
			} H264SeiCallbackStatus_t;

			//bool SeiInfoCbInit();

		}
	}
}
bool SeiInfoCbInit();