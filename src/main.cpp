#include "SimpleGameEngine.hpp"
#include <cmath>
#include <list>
#include <memory>

const float PI = 3.14159f;

class Echoes : public GameEngine {
private:
    std::string sLevel;
    int nLevelWidth{};
    int nLevelHeight{};
    // positions in tiles space
    float fCameraPosX = 0.0f;
    float fCameraPosY = 0.0f;
    float fPlayerPosX{};
    float fPlayerPosY{};
    float fPlayerVelX{};
    float fPlayerVelY{};
    int nTileWidth{};
    int nTileHeight{};
    bool bPlayerOnGround{};

public:
    void onUserInputEvent(int eventType, int button, int mouseX, int mouseY, float secPerFrame) override {
        if (eventType == SDL_KEYDOWN) {
            if (button == SDLK_UP) {
                fPlayerVelY += -10.0f * secPerFrame;
            }
            if (button == SDLK_DOWN) {
                fPlayerVelY += 10.0f * secPerFrame;
            }
            if (button == SDLK_LEFT) {
                fPlayerVelX += -10.0f * secPerFrame;
            }
            if (button == SDLK_RIGHT) {
                fPlayerVelX += 10.0f * secPerFrame;
            }
            if (button == SDLK_SPACE) {
                if (fPlayerVelY == 0) {
                    fPlayerVelY = -10.0f;
                }

            }
        }
    }

    bool onInit() override {

        nLevelWidth = 64;
        nLevelHeight = 16;
        nTileWidth = 24;
        nTileHeight = 24;


        sLevel += "................................................................";
        sLevel += "................................................................";
        sLevel += "................................................................";
        sLevel += "................................................................";
        sLevel += "................................................................";
        sLevel += ".................####......#.#..................................";
        sLevel += "...............##........#....#.................................";
        sLevel += "................................................................";
        sLevel += "###################################.############...#############";
        sLevel += "..................................#.#............##.............";
        sLevel += "..............................#####.#..........##...............";
        sLevel += "..............................#.....#........##.................";
        sLevel += "..............................#.#####......##...................";
        sLevel += "..............................#..........##.....................";
        sLevel += "..............................##########........................";
        sLevel += "................................................................";


        return true;
    }

    bool onFrameUpdate(float fElapsedTime) override {
        // utility lambdas
        auto GetTile = [&](int x, int y) -> char {
            if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight) {
                return sLevel[y * nLevelWidth + x];
            }
            return ' ';
        };

//        auto SetTile = [&](int x, int y, char c) {
//            if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight) {
//                sLevel[y * nLevelWidth + x] = c;
//            }
//        };

        // Gravity
        fPlayerVelY += 20.0f * fElapsedTime;

        // clamp velocities
        if (fPlayerVelY > 100) {
            fPlayerVelY = 100.0f;
        }
        if (fPlayerVelY < -100) {
            fPlayerVelY = -100.0f;
        }
        if (fPlayerVelX > 10) {
            fPlayerVelX = 10.0f;
        }
        if (fPlayerVelX < -10) {
            fPlayerVelX = -10.0f;
        }

        float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
        float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

        bPlayerOnGround = false;
        // resolve collision along X axis, if any
        if (fPlayerVelX < 0) {
            if (GetTile(static_cast<int>(fNewPlayerPosX), static_cast<int>(fPlayerPosY)) != '.' ||
                GetTile(static_cast<int>(fNewPlayerPosX), static_cast<int>(fPlayerPosY + 0.9)) != '.') {
                // cast the new position to an integer and shift by 1 so that the player is on the boundary
                // of the colliding tile, instead of leaving some space
                fNewPlayerPosX = static_cast<int>(fNewPlayerPosX) + 1;
                fPlayerVelX = 0;
            }
        } else if (fPlayerVelX > 0) {
            if (GetTile(static_cast<int>(fNewPlayerPosX + 1), static_cast<int>(fPlayerPosY)) != '.' ||
                GetTile(static_cast<int>(fNewPlayerPosX + 1), static_cast<int>(fPlayerPosY + 0.9)) != '.') {
                fNewPlayerPosX = static_cast<int>(fNewPlayerPosX);
                fPlayerVelX = 0;
            }
        }
        // check collision along y
        if (fPlayerVelY < 0) {
            if (GetTile(static_cast<int>(fNewPlayerPosX), static_cast<int>(fNewPlayerPosY)) != '.' ||
                GetTile(static_cast<int>(fNewPlayerPosX + 0.9), static_cast<int>(fNewPlayerPosY)) != '.') {
                fNewPlayerPosY = static_cast<int>(fNewPlayerPosY) + 1;
                fPlayerVelY = 0;
            }
        } else if (fPlayerVelY > 0) {
            if (GetTile(static_cast<int>(fNewPlayerPosX), static_cast<int>(fNewPlayerPosY + 1)) != '.' ||
                GetTile(static_cast<int>(fNewPlayerPosX + 0.9), static_cast<int>(fNewPlayerPosY + 1)) != '.') {
                fNewPlayerPosY = static_cast<int>(fNewPlayerPosY);
                fPlayerVelY = 0;
                bPlayerOnGround = true;
            }
        }


        fPlayerPosX = fNewPlayerPosX;
        fPlayerPosY = fNewPlayerPosY;

        // apply drag if player on ground
        if (bPlayerOnGround) {
            fPlayerVelX += -2.0f * fPlayerVelX * fElapsedTime;
            if(std::abs(fPlayerVelX) < 0.01f){
                fPlayerVelX = 0.0f;
            }
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
        if (fOffsetX > static_cast<float>(nLevelWidth - nVisibleTilesX))
            fOffsetX = static_cast<float>(nLevelWidth - nVisibleTilesX);
        if (fOffsetY > static_cast<float>(nLevelHeight - nVisibleTilesY))
            fOffsetY = static_cast<float>(nLevelHeight - nVisibleTilesY);

        float fTileOffsetX = fOffsetX - static_cast<int>(fOffsetX);
        float fTileOffsetY = fOffsetY - static_cast<int>(fOffsetY);

        // draw each tile
        // we overdraw on the corners to avoid distortion (hacky)
        for (int x = -1; x < nVisibleTilesX + 1; x++) {
            for (int y = -1; y < nVisibleTilesY + 1; y++) {
                char sTileId = GetTile(x + static_cast<int>(fOffsetX), y + static_cast<int>(fOffsetY));
                switch (sTileId) {
                    case '.':
                        fillRect(static_cast<int>((x - fTileOffsetX) * nTileWidth),
                                 static_cast<int>((y - fTileOffsetY) * nTileHeight), nTileWidth, nTileHeight,
                                 {0, 0xFF, 0xFF});
                        break;
                    case '#':
                        fillRect(static_cast<int>((x - fTileOffsetX) * nTileWidth),
                                 static_cast<int>((y - fTileOffsetY) * nTileHeight), nTileWidth, nTileHeight,
                                 {0xFF, 0xFF, 0});
                        break;
                    default:
                        break;
                }
            }
        }
        // draw player
        fillRect(static_cast<int>((fPlayerPosX - fOffsetX) * nTileWidth),
                 static_cast<int>((fPlayerPosY - fOffsetY) * nTileHeight), nTileWidth, nTileHeight);

        return true;
    }

};

int main() {
    Echoes echoes;
    echoes.constructConsole(30 * 24, 16 * 24, "Echoes Of Deception");
    echoes.startGameLoop();
    return 0;
}
