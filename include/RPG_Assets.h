//
// Created by Saurabh Mehta on 20/06/23.
//
#pragma once
#include <vector>
#include <unordered_map>
#include "SimpleGameEngine.h"
#include "RPG_Maps.h"

class RPG_Assets {
public:
    static RPG_Assets& get();
    RPG_Assets(RPG_Assets const&) = delete;
    void operator=(RPG_Assets const&) = delete;

    RPG_Assets();
    ~RPG_Assets();

    LTexture *getSprite(int idx);
    cMap *getMap(std::string mapName);
    void loadSprites();
    void loadMaps();

private:
    // each sprite would have a unique index.
    std::vector<LTexture*> vSprites;
    std::unordered_map<std::string, cMap*> maps;
    std::array<std::string, 30> mSprNames;
};
