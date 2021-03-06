#pragma once

#ifdef unix
#include "Map.h"
#include "GridMapSpecies.h"
#include "DSA.h"
#include <random>
#include <exception>
#include <map>
#include <fstream>
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

    MapGenerator() = default; // default constructor for dsa solver
    MapGenerator(const uint16_t populationSize,
        const uint16_t genSize); // custom constructor for ga solver
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
        Solver selectedSolver = Solver::DSA) override;
    virtual glm::vec2 generateMountains() noexcept override;

  private:
    /**
     * @brief Helper function for retreiving specific file. Remember to close the file
     * after reading the file, because this function will modify the stream buffer,
     * therefore we need to reset it
     *
     * @param file
     * @param num Line number to read. 0 based!
     * @return std::fstream& New stream containing the line that we read
     *
     * @warning 0 based line reader. If we want to read line 4, num should be 3!
     */
    std::fstream& Go2Line(std::fstream& file, unsigned int num)
    {
        file.seekg(std::ios::beg);
        while (num > 0) // keep going until no more lines
        {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            --num; // saw a line. reduce number of lines remaining to see
        }
        return file;
    }
};

extern "C" MapGenerator* create_dsa_map();

extern "C" MapGenerator* create_ga_map(
    const uint16_t populationSize, const uint16_t genSize);