#include "GridMapSpecies.h"

// GridMapSpecies* create_trees()
// {
//     return new GridMapSpecies();
// }

// GridMapSpecies::GridMapSpecies()
//     : tree()
// {
// }

GridMapSpecies::Tree::Tree(const uint16_t populationSize)
{
    // needed for getting access to helper functions
    population_size = populationSize;
    enclosing.reset(new GridMapSpecies());
    individuals.resize(populationSize);
}

std::vector<GridMap::Grid> GridMapSpecies::Tree::GCW_gasp(const Grid& A, const Grid& B)
{
    // 1. Generating random populations    
    for (auto i = 0; i < population_size; i++) {
        for (auto y = A.y; y >= B.y; y--) {
            for (auto k = A.x; k <= B.x; k++) {
                // highest priority to starting & finishing node
                if (k == A.x && y == A.y)
                    individuals[i].emplace(Grid(k, y), 0xff);
                else
                    individuals[i].emplace(Grid(k, y), enclosing->getRandomInt(1, 0xff));
            }
        }
    }

    // 2. Sequential node appending procedure. We check what nodes are available and
    // choose the one with highest priority
    std::vector<Grid> path;
    for (auto& i : individuals) {
        auto currNode(A);
        std::map<uint16_t, Grid> heap;
        while (currNode != B) {
            if (i.contains(Grid(currNode.x + 1, currNode.y))) {
                Grid temp(currNode.x + 1, currNode.y);
                heap.emplace(i[std::move(temp)], std::move(temp));
            }
            if (i.contains(Grid(currNode.x - 1, currNode.y))) {
                Grid temp(currNode.x - 1, currNode.y);
                heap.emplace(i[std::move(temp)], std::move(temp));
            }
            if (i.contains(Grid(currNode.x, currNode.y + 1))) {
                Grid temp(currNode.x, currNode.y + 1);
                heap.emplace(i[std::move(temp)], std::move(temp));
            }
            if (i.contains(Grid(currNode.x, currNode.y - 1))) {
                Grid temp(currNode.x, currNode.y - 1);
                heap.emplace(i[std::move(temp)], std::move(temp));
            }
            currNode = heap.rbegin()->second;
            heap.clear();
        }
    }
}

float GridMapSpecies::Tree::calculateFitness() const noexcept
{
    return 0.1f;
}

void GridMapSpecies::Tree::selection() const noexcept
{
    return;
}

void GridMapSpecies::Tree::crossover() noexcept
{
}

void GridMapSpecies::Tree::mutation() noexcept
{
}
