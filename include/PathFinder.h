//
// Created by Saurabh Mehta on 21/06/23.
//
#pragma once

#include <vector>

struct Node {
    bool bObstacle = false;
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
    Node *nodes = nullptr;
    int nMapWidth = 16;
    int nMapHeight = 16;
    Node *nodeStart = nullptr;
    Node *nodeEnd = nullptr;

public:
    PathFinder();
    ~PathFinder();

    bool solveAStar();
};

