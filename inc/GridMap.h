#pragma once

#ifdef unix
#include <random>
#include <exception>
#include <array>
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

// Remember to substract 1 as array is 0 based !!!
// if 22 = Ymax array should have 23 elements (with 0)
#define GRID_WIDTH  39
#define GRID_HEIGHT 23

/**
 * @brief defines the grid style implementation of the map. The grid is used for
 * knowing how many sprites could be rendered (e.g., one sprite occupies one grid
 * box). Iy could also be used for determining collisions in a straight forward
 * manner? This grid maps to window coordinates with the projection/transformation
 * matrix \f$ \begin{pmatrix}
 * 21 \\
 * 28
 * \end{pmatrix} \f$.
 * Additionaly, in our implementation, this class also implements the Genetic
 * algorithm for the "tree species" evolution.
 *
 * @todo Remove private members documentation from doxyfile in final version
 */
class GridMap {
  public:
    GridMap(const GridMap&) = delete;
    GridMap& operator=(GridMap&) = delete;
    GridMap();
    virtual ~GridMap() = default;

  protected:
    // Grid is protected so that it can only be initialized from within this class or
    // children of this class. "This is mine and i may change it anytime so you cannot
    // use it"
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

        bool operator!=(const Grid& a) const
        {
            if (this->x != a.x || this->y != a.y)
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
    std::array<std::array<uint8_t, GRID_HEIGHT>, GRID_WIDTH> weight;
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
    float getRandomFloat(float min, float max) const noexcept;
    float getRandomFloat(uint8_t min, uint32_t max) const noexcept;
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