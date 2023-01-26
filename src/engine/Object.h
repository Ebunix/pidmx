#pragma once
#include <string>
#include "EngineCereal.h"

class EngineObject {
public:
	std::string name;
	uint64_t objId;
};

void RegisterObject(EngineObject* obj);
void UnregisterObject(EngineObject* obj);
