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

class LTexture {
private:
    SDL_Texture *mTexture = nullptr;
    int mWidth;
    int mHeight;
public:
    LTexture();

    ~LTexture();

    bool loadTextureFromText(const std::string &text, SDL_Color color);

    bool loadTextureFromFile(std::string path);

    void drawTexture(int x, int y, int w = 0, int h = 0, SDL_Rect *clip = NULL,
                     double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWidth() const;

    int getHeight() const;

    void free();
};

struct Color {
    int r;
    int g;
    int b;
};

using KeyEventFuncPtr = std::function<void(int, int, int, int, float)>;

class InputEventHandler {
private:
    static std::vector<std::pair<std::string, KeyEventFuncPtr>> m_callbacks;

public:
    InputEventHandler() = delete; // The callback queue is shared, no point in creating instances
    static void addCallback(const std::string &cb_name, const KeyEventFuncPtr &fn);

    static void removeCallback(const std::string &cb_name);

    static void reset();

    static void runCallbacks(int eventType, int buttonCode, int mousePosX, int mousePosY, float secPerFrame);
};


class GameEngine {
protected:
    int mWindowWidth;
    int mWindowHeight;
    SDL_Event e;
private:
    void initScreen();

    SDL_Window *gWindow = nullptr;
public:
    GameEngine();

    virtual bool onFrameUpdate(float fElapsedTime) = 0;

    virtual bool onInit() = 0;

    virtual bool drawPoint(int x, int y, Color color = {0xFF, 0xFF, 0xFF});

    bool drawLine(int x1, int y1, int x2, int y2, Color color = {0xFF, 0xFF, 0xFF});
    bool fillRect(int x, int y, int w, int h, Color color = {0xFF, 0xFF, 0xFF});

    void DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y,
                            float r = 0.0f, float s = 1.0f, Color color = {0xFF, 0xFF, 0xFF});

    bool constructConsole(int nCharsX, int nCharsY, const char *title);

    bool createResources();

    bool renderConsole();

    void startGameLoop();

    bool loadMusic(const char *path);

    bool playMusic();
    bool stopMusic();
    void close_sdl();

    ~GameEngine();
};
