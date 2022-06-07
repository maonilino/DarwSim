#pragma once

#ifdef unix
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
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
        Tree(const uint16_t populationSize,
            const uint16_t generations); // Needed for GA solver
        Tree() = default;                // Only needeed for DSA solver. Does nothing

        virtual ~Tree() = default;

        /**
         * @brief Implements Gen, Cheng, and Wang's (GCW) ‘Genetic Algorithms for
         * Solving Shortest Path Problems’ (GCW_gasp)
         *
         * @param graph Weighted graph in which we are performing the search
         */
        virtual std::vector<Grid> GCW_gasp(const Grid& A, const Grid& B,
            const std::array<std::array<uint8_t, GRID_HEIGHT>, GRID_WIDTH> graph);
        /**
         * @brief Calculates the objective value of a path according to GCW's paper.
         * This is the 2nd step after converting the chromosomes into paths (we see how
         * good these paths are, and pass this objective value into a fitness fitting
         * function)
         *
         * @param path The path for which we want to calculate the objective score
         * @return uint16_t Total objective score of this path. This should be passed
         * to the fitness fitting function
         */
        virtual uint16_t objective(const std::vector<Grid>& path) const noexcept;
        /**
         * @brief Calculates the fitness score of an individual. Between 0~1. It is
         * inversially proportional to the objective score. In other words we inverse
         * the objective score. This is the 3rd step after converting the chromosomes
         * into paths.
         *
         * @param objective_score The lower this is, the higher the fitness that this
         * function will yield.
         * @param graphSize Size of the grid in which we are searching. (Used for
         * calculating the raio, this is the denominator)
         * @return float Fitness value. The higher, the better.
         */
        virtual float calculateFitness(
            const uint32_t objective_score, const uint8_t graphSize) const noexcept;

        virtual void selection() noexcept override;
        /**
         * @brief Performs position-based crossover (POS), which is a kind of uniform
         * crossover. The elitist way was combined with this method in order to
         * preserve the best chromosome for the next generation and overcome the
         * stochaistic errors of sampling. With the elitist selection, if the best
         * individual in the current generation is not reproduced into the new
         * generation, remove one individual randomly from the new population and add
         * the best one to the new population
         *
         * @note POS is not used here. The paper doesn't specify whether we need to
         * swap the nodeID's or the priorities. NodeID's cannot repeat in a chromosome
         * (they are unique), however, priority values can. That is why if we swap the
         * priority vlues only, it should also work?
         *
         * @return std::vector<encoding> The new generation
         */
        virtual std::vector<encoding> crossover() noexcept override;
        virtual void mutation() noexcept override;

      private:
        std::unique_ptr<GridMapSpecies> enclosing;

        // optional custom members 1 to 1 mapping with the individuals. Useful for
        // clearly seperating each steps of GCW-gasp
        std::vector<uint32_t> scores;
        std::vector<float> fitness;
        std::vector<uint32_t> selected;

        // optional member, used for marking all the nodes that are not too be mutated.
        // (i.e., do not mutate the individuals from the previous generations, and do
        // not mutate the starting and finishing nodes)
        std::array<std::shared_ptr<Grid>, 2> doNotMutate;
        std::pair<uint64_t, float> maxFitness;
    };
    GridMapSpecies() = default;
    GridMapSpecies(const GridMapSpecies&) = delete;
    GridMapSpecies& operator=(GridMapSpecies&) = delete;

    void setWeight(const std::array<std::array<uint8_t, GRID_HEIGHT>, GRID_WIDTH>
            inWeight) noexcept
    {
        weight = inWeight;
    }
};

// extern "C" GridMapSpecies* create_trees();