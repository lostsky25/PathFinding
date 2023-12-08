#pragma once

#include "DataTypes.h"

template <class T>
class AbstractGrid {
public:
    virtual size_t getCountRows()  {
        return grid.size();
    }

    virtual size_t getCountCols()  {
        return grid[0].size();
    }

    virtual T& operator[](Point p) {
        return grid[p.y][p.x];
    }

    virtual Size getSize() const {
        return Size(grid[0].size(), grid.size());
    }

    virtual T getValue(size_t row, size_t col) const {
        return grid[row][col];
    }

    virtual T getValue(Point p) const {
        return grid[p.y][p.x];
    }

    virtual void setValue(size_t row, size_t col, T value)  {
        grid[row][col] = value;
    }

    virtual void setValue(Point p, T value) {
        grid[p.y][p.x] = value;
    }

    virtual vv<T>& get() {
        return grid;
    }

    virtual void clear(const T value = T(0)) {
        for (size_t i = 0; i < grid.size(); i++) {
            for (size_t j = 0; j < grid[0].size(); j++) {
                grid[i][j] = value;
            }
        }
    }

    virtual void setSize(Size size, T value) {
        grid.resize(size.height);
        for (size_t i = 0; i < size.height; i++) {
            grid[i].resize(size.width, value);
        }
    }    
    
    virtual void setSize(Size size) {
        grid.resize(size.height);
        for (size_t i = 0; i < size.height; i++) {
            grid[i].resize(size.width);
        }
    }

protected:
    vv<T> grid;
};