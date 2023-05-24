#include "SimpleGameEngine.hpp"
#include <cmath>
#include <list>
#include <memory>
#include "RPG_Maps.h"

const float PI = 3.14159f;

class Player {
private:

    SDL_Rect spriteClips[4];
    float frame;
    LTexture texture;
    SDL_Renderer *renderer = NULL;
public:
    SDL_RendererFlip flipType;
    void initSpriteClips() {
        spriteClips[0].x = 0;
        spriteClips[0].y = 0;
        spriteClips[0].w = 64;
        spriteClips[0].h = 205;

        spriteClips[1].x = 64;
        spriteClips[1].y = 0;
        spriteClips[1].w = 64;
        spriteClips[1].h = 205;

        spriteClips[2].x = 128;
        spriteClips[2].y = 0;
        spriteClips[2].w = 64;
        spriteClips[2].h = 205;

        spriteClips[3].x = 192;
        spriteClips[3].y = 0;
        spriteClips[3].w = 64;
        spriteClips[3].h = 205;
    }

    Player(SDL_Renderer *renderer): renderer(renderer) {
        texture.loadTextureFromFile(renderer, "../res/graphics/man.png", true, {0, 0xFF, 0xFF});
        initSpriteClips();
        frame = 0.0f;
        flipType = SDL_FLIP_NONE;
    }

    void drawPlayer(int x, int y, int w, int h, bool changeFrame, float fSecElapsed) {
        SDL_Rect *currentClip = nullptr;
        if (changeFrame) {
            currentClip = &spriteClips[static_cast<int>(frame)];
            frame += 10*fSecElapsed;
            if (frame >= 4.0f) {
                frame = 0.0f;
            }
        } else {
            currentClip = &spriteClips[0];
        }
        texture.drawTexture(renderer, x, y, w, h, currentClip, 0,
                            NULL,
                            flipType);

    }
};

class Echoes : public GameEngine {
private:
    cMap *pCurrentMap = nullptr;
    Player *player = nullptr;
    // positions in tiles space
    float fCameraPosX = 0.0f;
    float fCameraPosY = 0.0f;
    float fPlayerPosX = 10.0f;
    float fPlayerPosY = 10.0f;
    float fPlayerVelX{};
    float fPlayerVelY{};
    int nTileWidth{};
    int nTileHeight{};

public:
    void onUserInputEvent(int eventType, const unsigned char *state, int mouseX, int mouseY, float secPerFrame) override {
            if (state[SDL_SCANCODE_UP]) {
                fPlayerVelY += -10.0f * secPerFrame;
            }
            if (state[SDL_SCANCODE_DOWN]) {
                fPlayerVelY += 10.0f * secPerFrame;
            }
            if (state[SDL_SCANCODE_LEFT]) {
                fPlayerVelX += -10.0f * secPerFrame;
                player->flipType = SDL_FLIP_NONE;
            }
            if (state[SDL_SCANCODE_RIGHT]) {
                fPlayerVelX += 10.0f * secPerFrame;
                player->flipType = SDL_FLIP_HORIZONTAL;

            }
            if (state[SDL_SCANCODE_SPACE]) {
                if (fPlayerVelY == 0) {
                    fPlayerVelY = -10.0f;
                }

            }

    }

    bool onInit() override {
        player = new Player(mRenderer);
        nTileWidth = 24;
        nTileHeight = 24;
        pCurrentMap = new cMap_Village(mRenderer);
        return true;
    }


