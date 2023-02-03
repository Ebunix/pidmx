#pragma once
#include "FixturePreset.h"
#include <vector>
#include <set>

class Engine {
public:
    std::set<Hash> activeFixtures;

	Engine();

    static void ProcessHotkeys();
    static void NewShow();
    static void LoadShow(const std::string& path);
    static void SaveShow(const std::string& path);
    static void Undo();
    static void Redo();

    static void DrawPanels();
};