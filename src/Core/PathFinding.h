#pragma once

#include "Algorithms/BFS.h"
#include "Algorithms/DFS.h"

class PathFinding
{
public:
    PathFinding(Size size);

    bool isOnGrid(Point p);
    void clear();
    void setAlgorithm(AlgorithmName algorithmName);
    BaseAlgorithm& getAlgorithm();
    ~PathFinding();

private:
    std::unordered_map<AlgorithmName, BaseAlgorithm*> algorithms;
    AlgorithmName currentAlgorithm = AlgorithmName::BreadthFirstSearch;
};