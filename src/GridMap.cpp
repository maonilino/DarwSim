#include "GridMap.h"

GridMap* create_grid_map()
{
    return new GridMap();
}

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
    for (auto i = A.y; i < C.y; i++) {
        for (auto k = C.x; k < A.x; k++) {
            weight[k][i] = getRandomInt(1, 32);
        }
    }
}

std::vector<GridMap::Grid> GridMap::randomPath(const Grid& A, const Grid& B) noexcept
{
    // set starting point and finishing point's weights to 0. This way we are sure to
    // end up where we need
    // weight[A.x][A.y] = weight[B.x][B.y] = 0;

    std::vector<GridMap::Grid> path;

    if (A.x < B.x) {
        path.emplace_back(std::move(Grid(A.x, A.y)));
        for (auto i = A.x + 1; i < B.x; i++) {
            path.emplace_back(std::move(Grid(i, getRandomInt(B.y, A.y))));
        }
        path.emplace_back(std::move(Grid(B.x, B.y)));
        return path;
    }
    else if (A.x > B.x) {
        path.emplace_back(std::move(Grid(A.x, A.y)));
        for (auto i = A.x - 1; i > B.x; i--) {
            path.emplace_back(std::move(Grid(i, getRandomInt(B.y, A.y))));
        }
        path.emplace_back(std::move(Grid(B.x, B.y)));
        return path;
    }
    else {
        path.emplace_back(std::move(Grid(A.x, A.y)));
        path.emplace_back(std::move(Grid(B.x, B.y)));
        return path;
    }
}

void GridMap::fillTrees(
    const std::vector<GridMap::Grid>& path, bool increment) noexcept
{
    if (!path.empty()) {
        fill[path.front().x][path.front().y] = true;
        fill[path.back().x][path.back().y] = true;

        for (auto& i : path) {
            auto k = i.y;
            if (increment) {
                while (k <= path.front().y) {
                    fill[i.x][k] = true;
                    k++;
                }
            }
            else {
                while (k != path.back().y) {
                    fill[i.x][k] = true;
                    k--;
                }
                if (k == path.back().y)
                    fill[i.x][k] = true;
            }
        }
    }
}

std::vector<glm::vec2> GridMap::tranformForrest() const noexcept
{
    std::vector<glm::vec2> forrestCoords;
    for (auto x = 0; x < GRID_WEIGHT; x++) {
        for (auto y = 0; y < GRID_HEIGHT; y++) {
            if (fill[x][y]) {
                forrestCoords.emplace_back(
                    glm::vec2(x * gridProjection.x, y * gridProjection.y));
            }
        }
    }
    return forrestCoords;
}

std::vector<glm::vec2> GridMap::generateForrest() noexcept
{
    uint8_t offset;

    // origin of the "forrest"
    Grid O(getRandomInt(0, GRID_WEIGHT - 1), getRandomInt(0, GRID_HEIGHT - 1));

    // get random ofsets used for calculating the 4 random coordinates
    O.y > 0 ? offset = getRandomInt(1, O.y) : offset = 0;
    Grid D(O.x, O.y - offset);

    O.x > 0 ? offset = getRandomInt(1, O.x) : offset = 0;
    Grid L(O.x - offset, O.y);

    O.y < GRID_HEIGHT - 1 ? offset = getRandomInt(1, (GRID_HEIGHT - 1) - O.y)
                          : offset = 0;
    Grid U(O.x, O.y + offset);

    O.x < GRID_WEIGHT - 1 ? offset = getRandomInt(1, (GRID_WEIGHT - 1) - O.x)
                          : offset = 0;
    Grid R(O.x + offset, O.y);

    // Creating random forrest shape
    memset(weight, 0xff, sizeof(weight));
    memset(fill, 0, sizeof(fill));
    fill[O.x][O.y] = true;
    // assignWeights(O, LU);
    auto path(randomPath(L, D));
    // fill[L->x][L->y] = true;
    // fill[D->x][D->y] = true;
    fillTrees(path, true);

    path = randomPath(U, R);
    fillTrees(path, false);

    path =  randomPath(R, D);
    fillTrees(path, true);

    path = randomPath(U, L);
    fillTrees(path, false);

    // fill[U->x][U->y] = true;
    // fill[R->x][R->y] = true;
    auto forrestCoords = tranformForrest();

    return forrestCoords;
}
