//
// Created by jg.wang on 2019/11/27.
//

#pragma once

#include <cstring>
#include <iostream>
#include "_dbg_color.h"
#include "_dump.h"
#include "_hex.h"
#include "_dbg_api_def.h"
#include "_dbg_cfg_switch.h"
#include "_dbg_cfg_base.h"
#include "_dbg_cout_std.h"
#include "_dbg_cfg_cout.h"
#include "OmfLoggerApi.h"
#include <sstream>
///////////////////////
#if NADK_LOG
extern int _gLevel;
#endif
////////////
#define dbgDebugDump0(ptr,len)			{Dump dump((void*)ptr,len); \
                                        OmfLoggerWrite(OMF_LOG_LEVEL_DEBUG,"debug",dump._str.c_str());}
#define dbgInfoDump0(ptr,len)			{Dump dump((void*)ptr,len); \
                                        OmfLoggerWrite(OMF_LOG_LEVEL_INFO,"info",dump._str.c_str());}
#define dbgWarnDump0(ptr,len)			{Dump dump((void*)ptr,len); \
                                        OmfLoggerWrite(OMF_LOG_LEVEL_WARN,"warn",dump._str.c_str());}
#define dbgErrDump0(ptr,len)			{Dump dump((void*)ptr,len); \
                                        OmfLoggerWrite(OMF_LOG_LEVEL_ERROR,"info",dump._str.c_str());}
/////
extern const char* dbgGetFileName(const char*path);
///
#define CVAL(v) #v<<'='<<v
#define ENDL() std::endl
#define POSTION dbgGetFileName(__FILE__)<<'/'<<__LINE__<<':'<<__FUNCTION__<<"()"
///
#if NADK_LOG
#define dbgTestS(s) {if(_gLevel<=OMF_LOG_LEVEL_VERB){std::stringstream ss;\
                    ss<<s;\
                    OmfLoggerWrite(OMF_LOG_LEVEL_VERB,"verb",ss.str().c_str());\
                    ss.str("");}\
                    }
#else
#define dbgTestS(s) dbgEntryTest(s)
#endif
///
#define dbgTestSL(s) dbgTestS(s<<ENDL())
#define dbgTestVL(s) dbgTestSL(CVAL(s))
#define dbgTestPL(s) dbgTestSL(POSTION)
#define dbgTestPS(s) dbgTestS(POSTION<<'#'<<s)
#define dbgTestPSL(s) dbgTestSL(POSTION<<'#'<<s)
#define dbgTestPVL(s) dbgTestPSL(CVAL(s))
#define dbgTestD(ptr,len) dbgTestS(dbgEntryDump(ptr,len))
#define dbgTestDL(ptr,len) dbgTestSL(dbgEntryDump(ptr,len))
#define dbgTestPD(ptr,len) dbgTestPS(dbgEntryDump(ptr,len))
#define dbgTestPDL(ptr,len) dbgTestPSL(dbgEntryDump(ptr,len))


///
#if NADK_LOG
#define dbgNoteS(s) {if(_gLevel<=OMF_LOG_LEVEL_INFO){std::stringstream ss;\
                    ss<<s;\
                    OmfLoggerWrite(OMF_LOG_LEVEL_INFO,"info",ss.str().c_str());\
                    ss.str("");}\
                    }
#else
#define dbgNoteS(s) dbgEntryNote(s)
#endif
///
#define dbgNoteSL(s) dbgNoteS(s<<ENDL())
#define dbgNoteVL(s) dbgNoteSL(CVAL(s))
#define dbgNotePL(s) dbgNoteSL(POSTION)
#define dbgNotePS(s) dbgNoteS(POSTION<<'#'<<s)

#define dbgNotePSL(s) dbgNoteSL(POSTION<<'#'<<s)
#define dbgNotePVL(s) dbgNotePSL(CVAL(s))
#define dbgNoteD(ptr,len) 	dbgNoteS(dbgEntryDump(ptr,len))
#define dbgNoteDL(ptr,len) 	dbgNoteSL(dbgEntryDump(ptr,len))
#define dbgNotePD(ptr,len) 	dbgNotePS(dbgEntryDump(ptr,len))
#define dbgNotePDL(ptr,len) dbgNotePSL(dbgEntryDump(ptr,len))




