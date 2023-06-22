#include "SimpleGameEngine.h"
#include <cmath>
#include <memory>
#include "RPG_Maps.h"
#include "RPG_Assets.h"
#include "RPG_Dynamic.h"
#include "RPG_Commands.h"
#include "PathFinder.h"

#define ASSETS RPG_Assets::get()

class RPG : public GameEngine {
private:
    cMap *pCurrentMap = nullptr;
    RPG_Dynamic *player = nullptr;
    // positions in tiles space
    float fCameraPosX = 0.0f;
    float fCameraPosY = 0.0f;
    int nTileWidth{};
    int nTileHeight{};
    ScriptProcessor mScript;
    std::vector<RPG_Dynamic *> mVecDynamics;
    bool gameStarted;
    bool playerOnRun;
    float totalTimeElapsed;
    bool bGameOver;
    float enemyVelocity = 7.0f;
    PathFinder *pathFinder;
    std::pair<int, int> homeLocation = std::make_pair(12, 6);
public:
    void handleInputEvent(int eventType, int keyCode, float fElapsedTime) override {
        if (eventType == SDL_KEYDOWN) {
            if (keyCode == SDLK_SPACE) {
                if (!mScript.bUserControlEnabled && !bGameOver) {
                    mScript.completeCommand();
                }
            }
            if (keyCode == SDLK_s) {
                gameStarted = true;
                mScript.addCommand(new Command_ShowDialog({"Welcome.", "You have a secret meeting with Bhai","At the back of your house"}, {0xFF, 0, 0}));
                mScript.addCommand(new Command_MoveTo(player, 6, 3, 2));
                mScript.addCommand(new Command_MoveTo(player, 12, 3, 2));
                mScript.addCommand(new Command_MoveTo(mVecDynamics[1], 6, 5, 0.5));
                mScript.addCommand(new Command_MoveTo(mVecDynamics[1], 6, 3, 1));
                mScript.addCommand(new Command_MoveTo(mVecDynamics[1], 14, 3, 3));
                mScript.addCommand(new Command_ShowDialog({"Bhai:", "What's up?"}));
                mScript.addCommand(new Command_ShowDialog({"You:", "Money first"}));
                mScript.addCommand(new Command_ShowDialog({"Bhai:", "Here you go.", "Now give me stuff"}));
                mScript.addCommand(new Command_ShowDialog({"You:", "Haha, Thanks for the money"}));
                mScript.addCommand(new Command_ShowDialog({"You:", "I don't have any stuff!"}));
                mScript.addCommand(new Command_ShowDialog({"You:", "Gotta run home", "Catch me if you can!"}));
                mScript.addCommand(new Command_ShowDialog({"Run to your main door", "Don't let Bhai catch you"}, {0xFF, 0, 0}));
            }
        }
    }

    void handleInputState(const unsigned char *state, int mouseX, int mouseY, float secPerFrame) override {
        if (!mScript.bUserControlEnabled) {
            return;
        }
        if (state[SDL_SCANCODE_UP]) {
            player->vy -= 10.0f * secPerFrame;
        }
        if (state[SDL_SCANCODE_DOWN]) {
            player->vy += 10.0f * secPerFrame;
        }
        if (state[SDL_SCANCODE_LEFT]) {
            player->vx -= 10.0f * secPerFrame;
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            player->vx += 10.0f * secPerFrame;
        }

    }

    bool onInit() override {
        gameStarted = false;
        playerOnRun = false;
        bGameOver = false;
        totalTimeElapsed = 0.0f;
        ASSETS.loadSprites();
        player = new DynamicCreature("player", ASSETS.getSprite(0), 42, 36, 3);
        player->px = 10;
        player->py = 10;

        DynamicCreature *dynObj1 = new DynamicCreature("bhai", ASSETS.getSprite(1), 32, 32, 4);
        dynObj1->px = 5;
        dynObj1->py = 5;

        // player is always the first object in the vector
        mVecDynamics.emplace_back(player);
        mVecDynamics.emplace_back(dynObj1);

        nTileWidth = 24;
        nTileHeight = 24;
        pCurrentMap = new cMap_Village();

        pathFinder = new PathFinder(pCurrentMap);

        std::unordered_map<std::string, std::string> soundWavFiles;
        soundWavFiles["trumpet"] = "../res/sound/trumpet.wav";
        soundWavFiles["orchestra"] =  "../res/sound/orchestra.wav";
        loadSoundEffects(soundWavFiles);
        return true;
    }

    bool doObjectsOverlap(float px1, float py1, float px2, float py2) {
        float distance = (px1 - px2) * (px1 - px2) + (py1 - py2) * (py1 - py2);
        return distance <= 1.0f;
    }

