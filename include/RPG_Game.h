//
// Created by Saurabh Mehta on 13/07/23.
//
#pragma once

#include <cmath>
#include <memory>
#include "SimpleGameEngine.h"
#include "RPG_Assets.h"
#include "RPG_Dynamic.h"
#include "RPG_Commands.h"
#include "PathFinder.h"


class RPG_Game : public GameEngine {
private:
    cMap *pCurrentMap = nullptr;
    DynamicCreature *player = nullptr;
    // positions in tiles space
    float fCameraPosX = 0.0f;
    float fCameraPosY = 0.0f;
    int nTileWidth{};
    int nTileHeight{};
    ScriptProcessor mScript;
    std::vector<RPG_Dynamic *> mVecDynamics;
    bool gameStarted;
    bool playerOnRun;
    float totalTimeElapsed;
    float enemyVelocity = 4.0f;
    std::pair<int, int> homeLocation = std::make_pair(12, 6);
    std::vector<std::string> vecDialogToShow;
    bool bShowDialog = false;
    float fDialogX = 0.0f;
    float fDialogY = 0.0f;
public:
    void handleInputEvent(int eventType, int keyCode, float fElapsedTime) override;

    void handleInputState(const unsigned char *state, int mouseX, int mouseY, float secPerFrame) override;

    bool onInit() override;

    bool onFrameUpdate(float fElapsedTime) override;

    void changeMap(std::string mapName, float x, float y);

    void showDialog(std::vector<std::string> vecLines);

    void displayDialog(std::vector<std::string> vecText, int x, int y);
    RPG_Dynamic * findObjectByName (std::vector<RPG_Dynamic *> vectorDyns, const char *name);

    RPG_Dynamic *findObjectByName(RPG_Dynamic *vector1, const char *string);
};