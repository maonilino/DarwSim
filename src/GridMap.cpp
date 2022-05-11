#include "GridMap.h"

GridMap::GridMap()
    : gridProjection(21, 28)
{
}

uint8_t GridMap::getRandomInt(uint8_t min, uint8_t max) const noexcept
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint8_t> dist(min, max);

    return dist(mt);
}

void GridMap::assignWeights(const Grid& A, const Grid& C) noexcept
{
    for (int16_t i = A.y; i >= C.y; i--) {
        for (int16_t k = C.x; k <= A.x; k++) {
            weight[k][i] = getRandomInt(1, 32);
        }
    }
}