#include "IIdentifiable.h"
#include "Engine.h"

IIdentifiable::IIdentifiable() {
    id = Engine::Instance().Show().GetID();
}