#include "_chrono_base.h"
#include "OmfMain.h"
#include "OmfFrame.h"
#include "OmfAudPlayer.h"
#include "IAudioPlayer.h"

/////////////////////////////////////////////
using namespace omf;
using namespace omf::api;
using namespace omf::chrono;
using namespace omf::api::streaming;
using namespace omf::api::streaming::common;
/////////////////////////////////////////////

EXTERNC void* OmfAudPlayerCreate(){
	OmfMain::Initialize();
	OmfMain::Globle().LogConfig("all=false,err=true,warn=true,note=true");

	auto player = IAudioPlayer::CreateNew("dualos");printf("[OMF]create:%p\n",player);
	player->RegisterProcessCallback([&player](Frame& frame)->bool{
		return true;
	});
	return player;
}

EXTERNC BOOL OmfAudPlayerDestory(void* hd){printf("[OMF]destory:%p\n",hd);
	returnIfC1(FALSE, !hd);
	delete OBJECT_CONVERT(hd, IAudioPlayer);
	return TRUE;
}

EXTERNC BOOL OmfAudPlayerStatusUp(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto player = OBJECT_CONVERT(hd, IAudioPlayer);printf("[OMF]%p-state:%d\n",player,state);
	return player->ChangeUp(State(state));	
}

EXTERNC BOOL OmfAudPlayerStatusDown(void* hd, OmfStateC state){
	returnIfC1(FALSE, !hd);
	auto player = OBJECT_CONVERT(hd, IAudioPlayer);printf("[OMF]%p-state:%d\n",player,state);
	return player->ChangeDown(State(state));
}

EXTERNC BOOL OmfAudPlayerPushFrame(void* hd, OmfFrameC_t* frame){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !frame);
	auto player = OBJECT_CONVERT(hd, IAudioPlayer);
	if(player){
		if(frame->data && frame->size){
			//auto dfree = frame->free;
			//auto data = frame->data;
			//std::function<void()> func = [&data,dfree](){if(data) dfree(data);};
			std::shared_ptr<frame_t> frm{
				new frame_t{ frame->index, frame->data, frame->size, true, TimePointFromNs(frame->pts_ms), nullptr }
			};
				
			return player->PushFrame(frm, TRUE);
		}
	}
	return FALSE;
}

EXTERNC BOOL OmfAudPlayerSetSampleRate(void* hd, unsigned int samplerate){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, samplerate <= 0);

	auto player = OBJECT_CONVERT(hd, IAudioPlayer);
	return player->SetSampleRate(samplerate);
}

EXTERNC BOOL OmfAudPlayerSetChannel(void* hd, unsigned int channel){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, channel <= 0)

	auto player = OBJECT_CONVERT(hd, IAudioPlayer);
	return player->SetChannels(channel);
}

EXTERNC BOOL OmfAudPlayerSetMediaInfo(void* hd, const char* codec){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !codec);

	auto player = OBJECT_CONVERT(hd, IAudioPlayer);

	char media[32];
	sprintf(media, "codec=%s", codec);
	
	return player->SetMediaInfo(media);
}

EXTERNC BOOL OmfAudPlayerSetCodecOnLinux(void* hd, BOOL en){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !en)

	auto player = OBJECT_CONVERT(hd, IAudioPlayer);
	return player->EnableCodecOnLinux(en);
}

EXTERNC BOOL OmfAudPlayerSetLiveDelayLimit(void* hd, const char* limit){
	returnIfC1(FALSE, !hd);
	returnIfC1(FALSE, !limit)

	auto player = OBJECT_CONVERT(hd, IAudioPlayer);
	return player->EnableCodecOnLinux(limit);
}


