#pragma once
#include "../v8.h"

namespace js {
	namespace impl {
		void require(const v8::FunctionCallbackInfo<v8::Value>& info);
	}
}
