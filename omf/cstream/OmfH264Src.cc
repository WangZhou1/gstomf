#include "OmfMain.h"
#include "OmfH264Src.h"
#include "IH264Source.h"

/////////////////////////////////////////////
using namespace omf;
using namespace omf::api;
using namespace omf::api::streaming;
using namespace omf::api::streaming::common;
/////////////////////////////////////////////

EXTERNC void* OmfH264SrcCreate(){
	OmfMain::Initialize();
	OmfMain::Globle().LogConfig("all=false,err=true,warn=true,note=true");

	auto src = IH264Source::CreateNew("dualos");printf("[OMF]create:%p\n",src);

	///set BitRateControl Mode
	OmfBRCMode brc("vbrc");
	returnIfC1(NULL, !src->IsSupportBRCMode(brc));
	returnIfC1(NULL, !src->SetBRCMode(brc));

	///set FrameRateControl Mode
	OmfFRCMode frc("vfrc");
	returnIfC1(NULL, !src->IsSupportFRCMode(frc));
	returnIfC1(NULL, !src->SetFRCMode(frc));

	///set IOControl Mode
	OmfIOMode io("pull");
	returnIfC1(NULL, !src->IsSupportIOMode(io));
	returnIfC1(NULL, !src->SetFrameIOMode(io));

	return src;
}

EXTERNC BOOL OmfH264SrcDestory(void* hd){printf("[OMF]destory:%p\n",hd);
	returnIfC1(FALSE, !hd);
	delete OBJECT_CONVERT(hd, IH264Source);
	return TRUE;
}

EXTERNC BOOL OmfH264SrcStatusUp(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IH264Source);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeUp(State(state));	
}

EXTERNC BOOL OmfH264SrcStatusDown(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IH264Source);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeDown(State(state));
}

EXTERNC BOOL OmfH264SrcGetFrame(void* hd, OmfFrameC_t* frame){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IH264Source);
	if(src){
		std::shared_ptr<IH264Source::frame_t> frm;
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

EXTERNC BOOL OmfH264SrcSelectSensor(void* hd, int id){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	return src->SelectSensor(id);
}

EXTERNC BOOL OmfH264SrcSetWidth(void* hd, int width){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	return src->SetWidth(width);
}

EXTERNC BOOL OmfH264SrcSetHeight(void* hd, int height){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	return src->SetHeight(height);
}

EXTERNC BOOL OmfH264SrcSetGop(void* hd, int gop){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	return src->SetGop(gop);
}

EXTERNC BOOL OmfH264SrcSetGopType(void* hd, const char* goptype){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	return src->SetGopType(goptype);
}

EXTERNC BOOL OmfH264SrcSetFrameRate(void* hd, int fr){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	return src->SetFrameRate(fr);
}

EXTERNC BOOL OmfH264SrcSetBitRate(void* hd, int br){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	return src->SetBitRate(br);
}

EXTERNC BOOL OmfH264SrcSetCodec(void* hd, const char* codec){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !codec);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	return src->SetCodec(codec);
}

EXTERNC BOOL OmfH264SrcSetPreRecord(void* hd, int group){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH264Source);		
	return src->SetPreRecordGroup(group);
}

EXTERNC BOOL OmfH264SrcSetPreRecordPipe(void* hd, const char* pipename){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !pipename);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	return src->SetPreRecordPipeline(pipename);
}

EXTERNC BOOL OmfH264SrcSetSharedEncoder(void* hd, int group){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	return src->SetSharedEncoderGroup(group);
}

EXTERNC BOOL OmfH264SrcSetCache(void* hd, unsigned cache){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	returnIfC1(FALSE, !src->SetCache(cache));
}

EXTERNC BOOL OmfH264SrcSetLowBandWidth(void* hd, BOOL en){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH264Source);
	return src->SetLowBandWidthEnable(en);
}

