//
// Created by Saurabh Mehta on 20/06/23.
//
#pragma once
#include "SimpleGameEngine.h"

class RPG_Dynamic {
public:
    RPG_Dynamic(std::string name);
//    ~RPG_Dynamic();

    float px, py;
    float vx, vy;
    bool bSolidVsMap, bSolidVsDyn, bFriendly;
    std::string sName;

    virtual void drawSelf(GameEngine *engine, float fOffsetX, float fOffsetY, int nTileWidth, int nTileHeight) = 0;
    virtual void update(float fElapsedTime) = 0;
};

class DynamicCreature : public RPG_Dynamic {
protected:
    LTexture *mSprite;
    float mTimer;
    int mGraphicCounter;
    // make sure all the moving sprites follow this direction convention
    enum {SOUTH = 0, EAST = 1, NORTH = 2, WEST = 3} mFacingDirection;
    enum {STANDING, WALKING, DEAD} mGraphicState;

public:
    int sprWidth;
    int sprHeight;
    int nSpritesInDirection;
    float health, healthMax;
    DynamicCreature(std::string name, LTexture *spr, int sprW, int sprH, int nSpritesInARow);

    void drawSelf(GameEngine *engine, float fOffsetX, float fOffsetY, int nTileWidth, int nTileHeight) override;

    void update(float fElapsedTime) override;

};