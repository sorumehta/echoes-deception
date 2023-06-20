//
// Created by Saurabh Mehta on 20/06/23.
//

#include "RPG_Assets.h"


RPG_Assets &RPG_Assets::get() {

    static RPG_Assets me;
    return me;

}

RPG_Assets::RPG_Assets() {
    mSprNames[6] = "house/9";
    mSprNames[7] = "house/8";
    mSprNames[8] = "house/7";
    mSprNames[9] = "tree";
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

RPG_Assets::~RPG_Assets() {
    for (auto spr : vSprites){
        delete spr;
    }
}



void RPG_Assets::loadSprites() {

    std::string basePath = "../res/graphics/";
    for (const auto & spriteName : mSprNames){
        LTexture *texture = new LTexture();
        std::string filePath;
        if(spriteName.empty()){
            filePath = basePath + "default.png";
        }
        else{
            filePath = basePath + spriteName + ".png";
        }
        texture->loadTextureFromFile(filePath);
        vSprites.emplace_back(texture);
    }

}

LTexture *RPG_Assets::getSprite(int idx) {
    return vSprites[idx];
}
