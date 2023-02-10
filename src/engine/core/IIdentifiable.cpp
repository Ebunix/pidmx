#include "IIdentifiable.h"
#include "Engine.h"

IIdentifiable::IIdentifiable() {
    id = Engine::Instance().Show().GetID();
}

void IIdentifiable::Load(const nbt::tag_compound &c) {
    id = nbt::Deserialize(c, "id", INVALID_HASH);
    if (id == INVALID_HASH) {
        id = Engine::Instance().Show().GetID();
    }
}
