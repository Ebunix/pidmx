#include "Object.h"
#include <map>

std::map<uint64_t, EngineObject*> objects; 

void RegisterObject(EngineObject *obj) {
	objects.insert_or_assign(obj->id, obj);
}

void UnregisterObject(EngineObject *obj) {
	auto index = objects.find(obj->id);
	if (index != objects.end()) {
		objects.erase(index);
	}
}
