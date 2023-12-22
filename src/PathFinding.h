#pragma once

#include "BFS.h"
#include "DFS.h"

class PathFinding
{
public:
    PathFinding(Size size);

    bool isOnGrid(Point p);
    void setFrom(Point from);
    void setTo(Point to);
    void clear();
    void setAlgorithm(AlgorithmType algorithmType);
    BaseAlgorithm& getAlgorithm();

    ~PathFinding() = default;

private:
    std::unordered_map<AlgorithmType, BaseAlgorithm*> algorithms;
    AlgorithmType currentAlgorithm = AlgorithmType::BreadthFirstSearch;
};