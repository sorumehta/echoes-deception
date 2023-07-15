//
// Created by Saurabh Mehta on 20/06/23.
//

#include "RPG_Dynamic.h"

RPG_Dynamic::RPG_Dynamic(std::string name) : sName(name), px(0), py(0), vx(0), vy(0), bSolidVsMap(true),
                                             bSolidVsDyn(true), bFriendly(true) {}


DynamicCreature::DynamicCreature(std::string name, LTexture *spr, int sprW, int sprH, int nSpritesInARow): RPG_Dynamic(name) {
    mSprite = spr;
    sprWidth = sprW;
    sprHeight = sprH;
    nSpritesInDirection = nSpritesInARow;
    health = 10;
    healthMax = 10;
    mFacingDirection = SOUTH;
    mGraphicState = STANDING;
    mTimer = 0.0f;
    mGraphicCounter = 0;
}

void DynamicCreature::drawSelf(GameEngine *engine, float fOffsetX, float fOffsetY, int nTileWidth, int nTileHeight) {
    int nSheetOffsetX = 0;
    int nSheetOffsetY = 0;

    switch (mGraphicState) {
        case WALKING:
        {
            nSheetOffsetY = mFacingDirection * sprHeight;
            nSheetOffsetX = mGraphicCounter * sprWidth;
        }
        break;
        case STANDING:
        {
            nSheetOffsetX = 0;
            nSheetOffsetY = 0;
        }
        break;
    }
    SDL_Rect clip;
    clip.x = nSheetOffsetX;
    clip.y = nSheetOffsetY;
    clip.w = sprWidth;
    clip.h = sprHeight;
    mSprite->drawTexture((px - fOffsetX)*nTileWidth, (py - fOffsetY)*nTileHeight, nTileWidth, nTileHeight, &clip);
}


void DynamicCreature::update(float fElapsedTime) {
    mTimer += fElapsedTime;
    if (mTimer >= 0.2f){
        mTimer = 0.0f;
        mGraphicCounter += 1;
        mGraphicCounter %= nSpritesInDirection;
    }
    if(std::fabs(vx) > 0 || std::fabs(vy) > 0.05){
        mGraphicState = WALKING;
    } else {
        mGraphicState = STANDING;
    }
    if(health <= 0){
        mGraphicState = DEAD;
    }
    if(vx > 0.05f){
        mFacingDirection = EAST;
    } else if(vx < -0.05f){
        mFacingDirection = WEST;
    } else if(vy > 0.05f){
        mFacingDirection = SOUTH;
    } else if (vy < -0.05f){
        mFacingDirection = NORTH;
    }
}

void Teleport::drawSelf(GameEngine *engine, float fOffsetX, float fOffsetY, int nTileWidth, int nTileHeight) {
    engine->drawRect((px - fOffsetX)*nTileWidth, (py - fOffsetY)*nTileHeight, nTileWidth, nTileHeight);
}

void Teleport::update(float fElapsedTime) {
    // does nothing
}

Teleport::Teleport(float x, float y, std::string targetMap, float tx, float ty): RPG_Dynamic("Teleport") {
    px = x;
    py = y;
    mapPosX = tx;
    mapPosY = ty;
    targetMapName = targetMap;
    bSolidVsDyn = false;
    bSolidVsMap = false;
}
