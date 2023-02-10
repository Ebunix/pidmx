#pragma once

#include <pidmx_utils.h>
#include <memory>
#include <utility>
#include "IIdentifiable.h"
#include "NbtFileIO.h"
#include "FixturePreset.h"
#include <js/js.h>

struct FixtureData : ISer {
    std::string name;
    Hash fixtureId = 0;
    Hash presetId = 0;
    int universe = 0;
    int channel = 0;

    FixtureData() = default;

    FixtureData(std::string name, Hash fixtureId, Hash presetId, int universe, int channel) :
            name(std::move(name)), fixtureId(fixtureId), presetId(presetId), universe(universe), channel(channel) {}

    nbt::tag_compound Save() const override;

    void Load(const nbt::tag_compound &data) override;
};

class Fixture : public IIdentifiable {
public:
    FixtureData data;

    Fixture() = default;

    explicit Fixture(FixtureData data);

    nbt::tag_compound Save() const override;

    void Load(const nbt::tag_compound &data) override;

    bool GetParamValue(FixtureParameterType type, float *result, bool fromOutput = true) {
        if (HasParam(type)) {
            *result = fromOutput ? outputValues.at(type) : calledValues.at(type);
            return true;
        }
        return false;
    }

    void SetParamValue(FixtureParameterType type, float value, bool toOutput = true) {
        if (HasParam(type)) {
            if (toOutput) {
                outputValues.insert_or_assign(type, value);
            }
            else {
                calledValues.insert_or_assign(type, value);
            }
        }
    }

    inline bool HasParam(FixtureParameterType type) {
        return availableParameters.contains(type);
    }

    void ResetOutputToCalled() {
        for (const auto &pair : calledValues) {
            outputValues.insert_or_assign(pair.first, pair.second);
        }
    }

    bool IsAnyOutputDiverged();
    bool IsOutputDiverged(FixtureParameterType param);

    inline const Set<FixtureParameterType> &AvailableParameters() { return availableParameters; }


#ifdef PIDMX_ENABLE_JAVASCRIPT

    static void Patch(const v8::FunctionCallbackInfo<v8::Value> &info);

#endif

private:
    Map<FixtureParameterType, float> calledValues;
    Map<FixtureParameterType, float> outputValues;
    Set<FixtureParameterType> availableParameters;

};

typedef std::shared_ptr<Fixture> FixtureInstance;