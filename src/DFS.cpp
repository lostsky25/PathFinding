#include "DFS.h"

Grid<int>& DFS::getGrid() {
    return grid;
}

Size DFS::getGridSize() const  {
    return grid.getSize();
}

bool DFS::hasFromTo() const {
    return stateFrom && stateTo;
}

void DFS::setGridSize(Size size) {
    this->size = size;

    grid.setSize(size);
    visited.setSize(size, false);
    short_path.clear();

    reset();

    path.setSize(size, Point(-1, -1));

    int x = 0;
    int y = 0;

    for (int i = 0; i < size.height; i++) {
        x += step;
        y += step;
        for (int j = 0; j < size.width; j++) {
            grid.setBoxPosition(i, j, Point(x, y));

            if (grid.getValue(i, j) == 0) {
                grid.setBoxColor(i, j, COLORS_EMPTY);
            }
            else {
                grid.setBoxColor(i, j, COLORS_WALL);
            }

            x += 10 + step;
        }
        x = 0;
        y += 10;
    }
}

void DFS::reset() {
    while (!s.empty()) {
        s.pop_back();
    }
    grid.clear(0);
    visited.clear(0);
    short_path.clear();
    path.clear(Point(-1, -1));

    stateFrom = false;
    stateTo = false;
    searchState = true;
}

void DFS::setFrom(const Point& from) {
    if (!(to.x == from.x && to.y == from.y)) {
        visited[prevFrom] = false;
        if (grid.getBoxColor(prevFrom.y, prevFrom.x).toInteger() == COLORS_START.toInteger()) {
            grid.setBoxColor(prevFrom.y, prevFrom.x, COLORS_EMPTY);
        }

        if (!s.empty()) {
            s.pop_back();
        }

        this->from = from;

        visited[from] = true;
        s.push_back(from);
        grid.setBoxColor(from.y, from.x, COLORS_START);

        prevFrom = from;
        stateFrom = true;
    }
}

void DFS::setTo(const Point& to) {
    if (!(to.x == from.x && to.y == from.y)) {
        if (grid.getBoxColor(prevTo.y, prevTo.x).toInteger() == COLORS_START.toInteger()) {
            grid.setBoxColor(prevTo.y, prevTo.x, COLORS_EMPTY);
        }

        this->to = to;
        grid.setBoxColor(to.y, to.x, COLORS_TARGET);

        prevTo = to;
        stateTo = true;
    }
}

v<Point> DFS::getNeighbors(const Point& p) {
    v<Point> neighbors;
    grid.setBoxColor(p.y, p.x, COLORS_NEIGHBOR);

    for (auto [dy, dx] : delta) {
        Point new_p = { p.x + dx, p.y + dy };
        if (
            new_p.x >= 0 && new_p.x < grid.getCountCols() &&
            new_p.y >= 0 && new_p.y < grid.getCountRows() &&
            !visited[new_p] && grid[new_p] == 0
            ) {
            neighbors.push_back(new_p);
            grid.setBoxColor(new_p.y, new_p.x, COLORS_TARGET);
        }
    }
    return neighbors;
}

bool DFS::singleStepOfAlg() {
    if (!s.empty() && searchState) {
        auto current = s.back();
        s.pop_back();

        visited[current] = true;

        for (const auto& newPoint : getNeighbors(current)) {
            if (!visited[newPoint]) {
                path[newPoint] = current;
                visited[newPoint] = true;
                s.push_back(newPoint);
            }

            if (newPoint.x == to.x && newPoint.y == to.y) {
                searchState = false;
                break;
            }
        }
        if (s.empty() || !searchState) {

            while (to.x != -1 && to.y != -1) {
                short_path.push_back(to);
                to = path[to];
            }

            for (size_t i = 0; i < short_path.size(); i++) {
                grid.setBoxColor(short_path[i].y, short_path[i].x, COLORS_PATH);
            }

            to = { 0, 0 };

            return true;
        }
    }

    return false;
}