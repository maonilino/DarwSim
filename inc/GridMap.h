#pragma once

#ifdef unix
#include <random>
#include <exception>
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

// Remember to substract 1 as array is 0 based !!!
// if 22 = Ymax array should have 23 elements (with 0)
#define GRID_WIDTH  39
#define GRID_HEIGHT 23

class GridMap {
  public:
    GridMap(const GridMap&) = delete;
    GridMap& operator=(GridMap&) = delete;
    GridMap();
    virtual ~GridMap() = default;

  protected:
    // Grid is protected so that it can only be initialized from within this class.
    // "This is mine and i may change it anytime so you cannot use it"
    struct Grid {
        Grid() = delete;
        Grid(const Grid&) = default;
        Grid& operator=(const Grid&) = default;
        // must provide move constructor and move assign for randomPath function for
        // moving temp grids into vector
        Grid(Grid&&) = default;
        Grid& operator=(Grid&&) = default;
        Grid(uint8_t a, uint8_t b)
            : x(a)
            , y(b)
        {
        }

        bool operator==(const Grid& a) const
        {
            if (this->x == a.x && this->y == a.y)
                return true;
            else
                return false;
        }

        bool operator!=(const Grid& a)
        {
            if (this->x != a.x && this->y != a.y)
                return true;
            else
                return false;
        }

        struct HashFunction {
            uint16_t operator()(const Grid& pos) const
            {
                uint16_t xHash = std::hash<uint16_t>()(pos.x);
                uint16_t yHash = std::hash<uint16_t>()(pos.y) << 1;
                return xHash ^ yHash;
            }
        };
        uint8_t x = 0;
        uint8_t y = 0;
    };

    Grid gridProjection; // transformation matrix

    // use C-style arrays cause we know the size at compile type, it's fixed. Weights
    uint8_t weight[GRID_WIDTH][GRID_HEIGHT];
    // type probably not suitable. Could use a enum to set region to the appropriate
    // type
    bool fill[GRID_WIDTH][GRID_HEIGHT] = {false};

    /**
     * @brief helper for getting a random uint8 number betweem min and max
     *
     * @param min
     * @param max
     * @return uint8_t random number generated with the entropy of the system
     */
    uint8_t getRandomInt(uint8_t min, uint8_t max) const noexcept;
    /**
     * @brief Helper for assigning random weights to the grids in the rectangle formed
     * by the random points. Random weights are used for getting random paths,
     * therefore forming random shapes.
     *
     * @param A Ax > Cx
     * @param C Cy > Ay
     */
    void assignWeights(const Grid& A, const Grid& C) noexcept;
};