///
#if NADK_LOG
#define dbgErrS(s) {if(_gLevel<=OMF_LOG_LEVEL_ERROR){std::stringstream ss;\
                    ss<<s;\
                    OmfLoggerWrite(OMF_LOG_LEVEL_ERROR,"error",ss.str().c_str());\
                    ss.str("");}\
                    }
#else
#define dbgErrS(s) dbgEntryErr(s)
#endif
///
#define dbgErrSL(s) dbgErrS(s<<ENDL())
#define dbgErrVL(s) dbgErrSL(CVAL(s))
#define dbgErrPL(s) dbgErrSL(POSTION)
#define dbgErrPS(s) dbgErrS(POSTION<<'#'<<s)
#define dbgErrPSL(s) dbgErrSL(POSTION<<'#'<<s)
#define dbgErrPVL(s) dbgErrPSL(CVAL(s))
#define dbgErrD(ptr,len) 	dbgErrS(dbgEntryDump(ptr,len))
#define dbgErrDL(ptr,len) 	dbgErrSL(dbgEntryDump(ptr,len))
#define dbgErrPD(ptr,len) 	dbgErrPS(dbgEntryDump(ptr,len))
#define dbgErrPDL(ptr,len) 	dbgErrPSL(dbgEntryDump(ptr,len))
///
#define returnIfErrC(v,c) 		if(c){dbgErrPSL(#c);return v;}
#define returnIfErrC0(c)		if(c){dbgErrPSL(#c);return;}
#define returnIfErrCS(v,c,s)	if(c){dbgErrPSL(#c<<','<<s);return v;}
#define returnIfErrCS0(c,s)		if(c){dbgErrPSL(#c<<','<<s);return;}
///
#define returnIfTestC(v,c) 		if(c){dbgTestPSL(#c);return v;}
#define returnIfTestC0(c)		if(c){dbgTestPSL(#c);return;}
#define returnIfTestCS(v,c,s)	if(c){dbgTestPSL(#c<<','<<s);return v;}
#define returnIfTestCS0(c,s)	if(c){dbgTestPSL(#c<<','<<s);return;}
///
#define returnIfNoteC(v,c) 		if(c){dbgNotePSL(#c);return v;}
#define returnIfNoteC0(c)		if(c){dbgNotePSL(#c);return;}
#define returnIfNoteCS(v,c,s)	if(c){dbgNotePSL(#c<<','<<s);return v;}
#define returnIfNoteCS0(c,s)	if(c){dbgNotePSL(#c<<','<<s);return;}



///
#if NADK_LOG
#define dbgInfoS(s) {if(_gLevel<=OMF_LOG_LEVEL_INFO){std::stringstream ss;\
                    ss<<s;\
                    OmfLoggerWrite(OMF_LOG_LEVEL_INFO,"info",ss.str().c_str());\
                    ss.str("");}\
                    }
#else
#define dbgInfoS(s) dbgEntryInfo(s)
#endif
///
#define dbgInfoSL(s) 	dbgInfoS(s<<ENDL())
#define dbgInfoVL(s) 	dbgInfoSL(CVAL(s))
#define dbgInfoPL(s) 	dbgInfoSL(POSTION)
#define dbgInfoPS(s) 	dbgInfoS(POSTION<<'#'<<s)
#define dbgInfoPSL(s) 	dbgInfoSL(POSTION<<'#'<<s)
#define dbgInfoPVL(s) 	dbgInfoPSL(CVAL(s))
#define dbgInfoD(ptr,len) 		dbgInfoS(dbgEntryDump(ptr,len))
#define dbgInfoDL(ptr,len) 		dbgInfoSL(dbgEntryDump(ptr,len))
#define dbgInfoPD(ptr,len) 		dbgInfoPS(dbgEntryDump(ptr,len))
#define dbgInfoPDL(ptr,len) 	dbgInfoPSL(dbgEntryDump(ptr,len))



