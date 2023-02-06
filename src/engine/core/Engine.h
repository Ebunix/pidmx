#pragma once

#include "FixturePreset.h"
#include "ShowData.h"
#include "EngineAction.h"
#include <vector>
#include <set>

class Engine;

extern Engine *engineInstance;

class Engine {
public:
    static const uint32_t VERSION = VERSION_CODE(0, 0, 1);
    EngineAction action;

    IDSet activeFixtures;
    IDSet activeGroups;
    Set<FixtureParameterType> availableParametersOnFixtures;

    Set<FixturePreset> fixturePresets;

    float dpiScale;

    Engine(int argc, char *argv[]);

    inline static Engine &Instance() { return *engineInstance; }

    void LoadFixturePresetsFromDirectory(const std::string &dir);

    void ProcessHotkeys();

    void NewShow();

    void LoadShow(const std::string &path);

    void SaveShow(const std::string &path);

    void Undo();

    void Redo();

    void SetAction(EngineAction action);

    void Clear();

    void SetActiveFixtures(const IDSet &fixtures);

    void AddActiveGroup(Hash id);

    ShowData& Show() { return *currentShow; }

private:
    ShowData* currentShow = nullptr;

    void UpdateAvailableParameters();
};

