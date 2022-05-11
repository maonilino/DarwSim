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
 * relies on our grid map implementation(that's why we make a wraper that will inherit
 * from GridMap, to get access to the protected Grid structure)
 *
 * @todo provide implementation of the priority based encoding
 */
struct GridMapSpecies : private GridMap {
    /**
     * @brief Wraped tree species. The goal of the species is to "survive"/"evolve"
     * here analogical to find the best shape into which it should grow
     *
     */
    class Tree : public GA<Grid> {
      public:
        Tree(Tree&) = delete;
        Tree& operator=(const Tree&) = delete;
        Tree();

        virtual ~Tree() = default;

        virtual void selection() noexcept override;
        virtual float calculateFitness() noexcept override;
        virtual void crossover() noexcept override;
    };
    GridMapSpecies() = default;
    GridMapSpecies(const GridMapSpecies&) = delete;
    GridMapSpecies& operator=(GridMapSpecies&) = delete;
};

extern "C" GridMapSpecies* create_trees();