/**************************************************************************
 *
 *         Copyright (c) 2014 by iCatch Technology Co., Ltd.
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
 *  Author: peng.tan
 *  Email:  peng.tan@sunmedia.com.cn
 *
 **************************************************************************/

#ifndef __NADK_LOG_HH__
#define __NADK_LOG_HH__

#include <stdio.h>
#include <stdlib.h>
#include "NADK_LogDef.h"

#ifdef __cplusplus
extern "C" {
#endif

NADK_LOG_API NADKBool NADKLog_initialize(
    const char* defaultDeviceID, const char* defaultApplication);
NADK_LOG_API NADKBool NADKLog_cleanup();

NADK_LOG_API NADKLogHandle NADKLog_create(const char* deviceID, const char* application);
NADK_LOG_API void NADKLog_delete(NADKLogHandle* logHandle);

NADK_LOG_API void NADKLog_setDebugMode(NADKLogHandle logHandle, NADKBool enable);
NADK_LOG_API void NADKLog_setCachingMode(NADKLogHandle logHandle, NADKBool enable);

NADK_LOG_API void NADKLog_setColor(NADKLogHandle logHandle, NADKBool enable);
NADK_LOG_API void NADKLog_setThreadInfo(NADKLogHandle logHandle, NADKBool enable);
NADK_LOG_API void NADKLog_setCachedInfo(NADKLogHandle logHandle, NADKBool enable);
NADK_LOG_API void NADKLog_setRelativeTime(NADKLogHandle logHandle, NADKBool enable);

NADK_LOG_API void NADKLog_setSystemLog(NADKLogHandle logHandle, NADKBool enable);

NADK_LOG_API void NADKLog_setDeviceID(NADKLogHandle logHandle, const char* deviceID);
NADK_LOG_API void NADKLog_setApplication(NADKLogHandle logHandle, const char* application);
NADK_LOG_API void NADKLog_setFileLogPath(NADKLogHandle logHandle, const char* path);
NADK_LOG_API void NADKLog_setFileLog(NADKLogHandle logHandle, NADKBool enable, NADKBool resetTimeStr);

NADK_LOG_API void NADKLog_setModule(NADKLogHandle logHandle, int module, NADKBool enable);
NADK_LOG_API void NADKLog_setModuleTag(NADKLogHandle logHandle, int module, const char* tag);
NADK_LOG_API void NADKLog_setModuleLevel(NADKLogHandle logHandle, int module, int level);

NADK_LOG_API void NADKLog_setOutput(NADKLogHandle logHandle, NADKBool enable);

NADK_LOG_API const char* NADKLog_getRelativeFileName(NADKLogHandle logHandle);
NADK_LOG_API const char* NADKLog_getAbsoluteFileName(NADKLogHandle logHandle);

NADK_LOG_API void  NADKLog_setUniqueID(NADKLogHandle log, const char* uniqueID);
NADK_LOG_API const char* NADKLog_getUniqueID(NADKLogHandle log);

NADK_LOG_API void* NADKLogPool_malloc(int size);
NADK_LOG_API void  NADKLogPool_free(void* buffer);

NADK_LOG_API int   NADKLog_canWrite(NADKLogHandle handle, int module, int level);
NADK_LOG_API void  NADKLog_write(NADKLogHandle handle, int module, int level, const char* tag, const char* message);

#ifdef __cplusplus
}
#endif

#endif

