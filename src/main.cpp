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
    int nTileWidth;
    int nTileHeight;

public:
    void onUserInputEvent(int eventType, int button, int mousePosX, int mousePosY, float secPerFrame) {
        if (eventType == SDL_KEYDOWN) {
            if (button == SDLK_UP) {
                fPlayerVelY = -16.0f;
            }
            if (button == SDLK_DOWN) {
                fPlayerVelY = 16.0f;
            }
            if (button == SDLK_LEFT) {
                fPlayerVelX = -16.0f;
            }
            if (button == SDLK_RIGHT) {
                fPlayerVelX = 16.0f;
            }
        }
    }

    bool onInit() override {
        auto onUserInputFn = [this](int eventType, int buttonCode, int mousePosX, int mousePosY, float secPerFrame) {
            onUserInputEvent(eventType, buttonCode, mousePosX, mousePosY, secPerFrame);
        };
        InputEventHandler::addCallback("onUserInputFn_Game", onUserInputFn);
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


        fPlayerPosX += fPlayerVelX * fElapsedTime;
        fPlayerPosY += fPlayerVelY * fElapsedTime;
        fPlayerVelX = 0;
        fPlayerVelY = 0;
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

        // draw each tile
        for (int x = 0; x < nVisibleTilesX; x++) {
            for (int y = 0; y < nVisibleTilesY; y++) {
                char sTileId = GetTile(x + static_cast<int>(fOffsetX), y + static_cast<int>(fOffsetY));
                switch (sTileId) {
                    case '.':
                        fillRect(x * nTileWidth, y * nTileHeight, nTileWidth, nTileHeight, {0, 0xFF, 0xFF});
                        break;
                    case '#':
                        fillRect(x * nTileWidth, y * nTileHeight, nTileWidth, nTileHeight, {0xFF, 0xFF, 0});
                        break;
                    default:
                        break;
                }
            }
        }
        // draw player
        fillRect(static_cast<int>(fPlayerPosX - fOffsetX )*nTileWidth , static_cast<int>(fPlayerPosY - fOffsetY )*nTileHeight , nTileWidth, nTileHeight);

        return true;
    }

};

int main() {
    Echoes echoes;
    echoes.constructConsole(30*24, 16 * 24, "Echoes Of Deception");
    echoes.startGameLoop();
    return 0;
}
