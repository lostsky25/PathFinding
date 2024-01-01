#pragma once
#include "Algorithms/DataTypes/DataTypes.h"

class BaseAlgorithm {
public:
    virtual void setFrom(const Point&) = 0;
    virtual void setTo(const Point&) = 0;
    virtual Point getFrom() = 0;
    virtual Point getTo() = 0;

    virtual bool singleStepOfAlg() = 0;
    virtual void drawShortPath() = 0;
    virtual v<Point> getNeighbors(const Point&) = 0;

    virtual void reset() = 0;

    virtual bool hasFromTo() const = 0;

    virtual void setGridSize(const Size) = 0;
    virtual Size getGridSize() const = 0;
    virtual Grid<int>& getGrid() = 0;

protected:
    Point from { 0, 0 };
    Point to { 0, 0 };

    Point prevFrom{ -1, -1 };
    Point prevTo{ -1, -1 };

    Grid<int> grid;
    Size size;

    v<Point> short_path;

    const int step = 1;

    v<Point> delta = {
        { -1, 0 },
        { 1, 0 },
        { 0, -1 },
        { 0, 1 }
    };
};