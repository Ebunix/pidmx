#pragma once

#include "FixturePreset.h"
#include <vector>
#include <set>

enum EngineAction {
    EngineAction_None,
    EngineAction_Store,
    EngineAction_Delete,
};

class Engine;

extern Engine *engineInstance;

class Engine {
public:
    static const uint32_t VERSION = VERSION_CODE(0, 0, 1);
    EngineAction action;

    std::set<Hash> activeFixtures;
    std::set<Hash> activeGroups;
    std::set<FixtureParameterType> availableParametersOnFixtures;
    float dpiScale;

    std::vector<FixturePreset> fixturePresets;

    Engine(int argc, char *argv[]);

    inline static Engine &Instance() { return *engineInstance; }

    void LoadFixturePresetsFromDirectory(const std::string &dir);

    static void ProcessHotkeys();

    static void NewShow();

    static void LoadShow(const std::string &path);

    static void SaveShow(const std::string &path);

    static void Undo();

    static void Redo();

    void SetAction(EngineAction action);

    void Clear();

    void SetActiveFixtures(const std::set<Hash> &fixtures);

private:

    void UpdateAvailableParameters();
};

