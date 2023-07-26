//
// Created by Saurabh Mehta on 26/07/23.
//

#include "Pseudo3DPlane.h"

Pseudo3DPlane::Pseudo3DPlane() {
    groundSurface = IMG_Load("../res/graphics/mariokart.png");
    skySurface = IMG_Load("../res/graphics/sky1.png");

}

bool Pseudo3DPlane::update(int mWindowWidth, int mWindowHeight, float fElapsedTime) {
    // calculate the corners of the frustum
    float fFarX1 = fWorldX + cosf(fWorldA - fFoVHalf) * fFar;
    float fFarY1 = fWorldY + sinf(fWorldA - fFoVHalf) * fFar;

    float fNearX1 = fWorldX + cosf(fWorldA - fFoVHalf) * fNear;
    float fNearY1 = fWorldY + sinf(fWorldA - fFoVHalf) * fNear;

    float fFarX2 = fWorldX + cosf(fWorldA + fFoVHalf) * fFar;
    float fFarY2 = fWorldY + sinf(fWorldA + fFoVHalf) * fFar;

    float fNearX2 = fWorldX + cosf(fWorldA + fFoVHalf) * fNear;
    float fNearY2 = fWorldY + sinf(fWorldA + fFoVHalf) * fNear;

    // assume that our vertical vanishing point is half way across the screen
    // starting with the furthest away scanline
    for(int y=0; y < mWindowHeight/2; y++){
        // normalise the value between 0 and 1
        float fSampleDepth = (float)y / ((float)mWindowHeight / 2.0f);
        // knowing how far into the frustum we want to be, we calculate two points
        // that represent scan line begin and end point
        // think of it as the horizontal line going across the frustum at a given depth
        float fStartX = (fFarX1 - fNearX1) / (fSampleDepth) + fNearX1;
        float fStartY = (fFarY1 - fNearY1) / (fSampleDepth) + fNearY1;
        float fEndX = (fFarX2 - fNearX2) / (fSampleDepth) + fNearX2;
        float fEndY = (fFarY2 - fNearY2) / (fSampleDepth) + fNearY2;

        // similarly, we now calculate where along this scan line should we sample our pixel
        for (int x = 0; x < mWindowWidth; x++){
            float fSampleWidth = (float)x / (float)mWindowWidth;
            float fSampleX = (fEndX - fStartX) * fSampleWidth + fStartX;
            float fSampleY = (fEndY - fStartY) * fSampleWidth + fStartY;
            // take the mod so that we never reach the end
            fSampleX = std::fmod(fSampleX, 1.0f);
            fSampleY = std::fmod(fSampleY, 1.0f);
            Uint32 groundPixel = GameEngine::get_pixel(groundSurface, fSampleX, fSampleY);
            Uint8 gR, gG, gB, gA;
            SDL_GetRGBA(groundPixel, groundSurface->format, &gR, &gG, &gB, &gA);

            Uint32 skyPixel = GameEngine::get_pixel(skySurface, fSampleX, fSampleY);
            Uint8 sR, sG, sB, sA;
            SDL_GetRGBA(skyPixel, skySurface->format, &sR, &sG, &sB, &sA);

            GameEngine::drawPoint(x, mWindowHeight/2 + y, {gR, gG, gB});
            GameEngine::drawPoint(x, mWindowHeight/2 - y, {sR, sG, sB});
        }
    }

    fWorldX += std::cosf(fWorldA) * 0.2f * fElapsedTime;
    fWorldY += std::sinf(fWorldA) * 0.2f * fElapsedTime;

    return true;
}


