#include "SimpleGameEngine.hpp"
#include <cmath>

const int FONT_SIZE = 18;
//const int FONT_WIDTH = 10;
//const int FONT_HEIGHT = 18;

SDL_Renderer *gRenderer = nullptr;
TTF_Font *gFont = NULL;
//The music that will be played
Mix_Music *gMusic = NULL;

LTexture::LTexture() {
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

int LTexture::getHeight() const { return mHeight; }

int LTexture::getWidth() const { return mWidth; }

void LTexture::drawTexture(int x, int y, int w, int h, SDL_Rect *clip, double angle, SDL_Point *center,
                           SDL_RendererFlip flip) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};


    if (w != 0 && h != 0) {
        renderQuad.w = w;
        renderQuad.h = h;
    }
        //Set clip rendering dimensions
    else if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void LTexture::free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

LTexture::~LTexture() {
    //Deallocate
    free();
}

bool LTexture::loadTextureFromText(const std::string &text, SDL_Color color) {
    if (text.length() == 0) {
        // nothing to render
        return true;
    }
    //free existing texture
    free();

    SDL_Surface *textSurface = TTF_RenderUTF8_Solid_Wrapped(gFont, text.c_str(), color, 0);
    if (textSurface == nullptr) {
        std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (mTexture == nullptr) {
        std::cout << "Unable to create texture from rendered text! SDL Error:" << SDL_GetError() << std::endl;
        return false;
    }
    mWidth = textSurface->w;
    mHeight = textSurface->h;
    SDL_FreeSurface(textSurface);
    return true;
}

bool LTexture::loadTextureFromFile(std::string path, bool toColorKey, SDL_Color colorKey) {

    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture *newTexture = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        std::cout << "Unable to load image " << path << " SDL_image Error: \n" << IMG_GetError() << std::endl;
    } else {
        if(toColorKey){
            //Color key image
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, colorKey.r, colorKey.g, colorKey.b));
        }


        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            std::cout << "Unable to create texture from" << path << "SDL Error: %s\n" << SDL_GetError() << std::endl;
        } else {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;

}

void LTexture::setColorMod(SDL_Color color) {
    SDL_SetTextureColorMod(mTexture, color.r, color.g, color.b);
}


GameEngine::GameEngine() : mWindowWidth(80), mWindowHeight(40), gWindow(nullptr) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL initialization failed: " << SDL_GetError();
    }
    // initialise font loading
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error:" << TTF_GetError();
        return;
    }
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: \n" << IMG_GetError() << std::endl;
        return;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        std::cout <<"SDL_mixer could not initialize! SDL_mixer Error: %s\n" << Mix_GetError() << std::endl;

    }
}

GameEngine::~GameEngine() {
    close_sdl();
}

bool GameEngine::constructConsole(int windowWidth = 80, int windowHeight = 40, const char *title = "Window") {
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int maxWidth = DM.w;
    int maxHeight = DM.h;
    if (windowWidth > maxWidth || windowHeight > maxHeight) {
        std::cout << "Window size too large! ";
        std::cout << "maxWidth = " << maxWidth << ", maxHeight = " << maxHeight << std::endl;
        return false;
    }
    gWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               windowWidth, windowHeight, SDL_WINDOW_SHOWN); // 5 margin
    if (gWindow == nullptr) {
        std::cout << "Window could not be created! SDL Error: " << SDL_GetError();
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == nullptr) {
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError();
        return false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    mWindowWidth = windowWidth;
    mWindowHeight = windowHeight;
    return true;

}

bool GameEngine::createResources() {
    gFont = TTF_OpenFont("../res/Roboto-Black.ttf", FONT_SIZE);
    if (gFont == nullptr) {
        std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError();
        return false;
    }
    return true;
}


bool GameEngine::loadMusic(const char *path) {
    gMusic = Mix_LoadMUS( path);
    if (gMusic == NULL) {
        std::cout << "Failed to load beat music! SDL_mixer Error: %s\n" << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}


bool GameEngine::renderConsole() {
    if (gRenderer == nullptr) {
        std::cout << "Renderer is not initialised. Perhaps you forgot to call constructConsole?" << std::endl;
        return false;
    }
    //update screen
    SDL_RenderPresent(gRenderer);
    return true;
}

bool GameEngine::drawLine(int x1, int y1, int x2, int y2, Color color) {
    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    if (SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2) != 0) {
        return false;
    }
    return true;
}

