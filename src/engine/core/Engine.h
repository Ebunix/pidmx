#pragma once
#include "FixturePreset.h"
#include <vector>
#include <set>

class Engine {
public:
    std::set<Hash> activeFixtures;
    float dpiScale = 1.0;

    std::vector<FixturePreset> fixturePresets;

	Engine(int argc, char* argv[]);

    void LoadFixturePresetsFromDirectory(const std::string& dir);

    static void ProcessHotkeys();
    static void NewShow();
    static void LoadShow(const std::string& path);
    static void SaveShow(const std::string& path);
    static void Undo();
    static void Redo();

    static void DrawPanels();
};

extern Engine* globalEngine;