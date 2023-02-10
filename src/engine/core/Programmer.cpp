//
// Created by ebu on 08.02.23.
//

#include "Programmer.h"
#include "Engine.h"

void Programmer::AddTrackingParameter(Hash fixtureId, FixtureParameterType paramType) {
    if (trackingParametersPerFixture.contains(fixtureId)) {
        trackingParametersPerFixture.at(fixtureId).insert(paramType);
    } else {
        trackingParametersPerFixture.insert_or_assign(fixtureId, Set<FixtureParameterType>{paramType});
    }
    trackingParametersAll.insert(paramType);
}

bool Programmer::IsTracking(Hash fixtureId, FixtureParameterType paramType) {
    return trackingParametersPerFixture.contains(fixtureId) && trackingParametersPerFixture.at(fixtureId).contains(paramType);
}

void Programmer::TrackAllParameters(Hash fixtureId) {
    ShowData &show = Engine::Instance().Show();
    if (show.fixtures.contains(fixtureId)) {
        FixtureInstance instance = show.fixtures.at(fixtureId);
        for (const auto &param : instance->AvailableParameters()) {
            AddTrackingParameter(fixtureId, param);
        }
    }
}

void Programmer::SetParamAcrossFixtures(const FixtureParameterType &param, float value) {
    ShowData &show = Engine::Instance().Show();
    for (const auto &pair : trackingParametersPerFixture) {
        if (!show.fixtures.contains(pair.first) || !pair.second.contains(param)) {
            continue;
        }

        auto &fix = show.fixtures.at(pair.first);
        fix->SetParamValue(param, value);
    }
}

void Programmer::Clear() {
    ShowData &show = Engine::Instance().Show();
    for (const auto &pair : trackingParametersPerFixture) {
        if (show.fixtures.contains(pair.first)) {
            show.fixtures.at(pair.first)->ResetOutputToCalled();
        }
    }
    trackingParametersPerFixture.clear();
    trackingParametersAll.clear();
}