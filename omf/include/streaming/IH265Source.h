//
// Created by jg.wang on 2019/6/28.
//

#pragma once
#include "common/IEncoder.h"
#include "common/IVideoSource.h"
#include "common/IH265.h"
namespace omf {
	namespace api {
		namespace streaming {
			/**
			 *@example _demoH264Source.cpp
			 */
			/**
			 * @brief provide h264 date source.
			 * You can get H264 realtime streaming from the interface.\n
			 * Firstly,create an instance.\n
			 * 	CreateNew(const char *keywords).\n
			 * Secondly,set the parameters.\n
			 * 	Video source: \n
			 *  	SelectSensor(int id) \n
			 *  	SetWidth(int w) \n
			 *  	SetHeight(int h) \n
			 * 	H264 encoder: \n
			 * 		SetGop(int gop)\n
			 * 		SetGopType(const char* type)\n
			 * 	BitRate Control:\n
			 * 	FrameRate Control:\n
			 * Thirdly,	register streaming callbck.\n
			 * 	RegisterMessageCallback(const FuncMessage &func)\n
			 * 	RegisterOutputCallback(const IStreamEntry::FuncFrame &func)\n
			 * Finally, operator streaming status.\n
			 * 	Open()\n
			 * 	Close()\n
			 * 	Start()\n
			 * 	Stop()\n
			 * Moreover, get same information from the instance.\n
			 *  GetH264MediaInfo(). call after Open().\n
			 *  GetCurrentFrameRate(). call after Start().\n
			 *  GetCurrentBitRate(). call after Start().\n
			 *  GetCurrentQP(). call after Start().\n
			 * Others,\n
			 *  ForceKeyFrame(). call after start().\n
			 */
			class IH265Source
				: virtual public common::IVideoSource
				, virtual public common::IEncoder
				, virtual public common::IH265
			{
			public:
				using H265MediaInfo=omf::api::streaming::common::H265MediaInfo;
			public:
				/**
				 * get the output H265 media info.
				 * @return H265 media info.
				 * @see H265MediaInfo
				 */
				virtual const H265MediaInfo& GetH265MediaInfo() =0;
			public:
				/**
				 * create a new H265Souce instance.
				 * @param keywords[in] the keywords for selecting a H265Souce type.
				 * @return the new H265Souce instance.
				 */
				static IH265Source* CreateNew(const char *keywords);
				static IH265Source* CreateNewFromConfig(const char *config);
				static IH265Source* CreateNewFromFile(const char *file);
			};
		}
	}
}




