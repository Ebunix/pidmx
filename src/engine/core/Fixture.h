#pragma once

#include <pidmx_utils.h>
#include <memory>
#include <utility>
#include "IIdentifiable.h"
#include "NbtFileIO.h"
#include <js/js.h>

struct FixtureData: ISer {
    std::string name;
    Hash fixtureId = 0;
    Hash presetId = 0;
    int universe = 0;
    int channel = 0;

    FixtureData() = default;
    FixtureData(std::string name, Hash fixtureId, Hash presetId, int universe, int channel):
        name(std::move(name)), fixtureId(fixtureId), presetId(presetId), universe(universe), channel(channel) {}

    nbt::tag_compound Save() override;
    void Load(const nbt::tag_compound &data) override;
};

class Fixture : public IIdentifiable {
public:
    FixtureData data;

    Fixture() = default;
    explicit Fixture(FixtureData data): data(std::move(data)) {}

    nbt::tag_compound Save() override;
    void Load(const nbt::tag_compound &data) override;

#ifdef PIDMX_ENABLE_JAVASCRIPT
    static void Patch(const v8::FunctionCallbackInfo<v8::Value> &info);
#endif

};

typedef std::shared_ptr<Fixture> FixtureInstance;