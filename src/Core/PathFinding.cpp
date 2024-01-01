#include "Core/PathFinding.h"

PathFinding::PathFinding(Size size) {
    algorithms[AlgorithmName::BreadthFirstSearch] = new BFS();
    algorithms[AlgorithmName::DepthFirstSearch] = new DFS();

    for (auto& [algorithmType, algorithm] : algorithms) {
        algorithm->setGridSize(size);
    }
}

PathFinding::~PathFinding() {
    for (auto& [name, algorithm] : algorithms) {
        if (algorithm != nullptr)
            delete algorithm;
    }
}

bool PathFinding::isOnGrid(Point p) {
    if (p.x < algorithms[currentAlgorithm]->getGrid().getCountCols() && p.x >= 0 && p.y >= 0 && p.y < algorithms[currentAlgorithm]->getGrid().getCountRows()) {
        return true;
    }

    return false;
}

void PathFinding::clear() {
    algorithms[currentAlgorithm]->reset();
}

void PathFinding::setAlgorithm(AlgorithmName algorithmName) {
    currentAlgorithm = algorithmName;
}

BaseAlgorithm& PathFinding::getAlgorithm() {
    return *algorithms[currentAlgorithm];
}