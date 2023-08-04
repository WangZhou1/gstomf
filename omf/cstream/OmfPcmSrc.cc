#include "OmfMain.h"
#include "OmfPcmSrc.h"
#include "OmfIOMode.h"
#include "IPcmSource.h"

/////////////////////////////////////////////
using namespace omf;
using namespace omf::api;
using namespace omf::api::streaming;
using namespace omf::api::streaming::common;
/////////////////////////////////////////////

EXTERNC void* OmfPcmSrcCreate(){
	OmfMain::Initialize();
	OmfMain::Globle().LogConfig("all=false,err=true,warn=true,note=true");

	auto src = IPcmSource::CreateNew("dualos");printf("[OMF]create:%p\n",src);

	OmfIOMode io("pull");
	returnIfC1(0, !src->IsSupportIOMode(io));
	returnIfC1(0, !src->SetFrameIOMode(io));

	return src;
}

EXTERNC BOOL OmfPcmSrcDestory(void* hd){printf("[OMF]destory:%p\n",hd);
	returnIfC1(FALSE, !hd);
	delete OBJECT_CONVERT(hd, IPcmSource);
	return TRUE;
}

EXTERNC BOOL OmfPcmSrcStatusUp(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IPcmSource);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeUp(State(state));	
}

EXTERNC BOOL OmfPcmSrcStatusDown(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IPcmSource);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeDown(State(state));
}

EXTERNC BOOL OmfPcmSrcGetFrame(void* hd, OmfFrameC_t* frame){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IPcmSource);
	if(src){
		std::shared_ptr<IPcmSource::frame_t> frm;
		if(src->PullFrame(frm)){
			if(frm && frm->data && frm->size){
				memcpy(frame, frm.get(), sizeof(OmfFrameC_t));
				
				frame->data = malloc(frm->size);
				returnIfC1(FALSE, !frame->data);

				frame->free = free;
				memcpy(frame->data, frm->data, frm->size);
											
				return TRUE;
			}
		}
	}
	return FALSE;
}

EXTERNC BOOL OmfPcmSrcSetSampleRate(void* hd, unsigned int samplerate){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, samplerate <= 0);

	auto src = OBJECT_CONVERT(hd, IPcmSource);
	return src->SetSampleRate(samplerate);
}

EXTERNC BOOL OmfPcmSrcSetChannel(void* hd, unsigned int channel){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IPcmSource);
	if( channel > 0){
		returnIfC1(FALSE, !src->SetChannels(channel));
	}
	return TRUE;
}

EXTERNC BOOL OmfPcmSrcSetAEC(void* hd, int level){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !level);
	
	auto src = OBJECT_CONVERT(hd, IPcmSource);
	returnIfC1(FALSE, !src->IsSupportAEC());

	char aec[64];
	sprintf(aec,"keys=webrtc,level=%d,samples=1600", level-1);

	return src->SetAEC(TRUE, aec);
}

EXTERNC BOOL OmfPcmSrcSetANS(void* hd, int mode){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !mode);
	
	auto src = OBJECT_CONVERT(hd, IPcmSource);
	returnIfC1(FALSE, !src->IsSupportANS());

	char ans[64];
	sprintf(ans,"keys=webrtc,mode=%d,samples=1600", mode-1);
	
	return src->SetANS(TRUE, ans);
}

EXTERNC BOOL OmfPcmSrcSetPreRecord(void* hd, unsigned group){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IPcmSource);
	returnIfC1(FALSE, !src->IsSupportPreRecord());

	return src->SetPreRecordGroup(group);
}

EXTERNC BOOL OmfPcmSrcSetPreRecordPipe(void* hd, const char* pipename){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !pipename);

	auto src = OBJECT_CONVERT(hd, IPcmSource);
	returnIfC1(FALSE, !src->IsSupportPreRecord());

	return src->SetPreRecordPipeline(pipename);
}

EXTERNC BOOL OmfPcmSrcSetCache(void* hd, unsigned cache){
	returnIfC1(FALSE, !hd);
	
	auto src = OBJECT_CONVERT(hd, IPcmSource);
	returnIfC1(FALSE, !src->IsSupportCache());

	return src->SetCache(cache);
}

EXTERNC BOOL OmfPcmSrcGetMediaInfo(void* hd, const char** pmedia){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IPcmSource);
		
	if(src->CurrentState() == State::null){
		returnIfC1(FALSE, !src->ChangeUp(State::ready));
	}

	auto &info = src->GetPcmMediaInfo();
	*pmedia = info.media.c_str();

	return TRUE;
}

