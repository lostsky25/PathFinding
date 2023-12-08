#pragma once
#include <SFML/Graphics.hpp>

#define COLORS_EMPTY sf::Color(184, 183, 180)
#define COLORS_WALL sf::Color(97, 95, 90)
#define COLORS_START sf::Color(255, 183, 180)
#define COLORS_TARGET sf::Color(184, 255, 180)
#define COLORS_NEIGHBOR sf::Color(0, 168, 70)
#define COLORS_PATH sf::Color(15, 10, 222)

template <class T>
using vv = std::vector<std::vector<T>>;
template <class T>
using v = std::vector<T>;

typedef enum ALG {
    BreadthFirstSearch = 1,
    DepthFirstSearch,

} AlgorithmType;

struct Size
{
    Size() = default;

    Size(int w, int h) : width(w), height(h) {}

    int width;
    int height;
};

struct Point
{
    Point() = default;

    Point(int _x, int _y) : x(_x), y(_y) {}

    int x;
    int y;
};

typedef enum POINT_TYPE {
    WALL = 1,
    EMPTY = 2,
    START = 3,
    TARGET = 4,
} PointType;
