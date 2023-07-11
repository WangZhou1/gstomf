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

#ifndef __NADK_LOG_DEFAULT_API_H__
#define __NADK_LOG_DEFAULT_API_H__

#include <stdio.h>
#include "NADK_LogDef.h"
#include "NADK_Log.h"
#include "default/NADK_LogDefault.h"
#include "default/NADK_LogDefaultDef.h"

#if defined(WIN32) || defined(__WINDOWS__)
#define snprintf _snprintf
#endif

#if defined(__ANDROID__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#define NADK_DEF_TRY_WRITE_LOG(module, level, tag, format, ...)     \
    do {                                                            \
        NADKLogHandle handle = NADKDefaultLog_get();                \
        if (0 != NADKLog_canWrite(handle, module, level)) { break;} \
                                                                    \
        char* message123 = (char*)NADKLogPool_malloc(512);          \
        if (message123 == NULL) { break; }                          \
                                                                    \
        snprintf(message123, 512 - 1, format, ##__VA_ARGS__);       \
        message123[512 - 1] = '\0';                                 \
                                                                    \
        NADKLog_write(handle, module, level, tag, message123);      \
        NADKLogPool_free(message123);                               \
    }while(0);

/* Common */
#define COMMON_DEF_LOG_V(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_VERB,   tag, format, ##__VA_ARGS__)
#define COMMON_DEF_LOG_D(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_DEBUG,  tag, format, ##__VA_ARGS__)
#define COMMON_DEF_LOG_I(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_INFO,   tag, format, ##__VA_ARGS__)
#define COMMON_DEF_LOG_W(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_WARN,   tag, format, ##__VA_ARGS__)
#define COMMON_DEF_LOG_E(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_ERROR,  tag, format, ##__VA_ARGS__)
#define COMMON_DEF_LOG_F(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_FATAL,  tag, format, ##__VA_ARGS__)
#define COMMON_DEF_LOG_T(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_COMMON, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__)

/* NADK */
#define NADK_DEF_LOG_V(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_VERB,   tag, format, ##__VA_ARGS__)
#define NADK_DEF_LOG_D(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_DEBUG,  tag, format, ##__VA_ARGS__)
#define NADK_DEF_LOG_I(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_INFO,   tag, format, ##__VA_ARGS__)
#define NADK_DEF_LOG_W(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_WARN,   tag, format, ##__VA_ARGS__)
#define NADK_DEF_LOG_E(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_ERROR,  tag, format, ##__VA_ARGS__)
#define NADK_DEF_LOG_F(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_FATAL,  tag, format, ##__VA_ARGS__)
#define NADK_DEF_LOG_T(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__)

/* NADK P2P */
#define NADK_P2P_DEF_LOG_V(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_VERB,   tag, format, ##__VA_ARGS__)
#define NADK_P2P_DEF_LOG_D(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_DEBUG,  tag, format, ##__VA_ARGS__)
#define NADK_P2P_DEF_LOG_I(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_INFO,   tag, format, ##__VA_ARGS__)
#define NADK_P2P_DEF_LOG_W(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_WARN,   tag, format, ##__VA_ARGS__)
#define NADK_P2P_DEF_LOG_E(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_ERROR,  tag, format, ##__VA_ARGS__)
#define NADK_P2P_DEF_LOG_F(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_FATAL,  tag, format, ##__VA_ARGS__)
#define NADK_P2P_DEF_LOG_T(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_NADK_P2P, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__)

/* Stream */
#define STREAM_DEF_LOG_V(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_VERB,   tag, format, ##__VA_ARGS__)
#define STREAM_DEF_LOG_D(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_DEBUG,  tag, format, ##__VA_ARGS__)
#define STREAM_DEF_LOG_I(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_INFO,   tag, format, ##__VA_ARGS__)
#define STREAM_DEF_LOG_W(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_WARN,   tag, format, ##__VA_ARGS__)
#define STREAM_DEF_LOG_E(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_ERROR,  tag, format, ##__VA_ARGS__)
#define STREAM_DEF_LOG_F(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_FATAL,  tag, format, ##__VA_ARGS__)
#define STREAM_DEF_LOG_T(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_STREAM, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__)

/* Render */
#define RENDER_DEF_LOG_V(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_VERB,   tag, format, ##__VA_ARGS__)
#define RENDER_DEF_LOG_D(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_DEBUG,  tag, format, ##__VA_ARGS__)
#define RENDER_DEF_LOG_I(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_INFO,   tag, format, ##__VA_ARGS__)
#define RENDER_DEF_LOG_W(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_WARN,   tag, format, ##__VA_ARGS__)
#define RENDER_DEF_LOG_E(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_ERROR,  tag, format, ##__VA_ARGS__)
#define RENDER_DEF_LOG_F(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_FATAL,  tag, format, ##__VA_ARGS__)
#define RENDER_DEF_LOG_T(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_RENDER, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__)

/* TINYAIOT */
#define TINYAIOT_DEF_LOG_V(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_TINYAIOT, NADK_LOG_LEVEL_VERB,   tag, format, ##__VA_ARGS__)
#define TINYAIOT_DEF_LOG_D(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_TINYAIOT, NADK_LOG_LEVEL_DEBUG,  tag, format, ##__VA_ARGS__)
#define TINYAIOT_DEF_LOG_I(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_TINYAIOT, NADK_LOG_LEVEL_INFO,   tag, format, ##__VA_ARGS__)
#define TINYAIOT_DEF_LOG_W(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_TINYAIOT, NADK_LOG_LEVEL_WARN,   tag, format, ##__VA_ARGS__)
#define TINYAIOT_DEF_LOG_E(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_TINYAIOT, NADK_LOG_LEVEL_ERROR,  tag, format, ##__VA_ARGS__)
#define TINYAIOT_DEF_LOG_F(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_TINYAIOT, NADK_LOG_LEVEL_FATAL,  tag, format, ##__VA_ARGS__)
#define TINYAIOT_DEF_LOG_T(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_TINYAIOT, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__)

/* lws try write */
#define THIRD_LWS_LOG_V(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_LWS, NADK_LOG_LEVEL_VERB,   tag, format, ##__VA_ARGS__);
#define THIRD_LWS_LOG_D(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_LWS, NADK_LOG_LEVEL_DEBUG,  tag, format, ##__VA_ARGS__);
#define THIRD_LWS_LOG_I(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_LWS, NADK_LOG_LEVEL_INFO,   tag, format, ##__VA_ARGS__);
#define THIRD_LWS_LOG_W(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_LWS, NADK_LOG_LEVEL_WARN,   tag, format, ##__VA_ARGS__);
#define THIRD_LWS_LOG_E(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_LWS, NADK_LOG_LEVEL_ERROR,  tag, format, ##__VA_ARGS__);
#define THIRD_LWS_LOG_F(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_LWS, NADK_LOG_LEVEL_FATAL,  tag, format, ##__VA_ARGS__);
#define THIRD_LWS_LOG_T(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_LWS, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__);

/*third party nadk try write */
#define THIRD_LIBS_LOG_V(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_THD_LIBS, NADK_LOG_LEVEL_VERB, tag, format, ##__VA_ARGS__);
#define THIRD_LIBS_LOG_D(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_THD_LIBS, NADK_LOG_LEVEL_DEBUG, tag, format, ##__VA_ARGS__);
#define THIRD_LIBS_LOG_I(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_THD_LIBS, NADK_LOG_LEVEL_INFO, tag, format, ##__VA_ARGS__);
#define THIRD_LIBS_LOG_W(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_THD_LIBS, NADK_LOG_LEVEL_WARN, tag, format, ##__VA_ARGS__);
#define THIRD_LIBS_LOG_E(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_THD_LIBS, NADK_LOG_LEVEL_ERROR, tag, format, ##__VA_ARGS__);
#define THIRD_LIBS_LOG_F(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_THD_LIBS, NADK_LOG_LEVEL_FATAL, tag, format, ##__VA_ARGS__);
#define THIRD_LIBS_LOG_T(tag, format, ...) NADK_DEF_TRY_WRITE_LOG(NADK_LOG_MODULE_THD_LIBS, NADK_LOG_LEVEL_TIMING, tag, format, ##__VA_ARGS__);

#if defined(__ANDROID__)
#pragma clang diagnostic pop
#endif

#endif
