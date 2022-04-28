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
    for (int16_t i = A.y; i >= C.y; i--) {
        for (int16_t k = C.x; k <= A.x; k++) {
            weight[k][i] = getRandomInt(1, 32);
        }
    }
}

std::vector<GridMap::Grid> GridMap::randomPath(const Grid& A, const Grid& B) noexcept
{
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

std::vector<GridMap::Grid> GridMap::dsp(Grid& A, Grid& B) noexcept
{
    // set starting point and finishing point's weights to 0. This way we are sure to
    // end up where we need
    weight[A.x][A.y] = weight[B.x][B.y] = 0;

    // if (A.y == B.y) // there is no Up or down point, there is no path
    //     return std::vector<GridMap::Grid> {0};

    // keeping track of the paths
    std::vector<Grid> path;
    std::unordered_map<GridMap::Grid, uint16_t, GridMap::Grid::HashFunction>
        verticesCost;
    // open list.
    std::multimap<uint16_t, GridMap::Grid> list;

    list.emplace(std::pair<uint16_t, GridMap::Grid>(0, A));
    std::vector<Grid> temp;
    temp.emplace_back(A);

    verticesCost[A] = 0;

    bool search = true;

    while (search) {
        auto C = list.begin();
        path.emplace_back(C->second);

        // visiting nodes and adding them to the open list
        if (C->second.y < GRID_HEIGHT - 1) {
            Grid temp(C->second.x, C->second.y + 1);
            if (verticesCost.contains(temp)) {
                if (verticesCost[std::move(temp)] >
                    (C->first + weight[temp.x][temp.y])) {
                    verticesCost[std::move(temp)] =
                        C->first + weight[temp.x][temp.y + 1];
                    list.emplace(std::move(std::pair<uint16_t, Grid>(
                        verticesCost[std::move(temp)], std::move(temp))));
                }
            }
            else {
                verticesCost[std::move(temp)] = C->first + weight[temp.x][temp.y];
                list.emplace(std::move(std::pair<uint16_t, Grid>(
                    verticesCost[std::move(temp)], std::move(temp))));
            }
        }
        if (C->second.y > 0) {
            Grid temp(C->second.x, C->second.y - 1);
            if (verticesCost.contains(temp)) {
                if (verticesCost[std::move(temp)] >
                    (C->first + weight[temp.x][temp.y])) {
                    verticesCost[std::move(temp)] = C->first + weight[temp.x][temp.y];
                    list.emplace(std::move(std::pair<uint16_t, Grid>(
                        verticesCost[std::move(temp)], std::move(temp))));
                }
            }
            else {
                verticesCost[std::move(temp)] = C->first + weight[temp.x][temp.y];
                list.emplace(std::move(std::pair<uint16_t, Grid>(
                    verticesCost[std::move(temp)], std::move(temp))));
            }
        }
        if (C->second.x < GRID_WEIGHT - 1) {
            Grid temp(C->second.x + 1, C->second.y);
            if (verticesCost.contains(temp)) {
                if (verticesCost[std::move(temp)] >
                    (C->first + weight[temp.x][temp.y])) {
                    verticesCost[std::move(temp)] = C->first + weight[temp.x][temp.y];
                    list.emplace(std::move(std::pair<uint16_t, Grid>(
                        verticesCost[std::move(temp)], std::move(temp))));
                }
            }
            else {
                verticesCost[std::move(temp)] = C->first + weight[temp.x][temp.y];
                list.emplace(std::move(std::pair<uint16_t, Grid>(
                    verticesCost[std::move(temp)], std::move(temp))));
            }
        }
        if (C->second.x > 0) {
            Grid temp(C->second.x - 1, C->second.y);
            if (verticesCost.contains(temp)) {
                if (verticesCost[std::move(temp)] >
                    (C->first + weight[temp.x][temp.y])) {
                    verticesCost[std::move(temp)] =
                        C->first + weight[C->second.x - 1][C->second.y];
                    list.emplace(std::move(std::pair<uint16_t, Grid>(
                        verticesCost[std::move(temp)], std::move(temp))));
                }
            }
            else {
                verticesCost[std::move(temp)] = C->first + weight[temp.x][temp.y];
                list.emplace(std::move(std::pair<uint16_t, Grid>(
                    verticesCost[std::move(temp)], std::move(temp))));
            }
        }

        weight[C->second.x][C->second.y] = 0xff;
        list.erase(C);
        if (list.begin()->second == B) {
            search = false;
            path.emplace_back(list.begin()->second);
        }
    }

    for (auto i = path.size() - 1; i != 0; i--) {
        if ((i - 1) != 0) {
            if ((path[i - 1].x - path[i].x) > 1 || (path[i - 1].y - path[i].y) > 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i].x - path[i - 1].x) > 1 || path[i].y - path[i - 1].y > 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i - 1].x - path[i].x) == 1 && (path[i - 1].y - path[i].y) == 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i].x - path[i - 1].x) == 1 && path[i].y - path[i - 1].y == 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i].x - path[i - 1].x) == 1 && path[i - 1].y - path[i].y == 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i - 1].x - path[i].x) == 1 && path[i].y - path[i - 1].y == 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i - 1].x - path[i].x) == 1 && path[i - 1].y - path[i].y == 1) {
                path.erase(path.begin() + (i - 1));
                i++;

            }
        }
    }
    return path;
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
                forrestCoords.emplace_back(x * gridProjection.x, y * gridProjection.y);
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

    Grid LD(L.x, D.y);
    // set grid weights, everything arround this square has weights of 255
    assignWeights(O, LD);
    auto path(dsp(L, D));
    // auto path(randomPath(L, D));
    // fill[L->x][L->y] = true;
    // fill[D->x][D->y] = true;
    fillTrees(path, true);
    memset(weight, 0xff, sizeof(weight)); // reset grid weights

    Grid RU(R.x, U.y);
    assignWeights(RU, O);
    path = dsp(U, R);
    fillTrees(path, false);
    memset(weight, 0xff, sizeof(weight)); // reset grid weights

    assignWeights(R, D);
    path = dsp(R, D);
    fillTrees(path, true);
    memset(weight, 0xff, sizeof(weight));
    // path = randomPath(R, D);

    assignWeights(U, L);
    path = dsp(U, L);
    fillTrees(path, false);
    memset(weight, 0xff, sizeof(weight));
    // path = randomPath(U, L);

    // fill[U->x][U->y] = true;
    // fill[R->x][R->y] = true;
    auto forrestCoords = tranformForrest();

    return forrestCoords;
}