    bool onFrameUpdate(float fElapsedTime) override {
        // clamp velocities
        if (fPlayerVelY > 10) {
            fPlayerVelY = 10.0f;
        }
        if (fPlayerVelY < -10) {
            fPlayerVelY = -10.0f;
        }
        if (fPlayerVelX > 10) {
            fPlayerVelX = 10.0f;
        }
        if (fPlayerVelX < -10) {
            fPlayerVelX = -10.0f;
        }

        float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
        float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

        // resolve collision along X axis, if any
        if (fPlayerVelX < 0) {
            if (pCurrentMap->GetSolid(static_cast<int>(fNewPlayerPosX), static_cast<int>(fPlayerPosY)) ||
                pCurrentMap->GetSolid(static_cast<int>(fNewPlayerPosX), static_cast<int>(fPlayerPosY + 0.9))) {
                // cast the new position to an integer and shift by 1 so that the player is on the boundary
                // of the colliding tile, instead of leaving some space
                fNewPlayerPosX = static_cast<int>(fNewPlayerPosX) + 1;
                fPlayerVelX = 0;
            }
        } else if (fPlayerVelX > 0) {
            if (pCurrentMap->GetSolid(static_cast<int>(fNewPlayerPosX + 1), static_cast<int>(fPlayerPosY)) ||
                pCurrentMap->GetSolid(static_cast<int>(fNewPlayerPosX + 1), static_cast<int>(fPlayerPosY + 0.9))) {
                fNewPlayerPosX = static_cast<int>(fNewPlayerPosX);
                fPlayerVelX = 0;
            }
        }
        // check collision along y
        if (fPlayerVelY < 0) {
            if (pCurrentMap->GetSolid(static_cast<int>(fNewPlayerPosX), static_cast<int>(fNewPlayerPosY)) ||
                pCurrentMap->GetSolid(static_cast<int>(fNewPlayerPosX + 0.9), static_cast<int>(fNewPlayerPosY))) {
                fNewPlayerPosY = static_cast<int>(fNewPlayerPosY) + 1;
                fPlayerVelY = 0;
            }
        } else if (fPlayerVelY > 0) {
            if (pCurrentMap->GetSolid(static_cast<int>(fNewPlayerPosX), static_cast<int>(fNewPlayerPosY + 1)) ||
                pCurrentMap->GetSolid(static_cast<int>(fNewPlayerPosX + 0.9), static_cast<int>(fNewPlayerPosY + 1))) {
                fNewPlayerPosY = static_cast<int>(fNewPlayerPosY);
                fPlayerVelY = 0;
            }
        }

        fPlayerPosX = fNewPlayerPosX;
        fPlayerPosY = fNewPlayerPosY;

        // apply friction
        fPlayerVelX += -4.0f * fPlayerVelX * fElapsedTime;
        if(std::abs(fPlayerVelX) < 0.01f){
            fPlayerVelX = 0.0f;
        }
        fPlayerVelY += -4.0f * fPlayerVelY * fElapsedTime;
        if(std::abs(fPlayerVelY) < 0.01f){
            fPlayerVelY = 0.0f;
        }

        fCameraPosX = fPlayerPosX;
        fCameraPosY = fPlayerPosY;
        // Draw Level
        int nVisibleTilesX = mWindowWidth / nTileWidth;
        int nVisibleTilesY = mWindowHeight / nTileHeight;
        // get the top left corner (in tiles space) to be shown on screen
        // camera posX and posY are also in tile space
        float fOffsetX = fCameraPosX - static_cast<float>(nVisibleTilesX) / 2.0f;
        float fOffsetY = fCameraPosY - static_cast<float>(nVisibleTilesY) / 2.0f;
        // clamp
        if (fOffsetX < 0) fOffsetX = 0;
        if (fOffsetY < 0) fOffsetY = 0;
        if (fOffsetX > static_cast<float>(pCurrentMap->nWidth - nVisibleTilesX))
            fOffsetX = static_cast<float>(pCurrentMap->nWidth - nVisibleTilesX);
        if (fOffsetY > static_cast<float>(pCurrentMap->nHeight - nVisibleTilesY))
            fOffsetY = static_cast<float>(pCurrentMap->nHeight - nVisibleTilesY);

        float fTileOffsetX = fOffsetX - static_cast<int>(fOffsetX);
        float fTileOffsetY = fOffsetY - static_cast<int>(fOffsetY);

        // draw each tile
        // we overdraw on the corners to avoid distortion (hacky)
        for (int x = -1; x < nVisibleTilesX + 1; x++) {
            for (int y = -1; y < nVisibleTilesY + 1; y++) {
                int spriteIdx = pCurrentMap->GetIndex(x + static_cast<int>(fOffsetX), y + static_cast<int>(fOffsetY));
                LTexture *texture = pCurrentMap->vSprites[spriteIdx];
                texture->drawTexture(mRenderer, static_cast<int>((x - fTileOffsetX) * nTileWidth),
                                     static_cast<int>((y - fTileOffsetY) * nTileHeight), nTileWidth, nTileHeight);
            }
        }
        // draw player
        bool bWalkingAnimation = false;
        if (std::abs(fPlayerVelX) > 0.3 || std::abs(fPlayerVelY) > 0.3){
            bWalkingAnimation = true;
        }
        player->drawPlayer(static_cast<int>((fPlayerPosX - fOffsetX) * nTileWidth),
                          static_cast<int>((fPlayerPosY - fOffsetY) * nTileHeight), nTileWidth, nTileHeight, bWalkingAnimation, fElapsedTime);

        return true;
    }

};

int main() {
    Echoes echoes;
    echoes.constructConsole(30 * 24, 16 * 24, "Echoes Of Deception");
    echoes.startGameLoop();
    return 0;
}
