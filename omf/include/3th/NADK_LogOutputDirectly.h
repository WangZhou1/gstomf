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

#ifndef __NADK_LOG_OUTPUT_DIRECTLY_H__
#define __NADK_LOG_OUTPUT_DIRECTLY_H__

#include "NADK_LogDef.h"

#ifdef __cplusplus
extern "C" {
#endif

NADK_LOG_API void* NADKLogPool_malloc(int size);
NADK_LOG_API void  NADKLogPool_free(void* buffer);
NADK_LOG_API void  nadk_write_log_directly(const char* tag, const char* message);

#ifdef __cplusplus
}
#endif

#ifdef NADK_LOG_INTERNAL_IMPL
#error "The file should not be used in interval log impl"
#endif

#define NADK_LOG_WRITE_DIRECTLY(tag, format, ...)                   \
    do {                                                            \
        char* message123 = (char*)NADKLogPool_malloc(512);          \
        if (message123 == NULL) { break; }                          \
                                                                    \
        snprintf(message123, 512 - 1, format, ##__VA_ARGS__);       \
        message123[512 - 1] = '\0';                                 \
                                                                    \
        nadk_write_log_directly(tag, message123);                   \
        NADKLogPool_free(message123);                               \
    } while(0);

#endif
