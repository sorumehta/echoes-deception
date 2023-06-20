#include "SimpleGameEngine.h"
#include <cmath>
#include <list>
#include <memory>
#include "RPG_Maps.h"
#include "RPG_Assets.h"

const float PI = 3.14159f;


enum FlipType {
    DOWN, RIGHT, UP, LEFT
};

class Player {
private:
    SDL_Rect spriteClips[12];
    float frame;
    LTexture texture;
public:
    FlipType flipType;

    void initSpriteClips() {
        int sprWidth = 42;
        int sprHeight = 36;
        for(int i = 0; i < 4; i++){
            for(int j=0; j < 3; j++){
                int idx = i * 3 + j;
                spriteClips[idx].x = j * sprWidth;
                spriteClips[idx].y = i * sprHeight;
                spriteClips[idx].w = sprWidth;
                spriteClips[idx].h = sprHeight;
            }
        }
    }

    Player() {
        texture.loadTextureFromFile("../res/graphics/main_guy.png", true, {0, 0xFF, 0xFF});
        initSpriteClips();
        frame = 0.0f;
        flipType = DOWN;
    }

    void drawPlayer(int x, int y, int w, int h, bool changeFrame, float fSecElapsed) {
        SDL_Rect *currentClip = nullptr;
        if (changeFrame) {
            currentClip = &spriteClips[static_cast<int>(flipType*3 + frame)];
            frame += 10 * fSecElapsed;
            if (frame >= 3.0f) {
                frame = 0.0f;
            }
        } else {
            currentClip = &spriteClips[0];
        }
        texture.drawTexture(x, y, w, h, currentClip);
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
    void
    onUserInputEvent(int eventType, const unsigned char *state, int mouseX, int mouseY, float secPerFrame) override {
        if (state[SDL_SCANCODE_UP]) {
            fPlayerVelY += -10.0f * secPerFrame;
            player->flipType = UP;
        }
        if (state[SDL_SCANCODE_DOWN]) {
            fPlayerVelY += 10.0f * secPerFrame;
            player->flipType = DOWN;
        }
        if (state[SDL_SCANCODE_LEFT]) {
            fPlayerVelX += -10.0f * secPerFrame;
            player->flipType = LEFT;
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            fPlayerVelX += 10.0f * secPerFrame;
            player->flipType = RIGHT;

        }
        if (state[SDL_SCANCODE_SPACE]) {
            if (fPlayerVelY == 0) {
                fPlayerVelY = -10.0f;
            }
        }
    }

    bool onInit() override {
        RPG_Assets::get().loadSprites();
        player = new Player();
        nTileWidth = 24;
        nTileHeight = 24;
        pCurrentMap = new cMap_Village();
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
        if (std::abs(fPlayerVelX) < 0.01f) {
            fPlayerVelX = 0.0f;
        }
        fPlayerVelY += -4.0f * fPlayerVelY * fElapsedTime;
        if (std::abs(fPlayerVelY) < 0.01f) {
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
                LTexture *texture = RPG_Assets::get().getSprite(spriteIdx);
                texture->drawTexture( static_cast<int>((x - fTileOffsetX) * nTileWidth),
                                     static_cast<int>((y - fTileOffsetY) * nTileHeight), nTileWidth, nTileHeight);
            }
        }
        // draw player
        bool bWalkingAnimation = false;
        if (std::abs(fPlayerVelX) > 0.3 || std::abs(fPlayerVelY) > 0.3) {
            bWalkingAnimation = true;
        }
        player->drawPlayer(static_cast<int>((fPlayerPosX - fOffsetX) * nTileWidth),
                           static_cast<int>((fPlayerPosY - fOffsetY) * nTileHeight), nTileWidth, nTileHeight,
                           bWalkingAnimation, fElapsedTime);

        LTexture *font = new LTexture();
        font->loadTextureFromText(getFont(), "Hello, everyone!", {0xFF, 0xFF, 0xFF});
        font->drawTexture((mWindowWidth/2 - font->getWidth())/2, mWindowHeight/2 - 20  );
        return true;
    }
};

int main() {
    Echoes echoes;
    echoes.init(30 * 24, 16 * 24, "Echoes Of Deception");
    echoes.startGameLoop();
    return 0;
}