///
#if NADK_LOG
#define dbgInfoGS(s) {if(_gLevel<=OMF_LOG_LEVEL_DEBUG){std::stringstream ss;\
                    ss<<s;\
                    OmfLoggerWrite(OMF_LOG_LEVEL_DEBUG,"debug",ss.str().c_str());\
                    ss.str("");}\
                    }
#else
#define dbgInfoGS(s) dbgEntryInfoG(s)
#endif

///
#define dbgInfoGSL(s) 	dbgInfoGS(s<<ENDL())
#define dbgInfoGVL(s) 	dbgInfoGSL(CVAL(s))
#define dbgInfoGPL(s) 	dbgInfoGSL(POSTION)
#define dbgInfoGPS(s) 	dbgInfoGS(POSTION<<'#'<<s)
#define dbgInfoGPSL(s) 	dbgInfoGSL(POSTION<<'#'<<s)
#define dbgInfoGPVL(s) 	dbgInfoGPSL(CVAL(s))
#define dbgInfoGD(ptr,len) 		dbgInfoGS(dbgEntryDump(ptr,len))
#define dbgInfoGDL(ptr,len) 	dbgInfoGSL(dbgEntryDump(ptr,len))
#define dbgInfoGPD(ptr,len) 	dbgInfoGPS(dbgEntryDump(ptr,len))
#define dbgInfoGPDL(ptr,len) 	dbgInfoGPSL(dbgEntryDump(ptr,len))



///
#if NADK_LOG
#define dbgLogS(s) {if(_gLevel<=OMF_LOG_LEVEL_DEBUG){std::stringstream ss;\
                    ss<<s;\
                    OmfLoggerWrite(OMF_LOG_LEVEL_DEBUG,"debug",ss.str().c_str());\
                    ss.str("");}\
                    }
#else
#define dbgLogS(s) dbgEntryLog(s)
#endif
///
#define dbgLogSL(s) dbgLogS(s<<ENDL())
#define dbgLogVL(s) dbgLogSL(CVAL(s))
#define dbgLogPL(s) dbgLogSL(POSTION)
#define dbgLogPS(s) dbgLogS(POSTION<<'#'<<s)
#define dbgLogPSL(s) dbgLogSL(POSTION<<'#'<<s)
#define dbgLogPVL(s) dbgLogPSL(CVAL(s))
#define dbgLogD(ptr,len) 	dbgLogS(dbgEntryDump(ptr,len))
#define dbgLogDL(ptr,len) 	dbgLogSL(dbgEntryDump(ptr,len))
#define dbgLogPD(ptr,len) 	dbgLogPS(dbgEntryDump(ptr,len))
#define dbgLogPDL(ptr,len) 	dbgLogPSL(dbgEntryDump(ptr,len))



////////////////////////////////////////////////////
#if NADK_LOG
#define dbgWarnS(s) {if(_gLevel<=OMF_LOG_LEVEL_WARN){std::stringstream ss;\
                    ss<<s;\
                    OmfLoggerWrite(OMF_LOG_LEVEL_WARN,"warn",ss.str().c_str());\
                    ss.str("");}\
                    }
#else
#define dbgWarnS(s) dbgEntryWarn(s)
#endif

///
#define dbgWarnSL(s) dbgWarnS(s<<ENDL())
#define dbgWarnVL(s) dbgWarnSL(CVAL(s))
#define dbgWarnPL(s) dbgWarnSL(POSTION)
#define dbgWarnPS(s) dbgWarnS(POSTION<<'#'<<s)
#define dbgWarnPSL(s) dbgWarnSL(POSTION<<'#'<<s)
#define dbgWarnPVL(s) dbgWarnPSL(CVAL(s))
#define dbgWarnD(ptr,len) 	dbgWarnS(dbgEntryDump(ptr,len))
#define dbgWarnDL(ptr,len) 	dbgWarnSL(dbgEntryDump(ptr,len))
#define dbgWarnPD(ptr,len) 	dbgWarnPS(dbgEntryDump(ptr,len))
#define dbgWarnPDL(ptr,len) 	dbgWarnPSL(dbgEntryDump(ptr,len))