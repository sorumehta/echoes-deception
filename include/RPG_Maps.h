//
// Created by Saurabh Mehta on 20/05/23.
//
#pragma once
#include <list>
#include <array>
#include <string>
#include "SimpleGameEngine.h"
#include "RPG_Dynamic.h"
#include "RPG_Commands.h"

class cMap {
    // the sprites need to be referenced by indices. So the map has to contain the indices of sprites
    // at the required position
private:
    int *aIndices;
    bool *aIsSolid;

public:
    enum NATURE {
        TALK,
        WALK
    };
public:
    int nWidth;
    int nHeight;
    std::string sName;
    static ScriptProcessor *g_scriptProcessor;
    cMap();
    int GetIndex(int x, int y);
    bool GetSolid(int x, int y);
    bool Create(const std::string& fileData, const std::string& name);
    virtual bool PopulateDynamics(std::vector<RPG_Dynamic *> &vecDynamics);
    virtual bool onInteraction(std::vector<RPG_Dynamic *> &vecDynObjs, RPG_Dynamic *target, NATURE nature);
    ~cMap();
};

class cMap_Village : public cMap {
public:
    cMap_Village();

    bool PopulateDynamics(std::vector<RPG_Dynamic *> &vecDynamics) override;

    bool onInteraction(std::vector<RPG_Dynamic *> &vecDynObjs, RPG_Dynamic *target, NATURE nature) override;
};

class cMap_Home : public cMap {
public:
    cMap_Home();
    bool onInteraction(std::vector<RPG_Dynamic *> &vecDynObjs, RPG_Dynamic *target, NATURE nature) override;
};