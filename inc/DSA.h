#pragma once

#ifdef unix
#include <vector>
#include <map>
#include <unordered_map>
#include "GridMap.h"
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

class DSA : protected GridMap {
  public:
    DSA(const DSA&) = delete;
    DSA& operator=(DSA&) = delete;

    DSA() = default;

    /**
     * @brief Find random "path", or curve between 2 points. This actually results in
     * histogram like shapes
     *
     * @bug results in "disconnected" path. get random int func call interval not valid
     * @image html histogram_forrest_edge.png forrest edge bug size_inidication=20

    * @return std::vector GridMap::Grid vector containing
    * all the points
    */
    std::vector<Grid> randomPath(const Grid& A, const Grid& B) noexcept;
    /**
     * @brief Dijikstra shortest path implementation for finding a random curve/edge
     * between the two given points
     *
     * @return std::vector<GridMap::Grid> Container with the grid coordinates of the
     * path/curve
     */
    std::vector<Grid> dsp(Grid& A, Grid& B) noexcept;

  protected:
};