//
// Created by Saurabh Mehta on 20/06/23.
//

#include "RPG_Commands.h"
#include "SimpleGameEngine.h"

ScriptProcessor::ScriptProcessor() {
    bUserControlEnabled = true;
}

void ScriptProcessor::addCommand(RPG_Commands *command){
    mListCommands.emplace_back(command);
}

void ScriptProcessor::processCommand(float fElapsedTime){
    bUserControlEnabled = mListCommands.empty();
    if(!mListCommands.empty()){
        auto command = mListCommands.front();
        bUserControlEnabled = !command->bDisableUserInputWhenRunning;
        if(!command->bCompleted) {
            if (command->bStarted) {
                command->update(fElapsedTime);
            } else {
                command->start();
                command->bStarted = true;
            }
        } else{
//            delete command;
            mListCommands.pop_front();
        }
    }
}

void ScriptProcessor::completeCommand(){
    if(!mListCommands.empty()){
        auto commandAtFront = mListCommands.front();
        commandAtFront->bCompleted = true;
    }
}

bool ScriptProcessor::isListEmpty() {
    return mListCommands.empty();
}


Command_MoveTo::Command_MoveTo(RPG_Dynamic *obj, float targetX, float targetY, float duration, bool disableInput) {
    object = obj;
    targetPosX = targetX;
    targetPosY = targetY;
    totalDuration = std::max(duration, 0.001f);
    timeSoFar = 0.0f;
    bDisableUserInputWhenRunning = disableInput;
}

void Command_MoveTo::start() {
    startPosX = object->px;
    startPosY = object->py;
}

void Command_MoveTo::update(float fElapsedTime) {
    timeSoFar += fElapsedTime;
//    float normalizedTime = timeSoFar / totalDuration;
//    if (normalizedTime >= 1.0f){
//        normalizedTime = 1.0f;
//    }
//    object->px = (targetPosX - startPosX) * normalizedTime + startPosX;
//    object->py = (targetPosY - startPosY) * normalizedTime + startPosY;

    object->vx = (targetPosX - startPosX) / totalDuration;
    object->vy = (targetPosY - startPosY) / totalDuration;
    if (timeSoFar >= totalDuration){
        object->vx = 0.0f;
        object->vy = 0.0f;
        object->px = targetPosX;
        object->py = targetPosY;
        bCompleted = true;
    }
}
//
RPG_Commands::RPG_Commands() = default;

Command_ShowDialog::Command_ShowDialog(const std::vector<std::string>& line, SDL_Color textColor) {
    vecLines = line;
    mTextColor = textColor;
}

void Command_ShowDialog::start() {

}

void Command_ShowDialog::update(float fElapsedTime) {
    int nLines = vecLines.size(); // h
    int maxLineLength = 0; // w
    for (auto l : vecLines){
        if (l.size() > maxLineLength){
            maxLineLength = l.size();
        }
    }
    // Now I want to call fillRect function of game engine here, but I can't, because it is inside a class
    // But there is nothing instance dependent in that method. That's why OOP sucks.
    // So I will break some rules here and just call the raw SDL functions
    int dialogBoxPosX = 20;
    int dialogBoxPosY = 20;
    SDL_SetRenderDrawColor(GameEngine::getRenderer(), 0, 0, 0xFF, SDL_ALPHA_OPAQUE);
    const SDL_Rect rect = {dialogBoxPosX, dialogBoxPosY, maxLineLength * 20, nLines * 20};
    SDL_RenderFillRect(GameEngine::getRenderer(), &rect);
    for (int l = 0; l < vecLines.size(); l++){
        LTexture *font = new LTexture();
        font->loadTextureFromText(GameEngine::getFont(), vecLines[l], mTextColor);
        font->drawTexture(dialogBoxPosX, dialogBoxPosY + l*18  );
    }
    LTexture *font = new LTexture();
    font->loadTextureFromText(GameEngine::getFont(), "press SPACE to continue", {0xFF, 0xFF, 0xFF});
    font->drawTexture(dialogBoxPosX, dialogBoxPosY + vecLines.size()*18  );

}
