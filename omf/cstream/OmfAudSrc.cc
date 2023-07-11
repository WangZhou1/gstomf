#include "OmfMain.h"
#include "OmfAudSrc.h"
#include "OmfIOMode.h"
#include "IAudioSource.h"

/////////////////////////////////////////////
using namespace omf;
using namespace omf::api;
using namespace omf::api::streaming;
using namespace omf::api::streaming::common;
/////////////////////////////////////////////

EXTERNC void* OmfAudSrcCreate(){
	OmfMain::Initialize();
	OmfMain::Globle().LogConfig("all=false,err=true,warn=true,note=true");

	auto src = IAudioSource::CreateNew("dualos");printf("[OMF]create:%p\n",src);

	OmfIOMode io("pull");
	returnIfC1(0, !src->IsSupportIOMode(io));
	returnIfC1(0, !src->SetFrameIOMode(io));

	return src;
}

EXTERNC BOOL OmfAudSrcDestory(void* hd){printf("[OMF]destory:%p\n",hd);
	returnIfC1(FALSE, !hd);
	delete OBJECT_CONVERT(hd, IAudioSource);
	return TRUE;
}

EXTERNC BOOL OmfAudSrcStatusUp(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IAudioSource);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeUp(State(state));	
}

EXTERNC BOOL OmfAudSrcStatusDown(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IAudioSource);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeDown(State(state));
}

EXTERNC BOOL OmfAudSrcGetFrame(void* hd, OmfFrameC_t* frame){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !frame);
	auto src = OBJECT_CONVERT(hd, IAudioSource);
	if(src){
		std::shared_ptr<IAudioSource::frame_t> frm;
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

EXTERNC BOOL OmfAudSrcSetMicSamples(void* hd, unsigned int samples){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !samples);

	auto src = OBJECT_CONVERT(hd, IAudioSource);
	return src->SetMicSamples(samples);
}

EXTERNC BOOL OmfAudSrcSetSampleRate(void* hd, unsigned int samplerate){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, samplerate <= 0);

	auto src = OBJECT_CONVERT(hd, IAudioSource);
	return src->SetSampleRate(samplerate);
}

EXTERNC BOOL OmfAudSrcSetChannel(void* hd, unsigned int channel){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, channel <= 0)

	auto src = OBJECT_CONVERT(hd, IAudioSource);
	return src->SetChannels(channel);
}

EXTERNC BOOL OmfAudSrcSetCodec(void* hd, const char* codec){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !codec);

	auto src = OBJECT_CONVERT(hd, IAudioSource);
	return src->SetCodec(codec);
}

EXTERNC BOOL OmfAudSrcSetAEC(void* hd, const char* para){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !para)

	auto src = OBJECT_CONVERT(hd, IAudioSource);
	returnIfC1(FALSE, !src->IsSupportAEC());
	
	return src->SetAEC(TRUE, para);
}

EXTERNC BOOL OmfAudSrcSetANS(void* hd, const char* para){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !para)

	auto src = OBJECT_CONVERT(hd, IAudioSource);
	returnIfC1(FALSE, !src->IsSupportANS());
	
	return src->SetANS(TRUE, para);
}

EXTERNC BOOL OmfAudSrcSetALC(void* hd, const char* para){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !para);

	auto src = OBJECT_CONVERT(hd, IAudioSource);
	returnIfC1(FALSE, !src->IsSupportALC());
	
	return src->SetALC(hd, para);
}

EXTERNC BOOL OmfAudSrcSetPreRecord(void* hd, int group){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IAudioSource);	
	return src->SetPreRecordGroup(group);
}

EXTERNC BOOL OmfAudSrcSetPreRecordPipe(void* hd, const char* pipename){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !pipename);

	auto src = OBJECT_CONVERT(hd, IAudioSource);
	return src->SetPreRecordPipeline(pipename);
}

EXTERNC BOOL OmfAudSrcSetSharedEncoder(void* hd, int group){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IAudioSource);
	return src->SetSharedEncoderGroup(group);
}


EXTERNC BOOL OmfAudSrcSetCache(void* hd, unsigned cache){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IAudioSource);	
	return src->SetCache(cache);
}


