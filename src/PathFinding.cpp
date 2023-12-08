#include "PathFinding.h"

PathFinding::PathFinding(Size size) {
    algorithms[AlgorithmType::BreadthFirstSearch] = new BFS();
    algorithms[AlgorithmType::DepthFirstSearch] = new BFS();

    for (auto& [algorithmType, algorithm] : algorithms) {
        algorithm->setGridSize(size);
    }
}

bool PathFinding::isOnGrid(Point p) {
    if (p.x < algorithms[currentAlgorithm]->getGrid().getCountCols() && p.x >= 0 && p.y >= 0 && p.y < algorithms[currentAlgorithm]->getGrid().getCountRows()) {
        return true;
    }

    return false;
}

void PathFinding::setFrom(Point from) {
    if (isOnGrid(from)) {
        if (algorithms[currentAlgorithm]->getGrid().getBoxColor(from.y, from.x).toInteger() != COLORS_WALL.toInteger()) {
            algorithms[currentAlgorithm]->setFrom(from);
        }
    }
}

void PathFinding::setTo(Point to) {
    if (isOnGrid(to)) {
        if (algorithms[currentAlgorithm]->getGrid().getBoxColor(to.y, to.x).toInteger() != COLORS_WALL.toInteger()) {
            algorithms[currentAlgorithm]->setTo(to);
        }
    }
}

void PathFinding::clear() {
    algorithms[currentAlgorithm]->reset();
}

void PathFinding::setAlgorithm(AlgorithmType algorithmType) {
    currentAlgorithm = algorithmType;
}

BaseAlgorithm& PathFinding::getAlgorithm() {
    return *algorithms[currentAlgorithm];
}