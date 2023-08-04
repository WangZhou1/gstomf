#include "OmfMain.h"
#include "OmfJpegSrc.h"
#include "OmfIOMode.h"
#include "IJpegSource.h"

/////////////////////////////////////////////
using namespace omf;
using namespace omf::api;
using namespace omf::api::streaming;
using namespace omf::api::streaming::common;
/////////////////////////////////////////////

EXTERNC void* OmfJpegSrcCreate(){
	OmfMain::Initialize();
	OmfMain::Globle().LogConfig("all=false,err=true,warn=true,note=true");

	auto src = IJpegSource::CreateNew("dualos");printf("[OMF]create:%p\n",src);

	OmfIOMode io("pull");
	returnIfC1(0, !src->IsSupportIOMode(io));
	returnIfC1(0, !src->SetFrameIOMode(io));

	return src;
}

EXTERNC BOOL OmfJpegSrcDestory(void* hd){printf("[OMF]destory:%p\n",hd);
	returnIfC1(FALSE, !hd);
	delete OBJECT_CONVERT(hd, IJpegSource);
	return TRUE;
}

EXTERNC BOOL OmfJpegSrcStatusUp(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IJpegSource);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeUp(State(state));	
}

EXTERNC BOOL OmfJpegSrcStatusDown(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IJpegSource);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeDown(State(state));
}

EXTERNC BOOL OmfJpegSrcGetFrame(void* hd, OmfFrameC_t* frame){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IJpegSource);
	if(src){
		std::shared_ptr<IJpegSource::frame_t> frm;
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

EXTERNC BOOL OmfJpegSrcSelectSensor(void* hd, int id){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IJpegSource);
	return src->SelectSensor(id);
}

EXTERNC BOOL OmfJpegSrcSetWidth(void* hd, int width){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IJpegSource);
	return src->SetWidth(width);
}

EXTERNC BOOL OmfJpegSrcSetHeight(void* hd, int height){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IJpegSource);
	return src->SetHeight(height);
}

EXTERNC BOOL OmfJpegSrcSetQP(void* hd, int qp){
	returnIfC1(FALSE, !hd);
	
	auto src = OBJECT_CONVERT(hd, IJpegSource);
	returnIfC1(FALSE, !src->IsSupportCache());

	return src->SetQP(qp);
}

EXTERNC BOOL OmfJpegSrcSetPreRecord(void* hd, unsigned group){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IJpegSource);
	returnIfC1(FALSE, !src->IsSupportPreRecord());

	return src->SetPreRecordGroup(group);
}

EXTERNC BOOL OmfJpegSrcGetMediaInfo(void* hd, const char **pmedia){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IJpegSource);
		
	if(src->CurrentState() == State::null){
		returnIfC1(FALSE, !src->ChangeUp(State::ready));
	}

	auto &info = src->GetJpegMediaInfo();
	*pmedia = info.media.c_str();
	
	return TRUE;
}

