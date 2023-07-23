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
    vecDynamics.push_back(new Teleport(12.0, 6.0, "home", 5.0, 12.0));
    DynamicCreature *enemy = new DynamicCreatureEnemy();
    enemy->px = 14;
    enemy->py = 3;
    vecDynamics.push_back(enemy);
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
                    {"Welcome.", "Today you have a secret meeting with Gonzo at the back of your house", "You have a plan to deceive him"},
                    {0xFF, 0, 0}));
    g_scriptProcessor->addCommand(new Command_ShowDialog({"Gonzo:", "Where is my stuff?"}));
    g_scriptProcessor->addCommand(new Command_ShowDialog({"You:", "Money first"}));
    g_scriptProcessor->addCommand(new Command_ShowDialog({"Gonzo:", "Its here, take it. Now give me my stuff"}));
    g_scriptProcessor->addCommand(new Command_ShowDialog({"You:", "I don't have the stuff, haha.", "Catch me if you can"}));
    g_scriptProcessor->addCommand(
            new Command_ShowDialog({"Run to the main door", "Don't let Gonzo catch you"}, {0xFF, 0, 0}));
}

cMap_Home::cMap_Home() {
    Create("../res/home.txt",  "home");
}

bool cMap_Home::onInteraction(std::vector<RPG_Dynamic *> &vecDynObjs, RPG_Dynamic *target, cMap::NATURE nature) {
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

bool cMap_Home::PopulateDynamics(std::vector<RPG_Dynamic *> &vecDynamics) {
    // add teleport square
    vecDynamics.push_back(new Teleport(5.0, 13.0, "village", 12.0, 7.0));
    vecDynamics.push_back(new Teleport(4.0, 13.0, "village", 12.0, 7.0));
    return true;
}

void cMap_Home::onChange(RPG_Dynamic *player) {
    g_scriptProcessor->addCommand(new Command_ShowDialog({"Welcome home, son"}));
}
