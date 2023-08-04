//
// Created by jg.wang on 2019/6/28.
//

#pragma once
#include "common/IEncoder.h"
#include "common/IVideoSource.h"
#include "common/IH264.h"
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
			class IH264Source
				: virtual public common::IVideoSource
				, virtual public common::IEncoder
				, virtual public common::IH264
			{
			public:
				using H264MediaInfo=omf::api::streaming::common::H264MediaInfo;
			public:
				/**
				 * get the output H264 media info.
				 * @return H264 media info.
				 * @see H264MediaInfo
				 */
				virtual const H264MediaInfo& GetH264MediaInfo()=0;
			public:
				/**
				 * create a new H264Souce instance.
				 * @param keywords[in] the keywords for selecting a H264Souce type.
				 * @return the new H264Souce instance.
				 */
				static IH264Source* CreateNew(const char *keywords);
				static IH264Source* CreateNewFromConfig(const char *config);
				static IH264Source* CreateNewFromFile(const char *file);

			public:
				/**
				 * Set the filter name of which is to process privacy zone.
				 * @param filterName.
				 */
				virtual bool SetPrivacyZoneFilter(const std::string& filterName, void* privateData=nullptr)=0;
				/**
				 * Set the filter name of which is to process Dori.
				 * @param filterName.
				 */
				virtual bool SetDoriFilter(const std::string& filterName, void* privateData=nullptr)=0;
				/**
				 * Set the low bandwidth enable.
				 * @param enable.
				 */
				virtual bool SetLowBandWidthEnable(bool en)=0;
			};
		}
	}
}




