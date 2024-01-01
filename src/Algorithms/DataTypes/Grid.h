#pragma once

#include "Core/Core.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "Algorithms/DataTypes/AbstractGrid.h"

template <class T = int>
class Grid : public AbstractGrid<T>
{
public:
    vv<sf::RectangleShape>& getView() {
        return gridView;
    }

    vv<int>& get() override {
        return AbstractGrid<T>::grid;
    }

    void setSize(Size size) override {
        gridView.resize(size.height);
        AbstractGrid<T>::grid.resize(size.height);

        for (size_t i = 0; i < size.height; i++) {
            gridView[i].resize(size.width, sf::RectangleShape(sf::Vector2f(10, 10)));
            AbstractGrid<T>::grid[i].resize(size.width, 0);
        }
    }

    void setBoxPosition(int row, int col, Point p) {
        gridView[row][col].setPosition(static_cast<float>(p.x), static_cast<float>(p.y));
    }

    void setBoxColor(int row, int col, sf::Color color) {
        if (color == COLORS_EMPTY) {
            AbstractGrid<T>::grid[row][col] = 0;
        }
        else if (color == COLORS_WALL) {
            AbstractGrid<T>::grid[row][col] = 1;
        }
        else if (color == COLORS_START) {
            AbstractGrid<T>::grid[row][col] = 0;
        }
        else if (color == COLORS_TARGET) {
            AbstractGrid<T>::grid[row][col] = 0;
        }

        gridView[row][col].setFillColor(color);
    }

    sf::Color getBoxColor(int row, int col) {
        return gridView[row][col].getFillColor();
    }

    void clear(const T value = T(0)) override {
        for (size_t i = 0; i < gridView.size(); i++) {
            for (size_t j = 0; j < gridView[0].size(); j++) {
                gridView[i][j].setFillColor(COLORS_EMPTY);
                AbstractGrid<T>::grid[i][j] = 0;
            }
        }
    }

private:
    vv<sf::RectangleShape> gridView;
};