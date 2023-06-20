//
// Created by Saurabh Mehta on 20/05/23.
//
#pragma once
#include "SimpleGameEngine.h"
#include <list>
#include <array>
#include <string>

class cMap {
    // the sprites need to be referenced by indices. So the map has to contain the indices of sprites
    // at the required position
private:
    int *aIndices;
    bool *aIsSolid;
public:
    int nWidth;
    int nHeight;
    std::string sName;
    int GetIndex(int x, int y);
    bool GetSolid(int x, int y);
    bool Create(const std::string& fileData, const std::string& name);
    cMap();

    ~cMap();
};

class cMap_Village : public cMap {
public:
    cMap_Village();
};

