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

    bool loadTextureFromText(SDL_Renderer *renderer,TTF_Font *font, const std::string &text, SDL_Color color);

    bool loadTextureFromFile(SDL_Renderer *renderer, std::string path, bool toColorKey = false, SDL_Color colorKey = {0,0,0});

    void drawTexture(SDL_Renderer *renderer, int x, int y, int w = 0, int h = 0, SDL_Rect *clip = NULL,
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
protected:
    int mWindowWidth;
    int mWindowHeight;
    SDL_Window *mWindow;
    SDL_Renderer *mRenderer;
    TTF_Font *mFont;
    Mix_Music *mMusic;
    int FONT_SIZE;

private:
    void initScreen();

public:
    GameEngine();

    virtual bool onFrameUpdate(float fElapsedTime) = 0;

    virtual bool onInit() = 0;
    virtual void onUserInputEvent(int eventType, const unsigned char *, int mouseX, int mouseY, float secPerFrame) = 0;

    virtual bool drawPoint(int x, int y, Color color = {0xFF, 0xFF, 0xFF});

    bool drawLine(int x1, int y1, int x2, int y2, Color color = {0xFF, 0xFF, 0xFF});
    bool fillRect(int x, int y, int w, int h, Color color = {0xFF, 0xFF, 0xFF});

    void DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y,
                            float r = 0.0f, float s = 1.0f, Color color = {0xFF, 0xFF, 0xFF});

    bool init(int nCharsX, int nCharsY, const char *title);

    bool createResources();

    bool renderConsole();

    void startGameLoop();

    bool loadMusic(const char *path);

    bool playMusic();
    bool stopMusic();
    void close_sdl();

    ~GameEngine();
};
