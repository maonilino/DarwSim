#pragma once

#ifdef unix
#include "Map.h"
#include "GridMapSpecies.h"
#include "DSA.h"
#include <random>
#include <exception>
#include <map>
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief Implementation of the Map interface. Here we generate the terrain/map with
 * the appropriate settings.
 *
 */
class MapGenerator : public Map<glm::vec2>, private GridMapSpecies::Tree, DSA {
  public:
    MapGenerator(const MapGenerator&) = delete;
    MapGenerator& operator=(MapGenerator&) = delete;

    MapGenerator() = default;                    // default constructor for dsa solver
    MapGenerator(const uint16_t populationSize); // custom constructor for ga solver
    virtual ~MapGenerator() = default;

    /**
     * @brief Helper function used for filling the trees in the shape
     *
     * @bug doesn't fill when k == path.front().y Need to consider this edge case
     *
     * @param path Line/Curve representing limit of forrest (edge between forrest and
     * plains)
     * @param increment decrement or increment (fill downwards or upwards)
     */
    void fillTrees(const std::vector<Grid>& path, bool increment) noexcept;
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

  /**
   * @brief 
   * 
   * @param selectedSolver solver to be used
   * @return std::vector<glm::vec2> forest area
   */
    virtual std::vector<glm::vec2> generateForrest(
        Solver selectedSolver = Solver::DSA) noexcept override;
    virtual glm::vec2 generateMountains() noexcept override;

  private:
};

extern "C" MapGenerator* create_dsa_map();

extern "C" MapGenerator* create_ga_map(const uint16_t populationSize);