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

#ifndef __NADK_LOGGER_API_H__
#define __NADK_LOGGER_API_H__

#include <stdio.h>
#include "NADK_Log.h"

#if defined(WIN32) || defined(__WINDOWS__)
#define snprintf _snprintf
#endif

#if defined(__ANDROID__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#define NADK_TRY_WRITE_LOG(handle, module, level, tag, format, ...) \
    do {                                                            \
        if (0 != NADKLog_canWrite(handle, module, level)) { break;} \
                                                                    \
        char* message123 = (char*)NADKLogPool_malloc(512);          \
        if (message123 == nullptr) { break; }                       \
                                                                    \
        snprintf(message123, 512 - 1, format, ##__VA_ARGS__);       \
        message123[512 - 1] = '\0';                                 \
                                                                    \
        NADKLog_write(handle, module, level, tag, message123);      \
        NADKLogPool_free(message123);                               \
    }while(0);

/* Common */
#define COMMON_LOG_V(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_VERB, tag, format, ##__VA_ARGS__)

#define COMMON_LOG_D(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_DEBUG, tag, format, ##__VA_ARGS__)

#define COMMON_LOG_I(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_INFO, tag, format, ##__VA_ARGS__)

#define COMMON_LOG_W(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_WARN, tag, format, ##__VA_ARGS__)

#define COMMON_LOG_E(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_ERROR, tag, format, ##__VA_ARGS__)

#define COMMON_LOG_F(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_FATAL, tag, format, ##__VA_ARGS__)

#define COMMON_LOG_T(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__)

/* NADK */
#define NADK_LOG_V(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_VERB, tag, format, ##__VA_ARGS__)

#define NADK_LOG_D(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_DEBUG, tag, format, ##__VA_ARGS__)

#define NADK_LOG_I(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_INFO, tag, format, ##__VA_ARGS__)

#define NADK_LOG_W(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_WARN, tag, format, ##__VA_ARGS__)

#define NADK_LOG_E(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_ERROR, tag, format, ##__VA_ARGS__)

#define NADK_LOG_F(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_FATAL, tag, format, ##__VA_ARGS__)

#define NADK_LOG_T(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__)

/* NADK P2P  */
#define NADK_P2P_LOG_V(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_VERB, tag, format, ##__VA_ARGS__)

#define NADK_P2P_LOG_D(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_DEBUG, tag, format, ##__VA_ARGS__)

#define NADK_P2P_LOG_I(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_INFO, tag, format, ##__VA_ARGS__)

#define NADK_P2P_LOG_W(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_WARN, tag, format, ##__VA_ARGS__)

#define NADK_P2P_LOG_E(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_ERROR, tag, format, ##__VA_ARGS__)

#define NADK_P2P_LOG_F(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_FATAL, tag, format, ##__VA_ARGS__)

#define NADK_P2P_LOG_T(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__)

/* STREAM  */
#define STREAM_LOG_V(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_VERB, tag, format, ##__VA_ARGS__)

#define STREAM_LOG_D(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_DEBUG, tag, format, ##__VA_ARGS__)

#define STREAM_LOG_I(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_INFO, tag, format, ##__VA_ARGS__)

#define STREAM_LOG_W(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_WARN, tag, format, ##__VA_ARGS__)

#define STREAM_LOG_E(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_ERROR, tag, format, ##__VA_ARGS__)

#define STREAM_LOG_F(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_FATAL, tag, format, ##__VA_ARGS__)

#define STREAM_LOG_T(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__)

/* Render  */
#define RENDER_LOG_V(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_VERB, tag, format, ##__VA_ARGS__)

#define RENDER_LOG_D(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_DEBUG, tag, format, ##__VA_ARGS__)

#define RENDER_LOG_I(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_INFO, tag, format, ##__VA_ARGS__)

#define RENDER_LOG_W(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_WARN, tag, format, ##__VA_ARGS__)

#define RENDER_LOG_E(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_ERROR, tag, format, ##__VA_ARGS__)

#define RENDER_LOG_F(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_FATAL, tag, format, ##__VA_ARGS__)

#define RENDER_LOG_T(handle, tag, format, ...) \
    NADK_TRY_WRITE_LOG(handle, NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__)

#if defined(__ANDROID__)
#pragma clang diagnostic pop
#endif

#endif

