#pragma once
#include "pidmx_utils.h"
#include "FixturePreset.h"

class Programmer {
public:
    void AddTrackingParameter(Hash fixtureId, FixtureParameterType paramType);
    void TrackAllParameters(Hash fixtureId);
    bool IsTracking(Hash fixtureId, FixtureParameterType paramType);
    inline bool IsTrackingAnything() { return trackingParametersPerFixture.size() > 0; }
    void Clear();
    inline const Map<Hash, Set<FixtureParameterType>> GetTracking() { return trackingParametersPerFixture; }
    inline const Set<FixtureParameterType> GetTrackingParameters() { return trackingParametersAll; }
    void SetParamAcrossFixtures(const FixtureParameterType &param, float value);

private:
    Map<Hash, Set<FixtureParameterType>> trackingParametersPerFixture;
    Set<FixtureParameterType> trackingParametersAll;
};