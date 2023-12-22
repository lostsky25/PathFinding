#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <unordered_map>

#include "DataTypes.h"
#include "Visited.h"
#include "Distance.h"
#include "Path.h"
#include "Grid.h"

#include "BaseAlgorithm.h"

class DFS : public BaseAlgorithm
{
public:
    DFS() = default;

    Grid<int>& getGrid();
    Size getGridSize() const override;
    bool hasFromTo() const;
    void setGridSize(Size size) override;
    void reset() override;
    void setFrom(const Point& from) override;
    void setTo(const Point& to) override;
    v<Point> getNeighbors(const Point& p) override;
    bool singleStepOfAlg() override;
    
    ~DFS() = default;

private:
    bool searchState = true;

    bool stateFrom = false;
    bool stateTo = false;

    // std::queue<Point> q;

    Visited<int> visited;
    Path<Point> path;
    v<Point> s;
    // Distance<int> dist;

    friend class PathFinding;
};