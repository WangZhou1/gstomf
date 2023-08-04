#include "OmfMain.h"
#include "OmfH265Src.h"
#include "IH265Source.h"

/////////////////////////////////////////////
using namespace omf;
using namespace omf::api;
using namespace omf::api::streaming;
using namespace omf::api::streaming::common;
/////////////////////////////////////////////

EXTERNC void* OmfH265SrcCreate(){
	OmfMain::Initialize();
	OmfMain::Globle().LogConfig("all=false,err=true,warn=true,note=true");

	auto src = IH265Source::CreateNew("dualos");printf("[OMF]create:%p\n",src);

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

EXTERNC BOOL OmfH265SrcDestory(void* hd){printf("[OMF]destory:%p\n",hd);
	returnIfC1(FALSE, !hd);
	delete OBJECT_CONVERT(hd, IH265Source);
	return TRUE;
}

EXTERNC BOOL OmfH265SrcStatusUp(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IH265Source);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeUp(State(state));	
}

EXTERNC BOOL OmfH265SrcStatusDown(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IH265Source);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeDown(State(state));
}

EXTERNC BOOL OmfH265SrcGetFrame(void* hd, OmfFrameC_t* frame){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IH265Source);
	if(src){
		std::shared_ptr<IH265Source::frame_t> frm;
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

EXTERNC BOOL OmfH265SrcSelectSensor(void* hd, int id){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);
	return src->SelectSensor(id);
}

EXTERNC BOOL OmfH265SrcSetWidth(void* hd, int width){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);
	return src->SetWidth(width);
}

EXTERNC BOOL OmfH265SrcSetHeight(void* hd, int height){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);
	return src->SetHeight(height);
}

EXTERNC BOOL OmfH265SrcSetGop(void* hd, int gop){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);
	return src->SetGop(gop);
}

EXTERNC BOOL OmfH265SrcSetGopType(void* hd, int goptype){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);

	switch(goptype){
		case 0:
			return src->SetGopType("IBBP");
		case 1:
			return src->SetGopType("IPPP");
		case 2:
			return src->SetGopType("IIII"); 
	}
	
	return FALSE;
}

EXTERNC BOOL OmfH265SrcSetFrameRate(void* hd, int fr){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);
	return src->SetFrameRate(fr);
}

EXTERNC BOOL OmfH265SrcSetBitRate(void* hd, int br){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);
	return src->SetBitRate(br);
}

EXTERNC BOOL OmfH265SrcSetCodec(void* hd, const char* codec){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !codec);

	auto src = OBJECT_CONVERT(hd, IH265Source);
	return src->SetCodec(codec);
}

EXTERNC BOOL OmfH265SrcSetPreRecord(void* hd, int group){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);		
	return src->SetPreRecordGroup(group);
}

EXTERNC BOOL OmfH265SrcSetPreRecordPipe(void* hd, const char* pipename){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !pipename);

	auto src = OBJECT_CONVERT(hd, IH265Source);
	return src->SetPreRecordPipeline(pipename);
}

EXTERNC BOOL OmfH265SrcSetSharedEncoder(void* hd, int group){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);
	return src->SetSharedEncoderGroup(group);
}

EXTERNC BOOL OmfH265SrcSetCache(void* hd, unsigned cache){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);
	returnIfC1(FALSE, !src->SetCache(cache));
}

EXTERNC BOOL OmfH265SrcSetLowBandWidth(void* hd, BOOL en){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);
	//return src->SetLowBandWidthEnable(en);
	return FALSE;
}

EXTERNC BOOL OmfH265SrcGetMediaInfo(void* hd, const char **pmedia){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IH265Source);
		
	if(src->CurrentState() == State::null){
		returnIfC1(FALSE, !src->ChangeUp(State::ready));
	}

	auto &info = src->GetH265MediaInfo();
	*pmedia = info.media.c_str();

	return TRUE;
}


