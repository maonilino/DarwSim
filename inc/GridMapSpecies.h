#pragma once

#ifdef unix
#include <unordered_map>
#include "GA.h"
#include "GridMap.h"
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief Implementation of the tree "species". Only suitable with terrains which
 * relies on 2D Grids (that's why we use pair for the coordinates)
 * The goal of the species is to "survive"/"evolve" here analogical to find the best
 * shape into which it should grow
 *
 */
struct GridMapSpecies : private GridMap {
    class Tree : public GA<Grid> {
      private:
      public:
        Tree(Tree&) = delete;
        Tree& operator=(const Tree&) = delete;
        Tree();

        virtual ~Tree() = default;

        virtual void selection() noexcept override;
        virtual float calculateFitness() noexcept override;
        virtual void crossover() noexcept override;
    };
    GridMapSpecies();
    GridMapSpecies(const GridMapSpecies&) = delete;
    GridMapSpecies& operator=(GridMapSpecies&) = delete;

    Tree tree;
};

extern "C" GridMapSpecies* create_trees();