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

#ifndef __NADK_LOG_DEFAULT_DEF_H__
#define __NADK_LOG_DEFAULT_DEF_H__

#include "NADK_LogDef.h"

typedef enum NADK_Default_LogModule
{
    /* specialy for lws */
    NADK_LOG_MODULE_LWS  = NADK_LOG_MODULE_THIRD_LIB1,
    /* macro for all Aiot third party libraries, excpet for lws */
    NADK_LOG_MODULE_THD_LIBS = NADK_LOG_MODULE_THIRD_LIB2,
} NADK_Default_LogModule;

#endif

