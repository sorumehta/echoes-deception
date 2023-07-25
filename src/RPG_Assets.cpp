//
// Created by Saurabh Mehta on 20/06/23.
//

#include "RPG_Assets.h"


RPG_Assets &RPG_Assets::get() {

    static RPG_Assets me;
    return me;

}

RPG_Assets::RPG_Assets() {
    mSprNames[0] = "wall"; // to be added
    mSprNames[1] = "floor"; // to be added
    mSprNames[2] = "main_guy";
    mSprNames[3] = "village_man";
    mSprNames[4] = "village_woman";
    mSprNames[5] = "void";
    mSprNames[6] = "house/9";
    mSprNames[7] = "house/8";
    mSprNames[8] = "house/7";
    mSprNames[9] = "tree";
    mSprNames[10] = "balloon_red";
    mSprNames[11] = "balloon_black";
    mSprNames[12] = "balloon_green";
    mSprNames[13] = "balloon_blue";
    mSprNames[14] = "grass";
    mSprNames[16] = "house/6";
    mSprNames[17] = "house/5";
    mSprNames[18] = "house/4";
    mSprNames[19] = "tree2";
    mSprNames[26] = "house/3";
    mSprNames[27] = "house/2";
    mSprNames[28] = "house/1";
    mSprNames[29] = "pave";
}

RPG_Assets::~RPG_Assets() = default;



void RPG_Assets::loadSprites() {

    std::string basePath = "../res/graphics/";

    LTexture *defaultTexture = new LTexture();
    defaultTexture->loadTextureFromFile(basePath + "water.png");
    for (const auto & spriteName : mSprNames){
        if(spriteName.empty()){
            vSprites.emplace_back(defaultTexture);
        }
        else{
            LTexture *texture = new LTexture();
            std::string filePath = basePath + spriteName + ".png";
            texture->loadTextureFromFile(filePath);
            vSprites.emplace_back(texture);
        }

    }

}

LTexture *RPG_Assets::getSprite(int idx) {
    return vSprites[idx];
}

void RPG_Assets::loadMaps() {
    auto load = [&](cMap *m){
        maps[m->sName] = m;
    };
    load(new cMap_Village());
    load(new cMap_Home());
    load(new Victory());
}

cMap *RPG_Assets::getMap(std::string mapName) {
    return maps[mapName];
}
