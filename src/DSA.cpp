#include "DSA.h"

std::vector<GridMap::Grid> DSA::randomPath(const Grid& A, const Grid& B) noexcept
{
    std::vector<Grid> path;

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

std::vector<GridMap::Grid> DSA::dsp(Grid& A, Grid& B) noexcept
{
    // set starting point and finishing point's weights to 0. This way we are sure to
    // end up where we need
    weight[A.x][A.y] = weight[B.x][B.y] = 0;

    // if (A.y == B.y) // there is no Up or down point, there is no path
    //     return std::vector<Grid> {0};

    // keeping track of the paths
    std::vector<Grid> path;
    std::unordered_map<Grid, uint16_t, GridMap::Grid::HashFunction> verticesCost;
    // open list.
    std::multimap<uint16_t, Grid> list;

    list.emplace(std::pair<uint16_t, Grid>(0, A));
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
        if (C->second.x < GRID_WIDTH - 1) {
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

    // do not use forrange loop. UB if so. Because of iterator invalidation
    for (auto i = path.size() - 1; i != 0; i--) {
        if ((i - 1) != 0) {
            if ((path[i - 1].x - path[i].x) > 1 || (path[i - 1].y - path[i].y) > 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i].x - path[i - 1].x) > 1 ||
                     path[i].y - path[i - 1].y > 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i - 1].x - path[i].x) == 1 &&
                     (path[i - 1].y - path[i].y) == 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i].x - path[i - 1].x) == 1 &&
                     path[i].y - path[i - 1].y == 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i].x - path[i - 1].x) == 1 &&
                     path[i - 1].y - path[i].y == 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i - 1].x - path[i].x) == 1 &&
                     path[i].y - path[i - 1].y == 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
            else if ((path[i - 1].x - path[i].x) == 1 &&
                     path[i - 1].y - path[i].y == 1) {
                path.erase(path.begin() + (i - 1));
                i++;
            }
        }
    }
    return path;
}