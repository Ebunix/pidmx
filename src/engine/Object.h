#pragma once
#include <string>

class EngineObject {
public:
	std::string name;
	uint64_t id;
};

void RegisterObject(EngineObject* obj);
void UnregisterObject(EngineObject* obj);
