//
// Created by Saurabh Mehta on 20/06/23.
//

#include "RPG_Commands.h"

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


Command_MoveTo::Command_MoveTo(RPG_Dynamic *obj, float targetX, float targetY, float duration) {
    object = obj;
    targetPosX = targetX;
    targetPosY = targetY;
    totalDuration = std::max(duration, 0.001f);
    timeSoFar = 0.0f;
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
//
//RPG_Commands::~RPG_Commands() {
//
//}
