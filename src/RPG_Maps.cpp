//
// Created by Saurabh Mehta on 20/05/23.
//
#include <fstream>
#include <map>
#include <utility>
#include "RPG_Maps.h"
#include "RPG_Assets.h"

ScriptProcessor *cMap::g_scriptProcessor = nullptr;

cMap::cMap(){
    nWidth = 0;
    nHeight = 0;
    aIndices = nullptr;
    aIsSolid = nullptr;
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
//    vecDynamics.push_back(new Teleport(5.0, 13.0, "village", 12.0, 7.0));
//    vecDynamics.push_back(new Teleport(4.0, 13.0, "village", 12.0, 7.0));
    return true;
}