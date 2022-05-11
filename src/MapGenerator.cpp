#include "MapGenerator.h"

MapGenerator* create_grid_map()
{
    return new MapGenerator();
}

MapGenerator::MapGenerator()
{
}

void MapGenerator::fillTrees(const std::vector<Grid>& path, bool increment) noexcept
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

std::vector<glm::vec2> MapGenerator::tranformForrest() const noexcept
{
    std::vector<glm::vec2> forrestCoords;
    for (auto x = 0; x < GRID_WIDTH; x++) {
        for (auto y = 0; y < GRID_HEIGHT; y++) {
            if (fill[x][y]) {
                forrestCoords.emplace_back(x * gridProjection.x, y * gridProjection.y);
            }
        }
    }
    return forrestCoords;
}

std::vector<glm::vec2> MapGenerator::generateForrest() noexcept
{
    uint8_t offset;

    // origin of the "forrest"
    Grid O(getRandomInt(0, GRID_WIDTH - 1), getRandomInt(0, GRID_HEIGHT - 1));

    // get random ofsets used for calculating the 4 random coordinates
    O.y > 0 ? offset = getRandomInt(1, O.y) : offset = 0;
    Grid D(O.x, O.y - offset);

    O.x > 0 ? offset = getRandomInt(1, O.x) : offset = 0;
    Grid L(O.x - offset, O.y);

    O.y < GRID_HEIGHT - 1 ? offset = getRandomInt(1, (GRID_HEIGHT - 1) - O.y)
                          : offset = 0;
    Grid U(O.x, O.y + offset);

    O.x < GRID_WIDTH - 1 ? offset = getRandomInt(1, (GRID_WIDTH - 1) - O.x)
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

glm::vec2 MapGenerator::generateMountains() noexcept
{
    // origin of the mountain
    Grid O(getRandomInt(0, GRID_WIDTH - 1), getRandomInt(0, GRID_HEIGHT - 1));

    // std::map<IncrementDirection, Grid> dir;
    // while (fill[O->x][O->y]) {
    //     if (O->x + 6 < GRID_WIDTH - 1) {
    //         O->x++;
    //         continue;
    //     }
    //     if (O->x - 6 > 0) {
    //         O->x--;
    //         continue;
    //     }
    //     if (O->y + 4 < GRID_HEIGHT - 1) {
    //         O->y++;
    //         continue;
    //     }
    //     if (O->y - 4 > 0) {
    //         O->y--;
    //         continue;
    //     }
    // }
    // if (O->x + 6 < GRID_WIDTH - 1)
    //     dir[IncrementDirection::right] = Grid(O->x + 6, O->y); // right
    // if (O->x - 6 > 0)
    //     dir[IncrementDirection::left] = Grid(O->x - 6, O->y); // left
    // if (O->y + 4 < GRID_HEIGHT - 1)
    //     dir[IncrementDirection::up] = Grid(O->x, O->y + 4); // up
    // if (O->y - 4 > 0)
    //     dir[IncrementDirection::down] = Grid(O->x, O->y - 4); // down

    // auto right = dir.find(IncrementDirection::right);
    // auto left = dir.find(IncrementDirection::left);
    // if (left != dir.end() || right != dir.end()) {}

    return glm::vec2(O.x * gridProjection.x, O.y * gridProjection.y);
}