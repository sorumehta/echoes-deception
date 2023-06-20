//
// Created by Saurabh Mehta on 20/05/23.
//
#include <fstream>
#include <map>
#include <utility>
#include "RPG_Maps.h"
#include "RPG_Assets.h"

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

cMap_Village::cMap_Village() {

    Create("../res/village.txt",  "Echotopia");
}
