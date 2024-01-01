#include "Algorithms/BFS.h"

Grid<int>& BFS::getGrid() {
    return grid;
}

Size BFS::getGridSize() const  {
    return grid.getSize();
}

bool BFS::hasFromTo() const {
    return stateFrom && stateTo;
}

void BFS::setGridSize(Size size) {
    this->size = size;

    grid.setSize(size);
    visited.setSize(size, false);
    short_path.clear();

    reset();

    dist.setSize(size, static_cast<int>(1e9));
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

void BFS::reset() {
    while (!q.empty()) {
        q.pop();
    }
    grid.clear(0);
    visited.clear(0);
    dist.clear(static_cast<int>(1e9));
    short_path.clear();
    path.clear(Point(-1, -1));

    stateFrom = false;
    stateTo = false;
    searchState = true;

    from = { -1, -1 };
    to = { -1, -1 };
}

void BFS::setFrom(const Point& from) {
    if (!(to.x == from.x && to.y == from.y)) {
        if (prevFrom.x >= 0 && prevFrom.y >= 0) {
            visited[prevFrom] = false;
            if (grid.getBoxColor(prevFrom.y, prevFrom.x).toInteger() == COLORS_START.toInteger()) {
                grid.setBoxColor(prevFrom.y, prevFrom.x, COLORS_EMPTY);
            }
        }

        if (!q.empty()) {
            q.pop();
        }

        this->from = from;

        visited[from] = true;
        dist[from] = 0;
        q.push(from);
        grid.setBoxColor(from.y, from.x, COLORS_START);

        prevFrom = from;
        stateFrom = true;
    }
}

void BFS::setTo(const Point& to) {
    if (!(to.x == from.x && to.y == from.y)) {
        if (prevTo.x >= 0 && prevTo.y >= 0) {
            if (grid.getBoxColor(prevTo.y, prevTo.x).toInteger() == COLORS_TARGET.toInteger()) {
                grid.setBoxColor(prevTo.y, prevTo.x, COLORS_EMPTY);
            }
        }

        this->to = to;
        grid.setBoxColor(to.y, to.x, COLORS_TARGET);

        prevTo = to;
        stateTo = true;
    }
}

v<Point> BFS::getNeighbors(const Point& p) {
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

bool BFS::singleStepOfAlg() {
    if (!q.empty() && searchState) {
        auto nowPoint = q.front();
        q.pop();

        for (auto newPoint : getNeighbors(nowPoint)) {
            q.push(newPoint);
            visited[newPoint] = 1;

            if (dist[nowPoint] + 1 < dist[newPoint]) {
                path[newPoint] = nowPoint;
                dist[newPoint] = dist[nowPoint] + 1;
            }

            if (newPoint.x == to.x && newPoint.y == to.y) {
                searchState = false;

                break;
            }
        }

        if (q.empty() || !searchState) {
            return true;
        }
    }

    return false;
}

Point BFS::getTo() {
    return to;
}

Point BFS::getFrom() {
    return from;
}

void BFS::drawShortPath() {
    if ((q.empty() || !searchState) && short_path.empty()) {
        while (to.x != -1 && to.y != -1) {
            short_path.push_back(to);
            to = path[to];
        }

        for (size_t i = 0; i < short_path.size(); i++) {
            grid.setBoxColor(short_path[i].y, short_path[i].x, COLORS_PATH);
        }

        to = { 0, 0 };
    }
}