bool GameEngine::drawPoint(int x, int y, Color color) {
    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    if (SDL_RenderDrawPoint(gRenderer, x, y) != 0) {
        return false;
    }
    return true;
}

bool GameEngine::fillRect(int x, int y, int w, int h, Color color) {
    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    const SDL_Rect rect = {x, y, w, h};
    if (SDL_RenderFillRect(gRenderer, &rect) != 0){
        return false;
    }
    return true;
}

void GameEngine::close_sdl() {
    //Free the music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;
    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    //Quit SDL subsystems
    SDL_Quit();
}

void GameEngine::initScreen() {
    //clear screen
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(gRenderer);
}

void GameEngine::startGameLoop() {
    bool quit = false;
    if (!createResources()) {
        std::cout << "error while loading resources" << std::endl;
        close_sdl();
        quit = true;
    }
    initScreen();
    if (!onInit()) {
        std::cout << "onInit function returned error" << std::endl;
        quit = true;
    }
    auto prevFrameTime = std::chrono::system_clock::now();
    auto currFrameTime = std::chrono::system_clock::now();

    const unsigned char *keyboardState = SDL_GetKeyboardState(NULL);

    while (!quit) {
        // handle timing
        currFrameTime = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = currFrameTime - prevFrameTime;
        prevFrameTime = currFrameTime;
        float frameElapsedTime = elapsedTime.count();
        initScreen();
        //handle input
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        onUserInputEvent(0, keyboardState, mouseX, mouseY, frameElapsedTime);
        if (!onFrameUpdate(frameElapsedTime)) {
            quit = true;
        }

        // 4. RENDER OUTPUT

        if (!renderConsole()) {
            std::cout << "error while loading texture" << std::endl;
            quit = true;
        }

    }
}

// Draws a model on screen with the given rotation(r), translation(x, y) and scaling(s)
void GameEngine::DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y,
                                    float r, float s, Color color) {
    // std::pair.first = x coordinate
    // std::pair.second = y coordinate

    // Create translated model vector of coordinate pairs, we don't want to change the original one
    std::vector<std::pair<float, float>> vecTransformedCoordinates;
    unsigned int verts = vecModelCoordinates.size();
    vecTransformedCoordinates.resize(verts);

    // Rotate
    // To rotate the ship by angle A to left, the equations are:
    //    P2_x = |P2|*cos(A1 + A2) where |P1| and |P2| are equal, A1 is original angle, A2 is rotated angle
    // => P2_x = P1_x * cos(A2) - P1_y * sin(A2)
    //    Similarly,
    //    P2_y = P1_x * sin(A2) + P1_y * cos(A2)
    // Since these equations are just manipulating x and y to get new x and y,
    // we can also represent these equations using a matrix multiplication
    // [P2_x] = [cos(A)  -sin(A)] [P1_x]
    // [P2_y] = [sin(A)   cos(A)] [P1_y]
    for (int i = 0; i < verts; i++) {
        vecTransformedCoordinates[i].first =
                vecModelCoordinates[i].first * std::cos(r) - vecModelCoordinates[i].second * std::sin(r);
        vecTransformedCoordinates[i].second =
                vecModelCoordinates[i].first * std::sin(r) + vecModelCoordinates[i].second * std::cos(r);
    }

    // Scale
    for (int i = 0; i < verts; i++) {
        vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
        vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
    }

    // Translate
    for (int i = 0; i < verts; i++) {
        vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
        vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
    }

    // Draw Closed Polygon
    for (int i = 0; i < verts + 1; i++) {
        int j = (i + 1);
        drawLine(static_cast<int>(std::round(vecTransformedCoordinates[i % verts].first)),
                 static_cast<int>(std::round(vecTransformedCoordinates[i % verts].second)),
                 static_cast<int>(std::round(vecTransformedCoordinates[j % verts].first)),
                 static_cast<int>(std::round(vecTransformedCoordinates[j % verts].second)), color);
    }
}

bool GameEngine::playMusic() {
    if( Mix_PlayingMusic() == 0 )
    {
        //Play the music
        Mix_PlayMusic( gMusic, -1 );
    }
}

bool GameEngine::stopMusic() {
    Mix_HaltMusic();
}
