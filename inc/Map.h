#pragma once

#ifdef unix
#include "Graphics.h"
#include "Solver.h"
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief
 */
template <typename coordinate_type> class Map {
  public:
    Map(const Map&) = delete;
    Map& operator=(Map&) = delete;
    Map() = default;
    virtual ~Map() = default;

    /**
     * @brief Function used for generating the forest
     *
     * @return std::vector<coordinate_type> Vector of coordinate type containing all
     * the coordinates of the forest (of each tree to be placed)
     */
    virtual std::vector<coordinate_type> generateForrest(Solver selectedSolver) noexcept = 0;
    /**
     * @brief Function for generating the mountain(s).
     *
     * @return coordinate_type For now, the system only supports one mountain
     * @todo Optionaly turn the return type into a container in order to support
     * multiple mountains
     */
    virtual coordinate_type generateMountains() noexcept = 0;

  private:
};