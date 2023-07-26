//
// Created by Saurabh Mehta on 26/07/23.
//
#pragma once
#include "SimpleGameEngine.h"

class Pseudo3DPlane {
private:
    SDL_Surface* groundSurface;
    SDL_Surface *skySurface;

    float fWorldX = 1000.4f;
    float fWorldY = 1000.39f;
    float fWorldA =  0.77135f;
    float fNear = 0.026492f;
    float fFar =  0.199961f;
    float fFoVHalf = 3.14159f / 4.0f;

public:
    bool update(int mWindowWidth, int mWindowHeight, float fElapsedTime);
    Pseudo3DPlane();

};
