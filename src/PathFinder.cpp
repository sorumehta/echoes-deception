//
// Created by Saurabh Mehta on 21/06/23.
//

#include "PathFinder.h"
#include <algorithm>
#include <list>
#include <utility>
#include <vector>

PathFinder::PathFinder(cMap *map) {
    nMapWidth = map->nWidth;
    nMapHeight = map->nHeight;
    nodesArr = new Node[nMapWidth * nMapHeight];
    for (int x = 0; x < nMapWidth; x++) {
        for (int y = 0; y < nMapHeight; y++) {
            int idx = y * nMapWidth + x;
            nodesArr[idx].x = x;
            nodesArr[idx].y = y;
            nodesArr[idx].parent = nullptr;
            nodesArr[idx].bVisited = false;
        }
    }

    for (int x = 0; x < nMapWidth; x++) {
        for (int y = 0; y < nMapHeight; y++) {
            int idx = y * nMapWidth + x;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (std::abs(i) == std::abs(j))
                        continue;
                    int neighborX = x + i;
                    int neighborY = y + j;

                    if (neighborX >= 0 && neighborX < nMapWidth &&
                        neighborY >= 0 && neighborY < nMapHeight) {
                        if (!map->GetSolid(neighborX, neighborY))
                            nodesArr[idx].vecNeighbours.emplace_back(
                                &nodesArr[neighborY * nMapWidth + neighborX]);
                    }
                }
            }
        }
    }
}

std::vector<std::pair<int, int>> PathFinder::solveAStar(int startX, int startY,
                                                        int endX, int endY) {
    Node *nodeStart = &nodesArr[startY * nMapWidth + startX];
    Node *nodeEnd = &nodesArr[endY * nMapWidth + endX];
    for (int x = 0; x < nMapWidth; x++) {
        for (int y = 0; y < nMapHeight; y++) {
            int idx = y * nMapWidth + x;
            nodesArr[idx].fGlobalGoal = INFINITY;
            nodesArr[idx].fLocalGoal = INFINITY;
            nodesArr[idx].parent = nullptr;
            nodesArr[idx].bVisited = false;
        }
    }
    auto heuristic = [](Node *a, Node *b) {
        return (a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y);
    };

    Node *nodeCurrent = nodeStart;
    nodeStart->fLocalGoal = 0.0f;
    nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

    std::list<Node *> listNotTestedNodes;
    listNotTestedNodes.emplace_back(nodeStart);

    while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd) {
        listNotTestedNodes.sort([](const Node *lhs, const Node *rhs) {
            return lhs->fGlobalGoal < rhs->fGlobalGoal;
        });
        // front of the list might also contain nodes that have been visited
        while (!listNotTestedNodes.empty() &&
               listNotTestedNodes.front()->bVisited) {
            listNotTestedNodes.pop_front();
        }
        if (listNotTestedNodes.empty())
            break;

        nodeCurrent = listNotTestedNodes.front();
        nodeCurrent->bVisited = true;

        for (auto nodeNbr : nodeCurrent->vecNeighbours) {
            if (!nodeNbr->bVisited) {
                listNotTestedNodes.emplace_back(nodeNbr);
            }
            float candLocalGoal =
                nodeCurrent->fLocalGoal + heuristic(nodeCurrent, nodeNbr);
            if (candLocalGoal < nodeNbr->fLocalGoal) {
                nodeNbr->fLocalGoal = candLocalGoal;
                nodeNbr->parent = nodeCurrent;
                nodeNbr->fGlobalGoal =
                    nodeNbr->fLocalGoal + heuristic(nodeNbr, nodeEnd);
            }
        }
    }
    std::vector<std::pair<int, int>> pathToTake;
    Node *p = nodeEnd;
    while (p->parent != nullptr) {
        pathToTake.emplace_back(p->x, p->y);
        p = p->parent;
    }
    std::reverse(pathToTake.begin(), pathToTake.end());
    return pathToTake;
}

PathFinder::~PathFinder() { delete[] nodesArr; }
