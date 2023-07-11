/**************************************************************************
 *
 *         Copyright (c) 2022 by iCatch Technology Co., Ltd.
 *
 *  This software is copyrighted by and is the property of Sunplus
 *  Technology Co., Ltd. All rights are reserved by Sunplus Technology
 *  Co., Ltd. This software may only be used in accordance with the
 *  corresponding license agreement. Any unauthorized use, duplication,
 *  distribution, or disclosure of this software is expressly forbidden.
 *
 *  This Copyright notice MUST not be removed or modified without prior
 *  written consent of Sunplus Technology Co., Ltd.
 *
 *  Sunplus Technology Co., Ltd. reserves the right to modify this
 *  software without notice.
 *
 *  Sunplus Technology Co., Ltd.
 *  19, Innovation First Road, Science-Based Industrial Park,
 *  Hsin-Chu, Taiwan, R.O.C.
 *
 *  Author: rf.wang
 *  Email:  rf.wang@sunmedia.com.cn
 *
 **************************************************************************/

#ifndef __OMF_LOGGER_API_H__
#define __OMF_LOGGER_API_H__

#if OMF_LOG
#define NADK_LOG 0
#else
#define NADK_LOG 1
#endif

#define NADK_LOG_LIB 1

#ifdef __cplusplus
extern "C" {
#endif	

struct OmfLoggerSetting {
	const char* application;
	const char* fileLogPath;
	int uartEnable;
	int fileEnable;
	int logLevel;
};

typedef enum omf_LogLevel
{
	OMF_LOG_LEVEL_VERB     = 0,
	OMF_LOG_LEVEL_DEBUG    = 1,
	OMF_LOG_LEVEL_INFO     = 2,
	OMF_LOG_LEVEL_WARN     = 3,
	OMF_LOG_LEVEL_ERROR    = 4,
	OMF_LOG_LEVEL_FATAL    = 5,
	OMF_LOG_LEVEL_TIMING   = 6,
} OMF_LogLevel;

void* OmfLogHandleGet();
void OmfLogHandleSet(void* handle,int level);
void OmfLogHandleClear();

void OmfLoggerConfig(struct OmfLoggerSetting* setting);
void OmfLoggerWrite(int level, const char* tag, const char* log);
void OmfLoggerLevelSet(int level);
int OmfLoggerLevelGet();
#ifdef __cplusplus
}
#endif	

#endif /* __OMF_LOGGER_API_H__ */