    bool onFrameUpdate(float fElapsedTime) override {
        float maxVelocity = 10.0f;
        for (auto &object: mVecDynamics) {
            // clamp velocities

            if (object->vy > maxVelocity) {
                object->vy = maxVelocity;
            }
            if (object->vy < -maxVelocity) {
                object->vy = -maxVelocity;
            }
            if (object->vx > maxVelocity) {
                object->vx = maxVelocity;
            }
            if (object->vx < -maxVelocity) {
                object->vx = -maxVelocity;
            }

            float fNewObjectPosX = object->px + object->vx * fElapsedTime;
            float fNewObjectPosY = object->py + object->vy * fElapsedTime;

            // resolve collision along X axis, if any
            if (object->vx < 0) {
                if (pCurrentMap->GetSolid(static_cast<int>(fNewObjectPosX), static_cast<int>(object->py)) ||
                    pCurrentMap->GetSolid(static_cast<int>(fNewObjectPosX), static_cast<int>(object->py + 0.9))) {
                    // cast the new position to an integer and shift by 1 so that the player is on the boundary
                    // of the colliding tile, instead of leaving some space
                    fNewObjectPosX = static_cast<int>(fNewObjectPosX) + 1;
                    object->vx = 0;
                }
            } else if (object->vx > 0) {
                if (pCurrentMap->GetSolid(static_cast<int>(fNewObjectPosX + 1), static_cast<int>(object->py)) ||
                    pCurrentMap->GetSolid(static_cast<int>(fNewObjectPosX + 1), static_cast<int>(object->py + 0.9))) {
                    fNewObjectPosX = static_cast<int>(fNewObjectPosX);
                    object->vx = 0;
                }
            }
            // check collision along y
            if (object->vy < 0) {
                if (pCurrentMap->GetSolid(static_cast<int>(fNewObjectPosX), static_cast<int>(fNewObjectPosY)) ||
                    pCurrentMap->GetSolid(static_cast<int>(fNewObjectPosX + 0.9), static_cast<int>(fNewObjectPosY))) {
                    fNewObjectPosY = static_cast<int>(fNewObjectPosY) + 1;
                    object->vy = 0;
                }
            } else if (object->vy > 0) {
                if (pCurrentMap->GetSolid(static_cast<int>(fNewObjectPosX), static_cast<int>(fNewObjectPosY + 1)) ||
                    pCurrentMap->GetSolid(static_cast<int>(fNewObjectPosX + 0.9),
                                          static_cast<int>(fNewObjectPosY + 1))) {
                    fNewObjectPosY = static_cast<int>(fNewObjectPosY);
                    object->vy = 0;
                }
            }

            // check collision with other dynamic objects if player on run
            if (playerOnRun && !bGameOver) {
                if (object->sName != player->sName) {
                    if (doObjectsOverlap(object->px, object->py, player->px, player->py)) {
                        playSound("orchestra");
                        mScript.addCommand(new Command_ShowDialog({"Game Over!", "You belong to Bhai now", "Press ESC"}));
                        bGameOver = true;
                    }
                } else{
                    if (doObjectsOverlap(object->px, object->py, homeLocation.first, homeLocation.second)){
                        playSound("trumpet");
                        mScript.addCommand(new Command_ShowDialog({"Well Done!", "You deceived Bhai", "Press ESC"}));
                        bGameOver = true;
                    }
                }
            }
            object->px = fNewObjectPosX;
            object->py = fNewObjectPosY;

            // apply friction
            object->vx += -4.0f * object->vx * fElapsedTime;
            if (std::abs(object->vx) < 0.01f) {
                object->vx = 0.0f;
            }
            object->vy += -4.0f * object->vy * fElapsedTime;
            if (std::abs(object->vy) < 0.01f) {
                object->vy = 0.0f;
            }
            object->update(fElapsedTime);
        }
        fCameraPosX = player->px;
        fCameraPosY = player->py;
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
                int spriteIdx = pCurrentMap->GetIndex(x + static_cast<int>(fOffsetX),
                                                      y + static_cast<int>(fOffsetY));
                LTexture *texture = ASSETS.getSprite(spriteIdx);
                texture->drawTexture(static_cast<int>((x - fTileOffsetX) * nTileWidth),
                                     static_cast<int>((y - fTileOffsetY) * nTileHeight), nTileWidth, nTileHeight);
            }
        }
        for (auto &object: mVecDynamics) {
            // draw object
            object->drawSelf(this, fOffsetX, fOffsetY, nTileWidth, nTileHeight);
        }

        mScript.processCommand(fElapsedTime);

        if (!gameStarted) {
            LTexture *font = new LTexture();
            font->loadTextureFromText(GameEngine::getFont(), "Press S to start the game", {0xFF, 0xFF, 0xFF});
            font->drawTexture(200, 200);
        } else {
            if (mScript.isListEmpty()) {
                playerOnRun = true;
            }
            if (playerOnRun && !bGameOver) {
                totalTimeElapsed += fElapsedTime;
                float recalibratePathInterval = 2.0f;
                if (totalTimeElapsed >= recalibratePathInterval) {
                    totalTimeElapsed = 0.0f;
                    enemyVelocity += 0.5f;
                    if (enemyVelocity >= maxVelocity){
                        enemyVelocity = maxVelocity;
                    }
                    // cancel any previously running command
                    for(auto cmd : mScript.mListCommands){
                        cmd->bCompleted = true;
                    }
                    auto aStarResult = pathFinder->solveAStar(mVecDynamics[1]->px, mVecDynamics[1]->py, player->px, player->py);
                    for(auto coord: aStarResult){
                        float distanceX = std::fabs(coord.first - mVecDynamics[1]->px);
                        float distanceY = std::fabs(coord.second - mVecDynamics[1]->py);
                        float targetDuration = (distanceX + distanceY) / enemyVelocity;
                        mScript.addCommand(new Command_MoveTo(mVecDynamics[1], coord.first, coord.second, targetDuration, false));
                    }
                }
            }
        }
        return true;
    }
};

int main() {
    RPG echoes;
    echoes.init(30 * 24, 16 * 24, "Echoes Of Deception");
    echoes.startGameLoop();
    return 0;
}
