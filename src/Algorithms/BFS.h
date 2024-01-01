#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <unordered_map>

#include "Algorithms/DataTypes/DataTypes.h"
#include "Algorithms/DataTypes/Visited.h"
#include "Algorithms/DataTypes/Distance.h"
#include "Algorithms/DataTypes/Path.h"
#include "Algorithms/DataTypes/Grid.h"

#include "Algorithms/BaseAlgorithm.h"

class BFS : public BaseAlgorithm
{
public:
    BFS() = default;

    Grid<int>& getGrid();
    Size getGridSize() const override;
    bool hasFromTo() const;
    void setGridSize(Size size) override;
    void drawShortPath() override;
    void reset() override;
    void setFrom(const Point& from) override;
    Point getFrom() override;
    Point getTo() override;
    void setTo(const Point& to) override;
    v<Point> getNeighbors(const Point& p) override;
    bool singleStepOfAlg() override;

    ~BFS() = default;

private:
    bool searchState = true;

    bool stateFrom = false;
    bool stateTo = false;

    std::queue<Point> q;

    Visited<int> visited;
    Path<Point> path;
    Distance<int> dist;

    friend class PathFinding;
};


