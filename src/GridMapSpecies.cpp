#include "GridMapSpecies.h"

GridMapSpecies* create_trees()
{
    return new GridMapSpecies();
}

// GridMapSpecies::GridMapSpecies()
//     : tree()
// {
// }

GridMapSpecies::Tree::Tree()
{
    // 1. generating random tree phenotype
}

void GridMapSpecies::Tree::selection() noexcept
{
    return;
}

float GridMapSpecies::Tree::calculateFitness() noexcept
{
    return 0.1f;
}

void GridMapSpecies::Tree::crossover() noexcept
{
}
