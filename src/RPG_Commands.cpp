//
// Created by Saurabh Mehta on 20/06/23.
//

#include "RPG_Commands.h"
#include "RPG_Game.h"

// initialise static variable
// the real initialisation would happen in onInit function
RPG_Game *RPG_Commands::engine = nullptr;

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

RPG_Commands::RPG_Commands() = default;

Command_ShowDialog::Command_ShowDialog(const std::vector<std::string>& line, SDL_Color textColor) {
    vecLines = line;
    mTextColor = textColor;
}

void Command_ShowDialog::start() {
    engine->showDialog(vecLines);
}

void Command_ShowDialog::update(float fElapsedTime) {

}

Command_ChangeMap::Command_ChangeMap(std::string name, float mapPosX, float mapPosY) {
    mapName = name;
    fMapPosX = mapPosX;
    fMapPosY = mapPosY;
}

void Command_ChangeMap::start() {
    engine->changeMap(mapName, fMapPosX, fMapPosY);
    bCompleted = true;
}

void Command_ChangeMap::update(float fElapsedTime) {

}
