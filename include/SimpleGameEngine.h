#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <unordered_map>

class LTexture {
private:
    SDL_Texture *mTexture = nullptr;
    int mWidth;
    int mHeight;
public:
    LTexture();

    ~LTexture();

    bool loadTextureFromText(TTF_Font *font, const std::string &text, SDL_Color color);

    bool loadTextureFromFile(std::string path, bool toColorKey = false, SDL_Color colorKey = {0,0,0});

    void drawTexture(int x, int y, int w = 0, int h = 0, SDL_Rect *clip = NULL,
                     double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void setColorMod(SDL_Color color);

    int getWidth() const;

    int getHeight() const;

    void free();
};

struct Color {
    int r;
    int g;
    int b;
};


class GameEngine {
private:
    static SDL_Window *mWindow;
    static SDL_Renderer *mRenderer;
    static TTF_Font *mFont;
    static std::unordered_map<std::string, Mix_Chunk *> mSoundEffects;
    void initScreen();
protected:
    int mWindowWidth;
    int mWindowHeight;
    int FONT_SIZE;

public:
    GameEngine();
    static SDL_Window *getWindow();
    static SDL_Renderer *getRenderer();
    static TTF_Font *getFont();
    static void loadSoundEffects(std::unordered_map<std::string, std::string> soundFiles);
    static std::unordered_map<std::string, Mix_Chunk *> getSoundEffects();
    static void playSound(std::string soundName);
    virtual bool onFrameUpdate(float fElapsedTime) = 0;
    virtual bool onInit() = 0;
    virtual void handleInputState(const unsigned char *keyboardState, int mouseX, int mouseY, float secPerFrame);
    virtual void handleInputEvent(int eventType, int keyCode, float fElapsedTime );

    static bool drawPoint(int x, int y, Color color = {0xFF, 0xFF, 0xFF});
    static bool drawLine(int x1, int y1, int x2, int y2, Color color = {0xFF, 0xFF, 0xFF});
    static bool fillRect(int x, int y, int w, int h, Color color = {0xFF, 0xFF, 0xFF});
    static bool drawRect(int x, int y, int w, int h, Color color = {0xFF, 0xFF, 0xFF});
    static bool drawText(std::string text, int x, int y, Color color = {0xFF, 0xFF, 0xFF});
    static void DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y,
                            float r = 0.0f, float s = 1.0f, Color color = {0xFF, 0xFF, 0xFF});

    bool init(int nCharsX, int nCharsY, const char *title);

    bool createResources();

    bool renderConsole();

    void startGameLoop();

    void close_sdl();

    ~GameEngine();
};
