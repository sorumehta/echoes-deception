//
// Created by Saurabh Mehta on 20/05/23.
//

#include "RPG_Maps.h"
#include <fstream>
#include <map>
#include <utility>

cMap::cMap() {
    nWidth = 0;
    nHeight = 0;
    aIndices = nullptr;
    aIsSolid = nullptr;
}

cMap::~cMap() {
    delete[] aIsSolid;
    delete[] aIndices;
    for (auto spr : vSprites){
        delete spr;
    }
}

int cMap::GetIndex(int x, int y) {
    if(x>=0 && x < nWidth && y>=0 && y < nHeight)
        return aIndices[y*nWidth + x];
    return 0;
}

bool cMap::GetSolid(int x, int y) {
    if(x>=0 && x < nWidth && y>=0 && y < nHeight)
        return aIsSolid[y*nWidth + x];
    return false;
}

bool cMap::Create(const std::string& fileData, std::vector<LTexture *> spriteList, const std::string& name) {
    sName = name;
    vSprites = std::move(spriteList);
    std::ifstream data(fileData, std::ios::in | std::ios::binary);
    if (data.is_open()){
        data >> nWidth >> nHeight;
        aIsSolid = new bool[nWidth * nHeight];
        aIndices = new int[nWidth * nHeight];
        for (int i=0; i < nWidth*nHeight; i++){
            data >> aIndices[i];
            data >> aIsSolid[i];
        }
        return true;
    }
    return false;
}

cMap_Village::cMap_Village() {
    std::string basePath = "../res/graphics/";
    std::string spriteMap[30];
    spriteMap[0] = basePath + "water.png";
    spriteMap[1] = basePath + "water.png";
    spriteMap[2] = basePath + "water.png";
    spriteMap[3] = basePath + "water.png";
    spriteMap[4] = basePath + "water.png";
    spriteMap[5] = basePath + "water.png";
    spriteMap[6] = basePath + "house/9.png";
    spriteMap[7] = basePath + "house/8.png";
    spriteMap[8] = basePath + "house/7.png";
    spriteMap[9] = basePath + "tree.png";
    spriteMap[10] = basePath + "water.png";
    spriteMap[11] = basePath + "water.png";
    spriteMap[12] = basePath + "water.png";
    spriteMap[13] = basePath + "water.png";
    spriteMap[14] = basePath + "grass.png";
    spriteMap[15] = basePath + "water.png";
    spriteMap[16] = basePath + "house/6.png";
    spriteMap[17] = basePath + "house/5.png";
    spriteMap[18] = basePath + "house/4.png";
    spriteMap[19] = basePath + "tree2.png";
    spriteMap[20] = basePath + "water.png";
    spriteMap[21] = basePath + "water.png";
    spriteMap[22] = basePath + "water.png";
    spriteMap[23] = basePath + "water.png";
    spriteMap[24] = basePath + "water.png";
    spriteMap[25] = basePath + "water.png";
    spriteMap[26] = basePath + "house/3.png";
    spriteMap[27] = basePath + "house/2.png";
    spriteMap[28] = basePath + "house/1.png";
    spriteMap[29] = basePath + "pave.png";

    std::vector<LTexture *> vSpritelist;
    for (const auto & spritePath : spriteMap){
        LTexture *spr = new LTexture();
        spr->loadTextureFromFile(spritePath);
        vSpritelist.emplace_back(spr);
    }

    Create("../res/village.txt",  vSpritelist,"Echotopia");
}
