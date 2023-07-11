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

#ifndef __NADK_LOGG_DEF_H__
#define __NADK_LOGG_DEF_H__

#if defined(__WINDOWS__)
  #if defined(NADK_API_EXPORT)
    #define NADK_LOG_API __declspec(dllexport)
  #else
    #define NADK_LOG_API
  #endif
#else
  #define NADK_LOG_API
#endif

#if defined(__ICATOS__)
#define NADKLogHandle long
#else
#define NADKLogHandle long long
#endif

#define NADKBool   int
#define NADK_TRUE  1
#define NADK_FALSE 0

typedef enum NADK_StandardLogModule
{
    /* The predefined module  */
    NADK_LOG_MODULE_COMMON      = 0x00,

    NADK_LOG_MODULE_NADK        = 0x01,
    NADK_LOG_MODULE_NADK_P2P    = 0x02,

    NADK_LOG_MODULE_STREAM      = 0x03,
    NADK_LOG_MODULE_RENDER      = 0x04,

    NADK_LOG_MODULE_SPHOST      = 0x05,
    NADK_LOG_MODULE_OMF         = 0x06,

    NADK_LOG_MODULE_ACCOUNT     = 0x07,
    NADK_LOG_MODULE_CONTROL     = 0x08,
    NADK_LOG_MODULE_FILE        = 0x09,
    NADK_LOG_MODULE_APP         = 0x0A,
    NADK_LOG_MODULE_TINYAIOT    = 0x0B,


    /* 0x0B - 0x0C */
    /* reserved for pre defined module */

    /* Leaved for later important modules
     * !! do not use the leaved modules !!
     */
    NADK_LOG_MODULE_RESERVED2     = 0x0D,

    /* Leaved for specified third libraries,
     * !! do not use the leaved modules !!
     */
    NADK_LOG_MODULE_THIRD_LIB1    = 0x0E,
    NADK_LOG_MODULE_THIRD_LIB2    = 0x0F,

    /* The max standard module max_index */
    NADK_LOG_STANDARD_TYPE_MAX
} NADK_StandardLogModule;

typedef enum NADK_LogLevel
{
    NADK_LOG_LEVEL_VERB     = 0, //NADK_LIBLEVEL_VERB,
    NADK_LOG_LEVEL_DEBUG    = 1, //NADK_LIBLEVEL_DEBG,
    NADK_LOG_LEVEL_INFO     = 2, //NADK_LIBLEVEL_INFO,
    NADK_LOG_LEVEL_WARN     = 3, //NADK_LIBLEVEL_WARN,
    NADK_LOG_LEVEL_ERROR    = 4, //NADK_LIBLEVEL_ERRO,
    NADK_LOG_LEVEL_FATAL    = 5, //NADK_LIBLEVEL_FERR,
    NADK_LOG_LEVEL_TIMING   = 6, //NADK_LIBLEVEL_TIMING,
} NADK_LogLevel;

#endif
