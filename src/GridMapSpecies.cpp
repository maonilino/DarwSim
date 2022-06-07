#include "GridMapSpecies.h"

// GridMapSpecies* create_trees()
// {
//     return new GridMapSpecies();
// }

// GridMapSpecies::GridMapSpecies()
//     : tree()
// {
// }

GridMapSpecies::Tree::Tree(const uint16_t populationSize, const uint16_t gens)
{
    // needed for getting access to helper functions
    population_size = populationSize;
    generations = gens;
    enclosing.reset(new GridMapSpecies());
    individuals.resize(populationSize);
    scores.resize(populationSize);
    fitness.resize(populationSize);
}

std::vector<GridMap::Grid> GridMapSpecies::Tree::GCW_gasp(const Grid& A, const Grid& B,
    const std::array<std::array<uint8_t, GRID_HEIGHT>, GRID_WIDTH> inWeight)
{
    // Get the weighted graph
    enclosing->setWeight(inWeight);

    // we do not want to mutate the priority of the starting and finishing nodes
    // which are always 0xf
    doNotMutate[0].reset(new Grid(A.x, A.y));
    doNotMutate[1].reset(new Grid(B.x, B.y));

    std::vector<Grid> path; // final path
    // 1. Generating random populations
    uint8_t pathGridSize = 0; // the grid size where the current path is being searched
    for (auto i = 0; i < population_size; i++) {
        // can garantee that when calling this function A,y will always be greater
        // than B.y. Not the same case for x though. This is why we need to check
        // the order.
        for (int y = A.y; y >= B.y; y--) {
            if (A.x <= B.x) {
                pathGridSize = (B.x - A.x) * (A.y - B.y);
                for (int k = A.x; k <= B.x; k++) {
                    // highest priority to starting & finishing node
                    if ((k == A.x && y == A.y) || (k == B.x && y == B.y))
                        individuals[i].emplace(Grid(k, y), 0xff);
                    else
                        individuals[i].emplace(
                            Grid(k, y), enclosing->getRandomInt(1, 0xfe));
                }
            }
            else {
                pathGridSize = (A.x - B.x) * (A.y - B.y);
                for (int k = A.x; k >= B.x; k--) {
                    // highest priority to starting & finishing node
                    if ((k == A.x && y == A.y) || (k == B.x && y == B.y))
                        individuals[i].emplace(Grid(k, y), 0xff);
                    else
                        individuals[i].emplace(
                            Grid(k, y), enclosing->getRandomInt(1, 0xfe));
                }
            }
        }
    }

    for (auto i = 0; i < generations; i++) {
        // 2. Convert chromosome to path by sequential node appending procedure. We
        // check what nodes are available and choose the one with highest priority.
        // Counter for idividual's. (Used for mapping with the scores)
        auto indsCount = 0;
        // we do want a copy and not a reference. Since we shouldn't modify the real
        // chromosomes in this step.
        for (auto i : individuals) {
            auto blocked = false; // Flag used for marking pendant or dead nodes
            std::vector<Grid> pathObj;
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
                pathObj.emplace_back(currNode);
                auto count = 0;
                // checking whether pendant node
                for (auto i : heap) {
                    if (i.first == 0)
                        count++;
                }
                if (count == heap.size()) {
                    scores[indsCount] = objective(pathObj);
                    // punish for not reaching destination = Bad chromosome,
                    // pendant node, or dead node
                    scores[indsCount] += pathGridSize * 0x35;
                    fitness[indsCount] =
                        calculateFitness(scores[indsCount], pathGridSize);
                    currNode = B;
                    blocked = true;
                }
                i[currNode] = 0x00; // Put lowest priority to already visited nodes.
                if (!blocked) {
                    currNode = heap.rbegin()->second;
                    heap.clear(); // clear the heap. Quite cheap as only 4 elements at
                                  // most
                }
            }
            if (!blocked) {
                pathObj.emplace_back(currNode);
                scores[indsCount] = objective(pathObj);
                // reward good individuals ?
                // scores[indsCount] -= enclosing->getRandomInt(1, scores[indsCount]);
                // 3. Convert the objective function's value to fitness
                fitness[indsCount] = calculateFitness(scores[indsCount], pathGridSize);
            }
            blocked = false;
            indsCount++;
        }

        // 4. Roulette wheel selection
        selection();

        // 5. Uniform crossover
        individuals = crossover();

        // 6. Mutation
        mutation();
    }

    // 7. Convert and format the best path to be returned
    auto i = individuals[maxFitness.first];
    path.clear(); // reset the path from previous individual
    auto currNode(A);
    std::map<uint16_t, Grid> heap; // used for checking pendant nodes
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
        auto count = 0;
        // checking whether pendant node
        for (auto i : heap) {
            if (i.first == 0)
                count++;
        }
        if (count == heap.size()) {
            break; // pendant node we exit
        }
        i[currNode] = 0x00; // Put lowest priority to already visited nodes.
        path.emplace_back(currNode);
        currNode = heap.rbegin()->second;
        heap.clear(); // clear the heap. Quite cheap as only 4 elements at most
    }

    // if (currNode == B) {
    //     path.emplace_back(currNode);
    // }

    if (!path.empty()) {
        path.emplace_back(currNode);
        return path;
    }
    else
        throw std::runtime_error("GA solver did not find any paths.");
}

