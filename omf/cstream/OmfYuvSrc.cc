#include "OmfMain.h"
#include "OmfYuvSrc.h"
#include "OmfIOMode.h"
#include "IYuvSource.h"

/////////////////////////////////////////////
using namespace omf;
using namespace omf::api;
using namespace omf::api::streaming;
using namespace omf::api::streaming::common;
/////////////////////////////////////////////

EXTERNC void* OmfYuvSrcCreate(){
	OmfMain::Initialize();
	OmfMain::Globle().LogConfig("all=false,err=true,warn=true,note=true");

	auto src = IYuvSource::CreateNew("dualos");printf("[OMF]create:%p\n",src);

	OmfIOMode io("pull");
	returnIfC1(0, !src->IsSupportIOMode(io));
	returnIfC1(0, !src->SetFrameIOMode(io));

	return src;
}

EXTERNC BOOL OmfYuvSrcDestory(void* hd){printf("[OMF]destory:%p\n",hd);
	returnIfC1(FALSE, !hd);
	delete OBJECT_CONVERT(hd, IYuvSource);
	return TRUE;
}

EXTERNC BOOL OmfYuvSrcStatusUp(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IYuvSource);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeUp(State(state));	
}

EXTERNC BOOL OmfYuvSrcStatusDown(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IYuvSource);printf("[OMF]%p-state:%d\n",src,state);
	return src->ChangeDown(State(state));
}

EXTERNC BOOL OmfYuvSrcGetFrame(void* hd, OmfFrameC_t* frame){
	returnIfC1(FALSE, !hd);
	auto src = OBJECT_CONVERT(hd, IYuvSource);
	if(src){
		std::shared_ptr<IYuvSource::frame_t> frm;
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

EXTERNC BOOL OmfYuvSrcSelectSensor(void* hd, int id){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IYuvSource);
	return src->SelectSensor(id);
}

EXTERNC BOOL OmfYuvSrcSetWidth(void* hd, int width){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IYuvSource);
	return src->SetWidth(width);
}

EXTERNC BOOL OmfYuvSrcSetHeight(void* hd, int height){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IYuvSource);
	return src->SetHeight(height);
}

EXTERNC BOOL OmfYuvSrcSetInterlaced(void* hd, BOOL interlace){
	returnIfC1(FALSE, !hd);
	
	auto src = OBJECT_CONVERT(hd, IYuvSource);
	src->SetInterlaced(interlace);

	return TRUE;
}

EXTERNC const char* OmfYuvSrcGetMediaInfo(void* hd){
	returnIfC1(FALSE, !hd);

	auto src = OBJECT_CONVERT(hd, IYuvSource);
		
	if(src->CurrentState() == State::null){
		returnIfC1(NULL, !src->ChangeUp(State::ready));
	}

	return src->GetYuvMediaInfo().media.c_str();
}

