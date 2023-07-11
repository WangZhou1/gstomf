//
// Created by jg.wang on 2019/8/6.
//
#pragma once

#include "common/MediaInfo.h"
#include "common/IH264.h"
namespace omf {
	namespace api {
		namespace streaming {
			namespace common{
				class IH265:public virtual IH264{
				};
			}
		}
	}
}