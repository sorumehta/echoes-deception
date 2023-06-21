//
// Created by Saurabh Mehta on 21/06/23.
//
#pragma once

#include <vector>
#include "RPG_Maps.h"

struct Node {
    bool bVisited = false;
    float fGlobalGoal;
    float fLocalGoal;
    int x;
    int y;
    std::vector<Node*> vecNeighbours;
    Node* parent;
};

class PathFinder {
private:
    Node *nodesArr = nullptr;
    int nMapWidth;
    int nMapHeight;


public:
    PathFinder(cMap *map);
    ~PathFinder();

    std::vector<std::pair<int, int>> solveAStar(int startX, int startY, int endX, int endY);
};

