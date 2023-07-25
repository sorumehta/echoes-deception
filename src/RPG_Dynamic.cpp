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


void Balloon::drawSelf(GameEngine *engine, float fOffsetX, float fOffsetY, int nTileWidth, int nTileHeight) {
    mSprite->drawTexture((px - fOffsetX)*nTileWidth, (py - fOffsetY)*nTileHeight, nTileWidth, nTileHeight);
}



Balloon::Balloon(float x, float y, BalloonType balloonType): RPG_Dynamic("Balloon") {
    px = x;
    py = y;
    type = balloonType;
    mSprite = ASSETS.getSprite(balloonType);
    bSolidVsDyn = false;
}

DynamicCreatureEnemy::DynamicCreatureEnemy(EnemyType enemyType, std::string name)
        : DynamicCreature(name, ASSETS.getSprite(enemyType), 32, 32, 4) {
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
    float fDiffX = player->px - px;
    float fDiffY = player->py - py;
    float fDistance = std::sqrtf(fDiffX*fDiffX + fDiffY*fDiffY);
    if(fDistance < 4.0f){
        vx = (fDiffX / fDistance) * 2.0f;
        vy = (fDiffY / fDistance) * 2.0f;
    } else if (stateTick <= 0){
        pathToFollow.clear();
        pathToFollow = pathFinder->solveAStar(px, py, player->px, player->py);
        stateTick = 2.0f;
        pathTick = 0.5f;
        pathIdx = 0;
    }
    // we need pathTick counter because we need some time for change in velocity to take effect
    else if(pathTick <= 0 && !pathToFollow.empty() && pathIdx < pathToFollow.size()){
            std::pair<int, int> targetCoord = pathToFollow[pathIdx];
            // set the position to the expected position
            float fTargetX = targetCoord.first - px;
            float fTargetY = targetCoord.second - py;
            float fTargetDist = std::sqrtf(fTargetX*fTargetX + fTargetY*fTargetY);
            vx = (fTargetX/fTargetDist) * 3.5f;
            vy = (fTargetY/fTargetDist) * 3.5f;
            pathIdx++;
            pathTick += 0.2f;
        }

}
