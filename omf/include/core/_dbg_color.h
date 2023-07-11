//
// Created by jg.wang on 2017/5/19.
//
#pragma once

namespace omf{
	namespace dbg{
		namespace color{
			const char* const  Normal		="\033[0m"  ;
			///foreground
			const char* const  Red 			="\033[31m" ;
			const char* const  Green 		="\033[32m" ;
			const char* const  Yellow 		="\033[33m" ;
			const char* const  Blue 		="\033[34m" ;
			const char* const  Pink 		="\033[35m" ;
			const char* const  SkyBlue 		="\033[36m" ;
			const char* const  White 		="\033[37m" ;
			///
			const char* const  Error 		="\033[31m" ; //red
			const char* const  Log			="\033[32m" ; //green
			const char* const  Warn 		="\033[33m" ; //yellow
			const char* const  Test			="\033[34m" ; //blue
			const char* const  Note 		="\033[35m" ; //pink
			const char* const  Info 		="\033[36m" ; //sky
			
			///background
			const char* const  Red_ 		="\033[41m" ;
			const char* const  Green_ 		="\033[42m" ;
			const char* const  Yellow_ 		="\033[43m" ;
			const char* const  Blue_ 		="\033[44m" ;
			const char* const  Pink_ 		="\033[45m" ;
			const char* const  SkyBlue_ 	="\033[46m" ;
			const char* const  White_ 		="\033[47m" ;
		}
	}
}

#define DBGRed(s)		omf::dbg::color::Red 		<<s<<	omf::dbg::color::Normal
#define DBGGreen(s)		omf::dbg::color::Green 		<<s<<	omf::dbg::color::Normal
#define DBGYellow(s)	omf::dbg::color::Yellow 	<<s<<	omf::dbg::color::Normal
#define DBGBlue(s)		omf::dbg::color::Blue 		<<s<<	omf::dbg::color::Normal
#define DBGPink(s)		omf::dbg::color::Pink 		<<s<<	omf::dbg::color::Normal
#define DBGSky(s)		omf::dbg::color::SkyBlue 	<<s<<	omf::dbg::color::Normal
#define DBGSkyBlue(s)	omf::dbg::color::SkyBlue 	<<s<<	omf::dbg::color::Normal
#define DBGWhite(s)		omf::dbg::color::White 		<<s<<	omf::dbg::color::Normal
#define DBGRed_(s)		omf::dbg::color::Red_ 		<<s<<	omf::dbg::color::Normal
#define DBGGreen_(s)	omf::dbg::color::Green_ 	<<s<<	omf::dbg::color::Normal
#define DBGYellow_(s)	omf::dbg::color::Yellow_ 	<<s<<	omf::dbg::color::Normal
#define DBGBlue_(s)		omf::dbg::color::Blue_ 		<<s<<	omf::dbg::color::Normal
#define DBGPink_(s)		omf::dbg::color::Pink_ 		<<s<<	omf::dbg::color::Normal
#define DBGSky_(s)		omf::dbg::color::SkyBlue_ 	<<s<<	omf::dbg::color::Normal
#define DBGSkyBlue_(s)	omf::dbg::color::SkyBlue_ 	<<s<<	omf::dbg::color::Normal
#define DBGWhite_(s)	omf::dbg::color::White_ 	<<s<<	omf::dbg::color::Normal

#if 0
#define DBGError(s)		omf::dbg::color::Error 		<<	"[omf][E]" <<s<<	omf::dbg::color::Normal
#define DBGWarn(s)		omf::dbg::color::Warn 		<<	"[omf][W]" <<s<<	omf::dbg::color::Normal
#define DBGNote(s)		omf::dbg::color::Note	 	<<	"[omf][N]" <<s<<	omf::dbg::color::Normal
#define DBGInfo(s)		omf::dbg::color::Info 		<<	"[omf][I]" <<s<<	omf::dbg::color::Normal
#define DBGInfoG(s)		omf::dbg::color::Log 		<<	"[omf][I]" <<s<<	omf::dbg::color::Normal
#define DBGLog(s)		omf::dbg::color::Log 		<<	"[omf][L]" <<s<<	omf::dbg::color::Normal
#define DBGTest(s)		omf::dbg::color::Test 		<<	"[omf][T]" <<s<<	omf::dbg::color::Normal
#else
long long GetDbgTime();
#define DBGTime 		GetDbgTime()
#define DBGError(s)		omf::dbg::color::Error 		<<	"BpSC|" << DBGTime << "|0|E|omf: " <<s<<	omf::dbg::color::Normal
#define DBGWarn(s)		omf::dbg::color::Warn 		<<	"BpSC|" << DBGTime << "|0|W|omf: " <<s<<	omf::dbg::color::Normal
#define DBGNote(s)		omf::dbg::color::Note	 	<<	"BpSC|" << DBGTime << "|0|N|omf: " <<s<<	omf::dbg::color::Normal
#define DBGInfo(s)		omf::dbg::color::Info 		<<	"BpSC|" << DBGTime << "|0|I|omf: " <<s<<	omf::dbg::color::Normal
#define DBGInfoG(s)		omf::dbg::color::Log 		<<	"BpSC|" << DBGTime << "|0|I|omf: " <<s<<	omf::dbg::color::Normal
#define DBGLog(s)		omf::dbg::color::Log 		<<	"BpSC|" << DBGTime << "|0|L|omf: " <<s<<	omf::dbg::color::Normal
#define DBGTest(s)		omf::dbg::color::Test 		<<	"BpSC|" << DBGTime << "|0|T|omf: " <<s<<	omf::dbg::color::Normal
#endif