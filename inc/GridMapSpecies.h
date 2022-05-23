#pragma once

#ifdef unix
#include <map>
#include <unordered_map>
#include "GA.h"
#include "GridMap.h"
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief Implementation of the tree "species". Only suitable with terrains which
 * relies on our grid map implementation(that's why we make a wraper that will inherit
 * from GridMap, to get access to the protected Grid structure)
 */
struct GridMapSpecies : private GridMap {
    // encoding suitable for GCW-gasp implementations
    using encoding = std::unordered_map<Grid, uint16_t, Grid::HashFunction>;
    /**
     * @brief Wraped tree species. The goal of the species is to "survive"/"evolve"
     * here analogical to find the best shape into which it should grow
     * Implements Gen, Cheng, and Wang's ‘Genetic Algorithms for Solving Shortest Path
     * Problems’ (GCW-gasp)
     *
     */
    class Tree : public GA<encoding> {
      public:
        Tree(Tree&) = delete;
        Tree& operator=(const Tree&) = delete;
        Tree(const uint16_t populationSize); // Needed for GA solver
        Tree() = default; // Only needeed for DSA solver

        virtual ~Tree() = default;

        /**
         * @brief Implements Gen, Cheng, and Wang's (GCW) ‘Genetic Algorithms for
         * Solving Shortest Path Problems’ (GCW_gasp)
         * @bug Generating in the wrong order! Fix it
         *
         * @return std::vector<Grid> proposed path
         */
        virtual std::vector<Grid> GCW_gasp(const Grid& A, const Grid& B);

        virtual float calculateFitness() const noexcept override;
        virtual void selection() const noexcept override;
        virtual void crossover() noexcept override;
        virtual void mutation() noexcept override;
      
      private:
        std::unique_ptr<GridMapSpecies> enclosing;
    };
    GridMapSpecies() = default;
    GridMapSpecies(const GridMapSpecies&) = delete;
    GridMapSpecies& operator=(GridMapSpecies&) = delete;
};

// extern "C" GridMapSpecies* create_trees();