uint16_t GridMapSpecies::Tree::objective(const std::vector<Grid>& path) const noexcept
{
    uint16_t score = 0;
    for (auto i : path) {
        score += enclosing->weight[i.x][i.y];
    }
    return score;
}

float GridMapSpecies::Tree::calculateFitness(
    const uint32_t objective_score, const uint8_t graphSize) const noexcept
{
    // With objective_score < maxRatio
    // probability to be selected
    // return (static_cast<float>(objective_score) / static_cast<float>(maxRatio *
    // 0xff));

    auto maxFit = 1000.0f / static_cast<float>(graphSize * 0x01);
    auto fit = 1000.0f / static_cast<float>(objective_score);

    return fit / maxFit;
}

void GridMapSpecies::Tree::selection() noexcept
{
    std::set<uint> picked; // use set since it's faster for searching
    float totFitness;

    for (auto& i : fitness)
        totFitness += i;

    // spin the wheel (20% of the size of the population) times, where n is the number
    // of population. We will obtain multiple individuals this way
    for (auto spinTimes = 0; spinTimes < population_size * 0.2; spinTimes++) {
        auto rndNumber = enclosing->getRandomFloat(0.0f, totFitness);
        auto offset = 0.0f;
        for (auto i = 0; i < population_size; i++) {
            offset += fitness[i];
            if (rndNumber < offset) {
                // Check whether already picked or not
                if (picked.find(i) == picked.end())
                    picked.emplace(i);
                break;
            }
        }
    }

    // combined with elitism selection
    maxFitness.second = 0.0f;
    for (auto i = 0; i < population_size; i++) {
        if (fitness[i] > maxFitness.second) {
            maxFitness.first = i;
            maxFitness.second = fitness[i];
        }
    }
    if (picked.find(maxFitness.first) != picked.end()) {
        picked.erase(enclosing->getRandomInt(0, picked.size() - 1));
        picked.emplace(maxFitness.first);
    }

    selected.clear(); // Clear at each new generation
    selected.reserve(picked.size());
    std::copy(picked.begin(), picked.end(), std::back_inserter(selected));
}

std::vector<GridMapSpecies::encoding> GridMapSpecies::Tree::crossover() noexcept
{
    auto rate = 0.4f;
    std::vector<encoding> newGen;
    newGen.reserve(population_size);

    while (true) {
        // randomly choosing 2 parents from the selected individuals
        auto parent1 =
            individuals[selected[enclosing->getRandomInt(0, (selected.size() - 1))]];
        auto parent2 =
            individuals[selected[enclosing->getRandomInt(0, (selected.size() - 1))]];
        encoding child1;
        encoding child2;

        auto it1 = parent1.begin();
        auto it2 = parent2.begin();

        while (it1 != parent1.end()) {
            auto p = enclosing->getRandomFloat(0.0f, 1.0f);
            // just return parents as offspring dependent on the rate or if parents are
            // the same
            if (p > rate || parent1 == parent2) {
                child1 = parent1;
                child2 = parent2;

                // do not mutate old generation ?

                it1++;
                it2++;
            }
            else {
                auto p1 = enclosing->getRandomFloat(0.0f, 1.0f);
                if (p1 < 0.5f) {
                    child1.emplace(*it2);
                    child2.emplace(*it1);
                }
                else {
                    child1.emplace(*it1);
                    child2.emplace(*it2);
                }

                it1++;
                it2++;
            }
        }
        newGen.emplace_back(child1);
        if (newGen.size() == population_size)
            break;
        newGen.emplace_back(child2);
        if (newGen.size() == population_size)
            break;
    }

    return newGen;
}

void GridMapSpecies::Tree::mutation() noexcept
{
    auto rate = 0.2f;

    std::vector<Grid> to_mutate;
    to_mutate.reserve(2);

    auto chromosome_count = 0u;
    for (auto& chromosome : individuals) {
        auto randLocus1 = std::next(
            chromosome.begin(), enclosing->getRandomInt(0, chromosome.size() - 1));
        auto randLocus2 = std::next(
            chromosome.begin(), enclosing->getRandomInt(0, chromosome.size() - 1));

        if (randLocus1 != randLocus2) {
            auto rand = enclosing->getRandomFloat(0.0f, 1.0f);
            if (rand < rate) {
                if (*doNotMutate[0] != randLocus1->first ||
                    *doNotMutate[0] != randLocus2->first ||
                    *doNotMutate[1] != randLocus1->first ||
                    *doNotMutate[1] != randLocus2->first) {
                    to_mutate.emplace_back(randLocus1->first);
                    to_mutate.emplace_back(randLocus2->first);
                }
            }
        }

        if (to_mutate.size() == 2) {
            auto temp = individuals[chromosome_count][to_mutate[0]];
            individuals[chromosome_count][to_mutate[0]] =
                individuals[chromosome_count][to_mutate[1]];
            individuals[chromosome_count][to_mutate[1]] = temp;
        }

        to_mutate.clear();
        chromosome_count++;
    }
}
