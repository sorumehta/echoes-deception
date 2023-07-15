//
// Created by Saurabh Mehta on 20/06/23.
//

#include "RPG_Dynamic.h"
#include "RPG_Assets.h"
#include "RPG_Maps.h"
#include "PathFinder.h"

RPG_Dynamic::RPG_Dynamic(std::string name) : sName(name), px(0), py(0), vx(0), vy(0), bSolidVsMap(true),
                                             bSolidVsDyn(true), bFriendly(true) {}

void RPG_Dynamic::update(float fElapsedTime, RPG_Dynamic *player,cMap *map ) {

}


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


void DynamicCreature::update(float fElapsedTime, RPG_Dynamic *player,cMap *map ) {
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

    behaviour(fElapsedTime, player, map);
}

void DynamicCreature::behaviour(float fElapsedTime, RPG_Dynamic *player, cMap *map ) {
    // do nothing by default
}

void Teleport::drawSelf(GameEngine *engine, float fOffsetX, float fOffsetY, int nTileWidth, int nTileHeight) {
    engine->drawRect((px - fOffsetX)*nTileWidth, (py - fOffsetY)*nTileHeight, nTileWidth, nTileHeight);
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

DynamicCreatureEnemy::DynamicCreatureEnemy()
        : DynamicCreature("Enemy", ASSETS.getSprite(3), 32, 32, 4) {
    bFriendly = false;
    pathFinder = nullptr;
    stateTick = 2.0f; // enemy makes a decision every these seconds
    pathTick = 0.5f;
    pathIdx = 0;
}

void DynamicCreatureEnemy::behaviour(float fElapsedTime, RPG_Dynamic *player,cMap *map) {
    if(!pathFinder && map){
        pathFinder = new PathFinder(map);
    }
    stateTick -= fElapsedTime;
    pathTick -= fElapsedTime;
    if (stateTick <= 0){
        pathToFollow.clear();
        pathToFollow = pathFinder->solveAStar(px, py, player->px, player->py);
        for (auto p : pathToFollow){
            std::cout << p.first << "," << p.second << "  ";
        }
        std::cout << std::endl;
        stateTick += 2.0f;
        pathTick = 0.5f;
        pathIdx = 0;
    }
    else{
        // we need pathTick counter because we need some time for change in velocity to take effect
        if(pathTick <= 0 && !pathToFollow.empty() && pathIdx < pathToFollow.size()){
            std::pair<int, int> targetCoord = pathToFollow[pathIdx];
            // set the position to the expected position
            float fTargetX = targetCoord.first - px;
            float fTargetY = targetCoord.second - py;
            vx = (fTargetX) * 4.0f;
            vy = (fTargetY) * 4.0f;
            pathIdx++;
            pathTick += 0.5f;
        }
    }
}
