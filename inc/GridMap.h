#pragma once

#ifdef unix
#include "Map.h"
#include <random>
#include <exception>
#include <map>
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

// Remember to substract 1 as array is 0 based !!!
// if 22 = Ymax array should have 23 elements (with 0)
#define GRID_WIDTH 39
#define GRID_HEIGHT 23

/**
 * @brief defines the grid style implementation of the map. The grid is used for
 * knowing how many sprites could be rendered (e.g., one sprite occupies one grid box).
 * Iy could also be used for determining collisions in a straight forward manner? This
 * grid maps to window coordinates with the projection/transformation matrix \f$
 * \begin{pmatrix}
 * 21 \\
 * 28
 * \end{pmatrix} \f$.
 *
 * @todo Remove private members documentation from doxyfile in final version
 */
class GridMap : public Map<glm::vec2> {
  private:
    enum class IncrementDirection {up, down, left, right};
    // Grid is private so that it can only be initialized from within this class. "This
    // is mine and i may change it anytime so you cannot use it"
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

        uint8_t x = 0;
        uint8_t y = 0;

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
    };

    // use C-style arrays cause we know the size at compile type, it's fixed. Weights
    // only used for dijkstra algo
    uint8_t weight[GRID_WIDTH][GRID_HEIGHT];
    // type probably not suitable. Could use a enum to set region to the appropriate
    // type
    bool fill[GRID_WIDTH][GRID_HEIGHT] = {false};

    Grid gridProjection; // transformation matrix

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
    /**
     * @brief Dijkstra’s shortest path algorithm for finding shortest path between the
     * 2 points. The weights in the grid are always random, thus this path should never
     * be identical.
     *
     * @return std::vector GridMap::Grid&& an array with the coordinates of the path.
     * This should later be used for drawing under the curve.
     */
    std::vector<GridMap::Grid>&& findShortestPath(
        const Grid& A, const Grid& B) noexcept;
    /**
     * @brief Find random "path", or curve between 2 points. This actually results in
     * histogram like shapes
     *
     * @bug results in "disconnected" path. get random int func call interval not valid
     * @image html histogram_forrest_edge.png forrest edge bug size_inidication=20

     * @return std::vector GridMap::Grid vector containing
     * all the points
     */
    std::vector<GridMap::Grid> randomPath(const Grid& A, const Grid& B) noexcept;
    /**
     * @brief Dijikstra shortest path implementation for finding a random curve/edge
     * between the two given points
     *
     * @return std::vector<GridMap::Grid> Container with the grid coordinates of the
     * path/curve
     */
    std::vector<GridMap::Grid> dsp(Grid& A, Grid& B) noexcept;
    /**
     * @brief Helper function used for filling the trees in the shape
     *
     * @bug doesn't fill when k == path.front().y Need to consider this edge case
     *
     * @param path Line/Curve representing limit of forrest (edge between forrest and
     * plains)
     * @param increment decrement or increment (fill downwards or upwards)
     */
    void fillTrees(const std::vector<GridMap::Grid>& path, bool increment) noexcept;
    /**
     * @brief Transforms the grid coordinates of the forrest into window coordinates
     *
     * @return std::vector<glm::vec2> vector with the coordinates of each tree from the
     * forrest
     * @image html grid_tranformation_bug.png forrest transformation bug
     * size_inidication=30
     * @bug forrest transformation error. Due to transformatrion matrix multiplying by
     * 0 (first element of array is 0) unmatching references when transforming from
     * grid coordintates to window coordinates
     * @warning Bug FIXED
     *
     * Transformation function was incorrect due to GRID_HEIGHT and WEIGHT being
     * incorrect. Also, the value the window size was incorrect by one grid. 21 * 38 =
     * 798, but 798 is the starting coordinate of the sprite (i.e., the sprite drawing
     * ends after). MAIN error was in the size and indexing of the filling array.
     * We use this projecton matrix so that we have (0,0) at the top left of the window
     * Our code follows this coordinate system, the transformation takes care of all
     * the rest.
     */
    std::vector<glm::vec2> tranformForrest() const noexcept;

    Grid* O;

  public:
    GridMap(const GridMap&) = delete;
    GridMap& operator=(GridMap&) = delete;
    GridMap();
    virtual ~GridMap() = default;

    virtual std::vector<glm::vec2> generateForrest() noexcept override;
    virtual glm::vec2 generateMountains() noexcept override;
};

extern "C" GridMap* create_grid_map();