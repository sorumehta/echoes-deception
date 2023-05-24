//
// Created by Saurabh Mehta on 20/05/23.
//
#pragma once
#include "SimpleGameEngine.hpp"
#include <list>

class cMap {
private:
    int *aIndices;
    bool *aIsSolid;
    SDL_Renderer* renderer;
public:
    int nWidth;
    int nHeight;
    std::string sName;
    std::vector<LTexture *> vSprites;
    int GetIndex(int x, int y);
    bool GetSolid(int x, int y);
    bool Create(const std::string& fileData, std::vector<LTexture *> vSpriteList, const std::string& name);
    cMap(SDL_Renderer* r);

    ~cMap();
};

class cMap_Village : public cMap {
public:
    cMap_Village(SDL_Renderer* r);
};

