//
// Created by Saurabh Mehta on 20/05/23.
//
#include <fstream>
#include <map>
#include "RPG_Maps.h"

ScriptProcessor *cMap::g_scriptProcessor = nullptr;

cMap::cMap(){
    nWidth = 0;
    nHeight = 0;
    aIndices = nullptr;
    aIsSolid = nullptr;
    hasPlayerBeenHere = false;
}

cMap::~cMap() {
    delete[] aIsSolid;
    delete[] aIndices;

}

int cMap::GetIndex(int x, int y) {
    if(x>=0 && x < nWidth && y>=0 && y < nHeight)
        return aIndices[y*nWidth + x];
    return 0;
}

bool cMap::GetSolid(int x, int y) {
    if(x>=0 && x < nWidth && y>=0 && y < nHeight)
        return aIsSolid[y*nWidth + x];
    return false;
}

bool cMap::Create(const std::string& fileData, const std::string& name) {
    sName = name;
    std::ifstream data(fileData, std::ios::in | std::ios::binary);
    if (data.is_open()){
        data >> nWidth >> nHeight;
        aIsSolid = new bool[nWidth * nHeight];
        aIndices = new int[nWidth * nHeight];
        for (int i=0; i < nWidth*nHeight; i++){
            data >> aIndices[i];
            data >> aIsSolid[i];
        }
        return true;
    }
    return false;
}

bool cMap::PopulateDynamics(std::vector<RPG_Dynamic *> &vecDynamics) {
    return true;
}

bool cMap::onInteraction(std::vector<RPG_Dynamic *> &vecDynObjs, RPG_Dynamic *target, cMap::NATURE nature) {
    return false;
}

void cMap::onChange(RPG_Dynamic *player) {}

cMap_Village::cMap_Village() {

    Create("../res/village.txt",  "village");
}

bool cMap_Village::PopulateDynamics(std::vector<RPG_Dynamic *> &vecDynamics) {
    // add teleport square
    vecDynamics.push_back(new Teleport(12.0, 6.0, "home", 5.0, 10.0));
    DynamicCreature *enemy = new DynamicCreatureEnemy(DynamicCreatureEnemy::MAN, "Gonzo");
    enemy->px = 16;
    enemy->py = 3;
    vecDynamics.push_back(enemy);
    DynamicCreature *enemy2 = new DynamicCreatureEnemy(DynamicCreatureEnemy::WOMAN, "Ginzi");
    enemy2->px = 10;
    enemy2->py = 10;
    vecDynamics.push_back(enemy2);
    return true;
}

bool cMap_Village::onInteraction(std::vector<RPG_Dynamic *> &vecDynObjs, RPG_Dynamic *target, cMap::NATURE nature) {
    if(target->sName == "Teleport"){
        g_scriptProcessor->addCommand(new Command_ChangeMap(
                ((Teleport *)target)->targetMapName,
                ((Teleport *)target)->mapPosX,
                ((Teleport *)target)->mapPosY
                ));
        return true;
    }
    return false;
}

void cMap_Village::onChange(RPG_Dynamic *player) {
    if(hasPlayerBeenHere){
        return;
    }
    g_scriptProcessor->addCommand(
            new Command_ShowDialog(
                    {"Welcome.", "Today you have a secret meeting with Gonzo at the back of your house", "You have a plan to deceive him"}));
    g_scriptProcessor->addCommand(new Command_ShowDialog({"Gonzo:", "Where is my stuff?"}));
    g_scriptProcessor->addCommand(new Command_ShowDialog({"You:", "Money first"}));
    g_scriptProcessor->addCommand(new Command_ShowDialog({"Gonzo:", "Its here, take it. Now give me my stuff"}));
    g_scriptProcessor->addCommand(new Command_ShowDialog({"You:", "I don't have the stuff, haha.", "Catch me if you can"}));
    g_scriptProcessor->addCommand(
            new Command_ShowDialog({"Run to the main door", "Don't let Gonzo catch you"}, {0xFF, 0, 0}));
}

cMap_Home::cMap_Home() {
    nBalloonsCollected = 0;
    nTargetBalloons = 4;
    Create("../res/home.txt",  "home");
}

bool cMap_Home::onInteraction(std::vector<RPG_Dynamic *> &vecDynObjs, RPG_Dynamic *target, cMap::NATURE nature) {
    if (target->sName == "Balloon") {
        vecDynObjs.erase(std::remove(vecDynObjs.begin(), vecDynObjs.end(), target), vecDynObjs.end());
        nBalloonsCollected++;
        if(nBalloonsCollected == nTargetBalloons){
            vecDynObjs.push_back(new Teleport(5.0, 12.0, "victory", 12.0, 7.0));
            vecDynObjs.push_back(new Teleport(4.0, 12.0, "victory", 12.0, 7.0));
        }
    }
    else if(target->sName == "Teleport"){
        g_scriptProcessor->addCommand(new Command_ChangeMap(
                ((Teleport *)target)->targetMapName,
                ((Teleport *)target)->mapPosX,
                ((Teleport *)target)->mapPosY
        ));
        return true;
    }

    return false;
}

bool cMap_Home::PopulateDynamics(std::vector<RPG_Dynamic *> &vecDynamics) {

    vecDynamics.push_back(new Balloon(6.0, 10.0, Balloon::RED));
    vecDynamics.push_back(new Balloon(6.0, 6.0, Balloon::GREEN));
    vecDynamics.push_back(new Balloon(11.0, 8.0, Balloon::BLACK));
    vecDynamics.push_back(new Balloon(12.0, 4.0, Balloon::BLUE));
    DynamicCreature *enemy = new DynamicCreatureEnemy(DynamicCreatureEnemy::MAN, "Gonzo");
    enemy->px = 4;
    enemy->py = 11.5;
    vecDynamics.push_back(enemy);
    DynamicCreature *enemy2 = new DynamicCreatureEnemy(DynamicCreatureEnemy::WOMAN, "Ginzi");
    enemy2->px = 4;
    enemy2->py = 12;
    vecDynamics.push_back(enemy2);
    return true;
}

void cMap_Home::onChange(RPG_Dynamic *player) {
    g_scriptProcessor->addCommand(
            new Command_MoveTo(player, 5, 6, 2.0f));
    g_scriptProcessor->addCommand(
            new Command_ShowDialog(
                    {"You have entered a closed balloon shop.", "Collect all the balloons in the shop"}));

}

Victory::Victory() {
    Create("../res/village.txt",  "victory");
}

void Victory::onChange(RPG_Dynamic *player) {
    g_scriptProcessor->addCommand(new Command_ShowDialog({"Well done, you flew away using the balloons!"}));
}

bool Victory::PopulateDynamics(std::vector<RPG_Dynamic *> &vecDynamics) {
    return true;
}

bool Victory::onInteraction(std::vector<RPG_Dynamic *> &vecDynObjs, RPG_Dynamic *target, cMap::NATURE nature) {
    return true;
}
