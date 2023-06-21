//
// Created by Saurabh Mehta on 20/06/23.
//
#pragma once

#include <list>
#include "RPG_Dynamic.h"

class RPG_Commands {
public:
    RPG_Commands();
//    ~RPG_Commands();
    bool bCompleted = false;
    bool bStarted = false;
    bool bDisableUserInputWhenRunning = true;
    virtual void start() = 0;
    virtual void update(float fElapsedTime) = 0;
};

class ScriptProcessor{
private:
    std::list<RPG_Commands*> mListCommands;
public:
    bool bUserControlEnabled;
    void addCommand(RPG_Commands *command);
    void processCommand(float fElapsedTime);
    void completeCommand();
    bool isListEmpty();
    ScriptProcessor();
};

class Command_MoveTo : public RPG_Commands {
private:
    float targetPosX, targetPosY;
    float startPosX, startPosY;
    float timeSoFar, totalDuration;
    RPG_Dynamic *object;
public:
    Command_MoveTo(RPG_Dynamic *obj, float targetX, float targetY, float duration=0.0f, bool disableInput = true);
    void start() override;
    void update(float fElapsedTime) override;
};

class Command_ShowDialog : public RPG_Commands {
private:
    std::vector<std::string> vecLines;
public:
    Command_ShowDialog(const std::vector<std::string>& line, bool disableInput=true);
    void start() override;
    void update(float fElapsedTime) override;
};
