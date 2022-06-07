#include "MapGenerator.h"

MapGenerator* create_ga_map(const uint16_t populationSize, const uint16_t genSize)
{
    return new MapGenerator(populationSize, genSize);
}

MapGenerator* create_dsa_map()
{
    return new MapGenerator();
}

MapGenerator::MapGenerator(const uint16_t populationSize, const uint16_t genSize)
    : GridMapSpecies::Tree::Tree(populationSize, genSize)
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

std::vector<glm::vec2> MapGenerator::generateForrest(Solver selectedSolver)
{
    std::fstream fileHandler;
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
    memset(&weight[0], 0xff, sizeof(weight[0]) * weight.size());
    std::vector<GridMap::Grid> path;

    if (selectedSolver == Solver::DSA) {
        fileHandler.open(
            "../data.csv", std::ios::in | std::ios::out | std::ios::trunc);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for write");

        fileHandler << std::to_string(O.x) << "," << std::to_string(O.y) << "\n"
                    << std::to_string(D.x) << "," << std::to_string(D.y) << "\n"
                    << std::to_string(L.x) << "," << std::to_string(L.y) << "\n"
                    << std::to_string(U.x) << "," << std::to_string(U.y) << "\n"
                    << std::to_string(R.x) << "," << std::to_string(R.y) << "\n";

        fill[O.x][O.y] = true;

        Grid LD(L.x, D.y);
        // set grid weights, everything arround this square has weights of 255
        assignWeights(O, LD);

        for (auto x = 0; x < GRID_WIDTH; x++) {
            for (auto y = 0; y < GRID_HEIGHT; y++) {
                fileHandler << std::to_string(weight[x][y]) << ",";
            }
        }
        fileHandler << "\n";

        fileHandler.close();

        path = dsp(L, D);
    }
    else if (selectedSolver == Solver::GA) {
        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line3;

        Go2Line(fileHandler, 2);
        fileHandler >> line3;

        std::stringstream convertor(line3);
        std::string token; // somewhere to put the comma separated value
        std::vector<uint8_t> Larr;
        Larr.reserve(2);
        while (std::getline(convertor, token, ',')) {
            Larr.emplace_back(std::atoi(token.c_str()));
        }

        fileHandler.close();

        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line2;

        Go2Line(fileHandler, 1);
        fileHandler >> line2;

        std::stringstream convertor2(line2);
        std::string token2; // somewhere to put the comma separated value
        std::vector<uint8_t> Darr;
        Darr.reserve(2);
        while (std::getline(convertor2, token2, ',')) {
            Darr.emplace_back(std::atoi(token2.c_str()));
        }

        fileHandler.close();

        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line6;

        Go2Line(fileHandler, 5);
        fileHandler >> line6;

        std::stringstream convertor3(line6);
        std::string tokens; // somewhere to put the comma separated value
        std::array<std::array<uint8_t, GRID_HEIGHT>, GRID_WIDTH> tempWeight;
        uint x = 0;
        uint y = 0;
        while (std::getline(convertor3, tokens, ',')) {
            if (y < GRID_HEIGHT) {
                tempWeight[x][y] = std::atoi(tokens.c_str());
                y++;
            }
            else if (x < GRID_WIDTH) {
                y = 0;
                x++;
                tempWeight[x][y] = std::atoi(tokens.c_str());
                y++;
            }
        }

        fileHandler.close();

        Grid tempL(Larr[0], Larr[1]);
        Grid tempD(Darr[0], Darr[1]);

        path = GCW_gasp(tempL, tempD, tempWeight);
        auto dbg = 0;
    }
    fillTrees(path, true);
    memset(&weight[0], 0xff, sizeof(weight[0]) * weight.size());

    if (selectedSolver == Solver::DSA) {
        fileHandler.open("../data.csv", std::ios::in | std::ios::out | std::ios::app);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for write");
        Grid RU(R.x, U.y);
        assignWeights(RU, O);

        for (auto x = 0; x < GRID_WIDTH; x++) {
            for (auto y = 0; y < GRID_HEIGHT; y++) {
                fileHandler << std::to_string(weight[x][y]) << ",";
            }
        }
        fileHandler << "\n";

        fileHandler.close();

        path = dsp(U, R);
    }
    else if (selectedSolver == Solver::GA) {
        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line4;

        Go2Line(fileHandler, 3);
        fileHandler >> line4;

        std::stringstream convertor(line4);
        std::string token; // somewhere to put the comma separated value
        std::vector<uint8_t> Uarr;
        Uarr.reserve(2);
        while (std::getline(convertor, token, ',')) {
            Uarr.emplace_back(std::atoi(token.c_str()));
        }

        fileHandler.close();

        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line5;

        Go2Line(fileHandler, 4);
        fileHandler >> line5;

        std::stringstream convertor2(line5);
        std::string token2; // somewhere to put the comma separated value
        std::vector<uint8_t> Rarr;
        Rarr.reserve(2);
        while (std::getline(convertor2, token2, ',')) {
            Rarr.emplace_back(std::atoi(token2.c_str()));
        }

        fileHandler.close();

        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line7;

        Go2Line(fileHandler, 6);
        fileHandler >> line7;

        std::stringstream convertor3(line7);
        std::string tokens; // somewhere to put the comma separated value
        std::array<std::array<uint8_t, GRID_HEIGHT>, GRID_WIDTH> tempWeight;
        uint x = 0;
        uint y = 0;
        while (std::getline(convertor3, tokens, ',')) {
            if (y < GRID_HEIGHT) {
                tempWeight[x][y] = std::atoi(tokens.c_str());
                y++;
            }
            else if (x < GRID_WIDTH) {
                y = 0;
                x++;
                tempWeight[x][y] = std::atoi(tokens.c_str());
                y++;
            }
        }

        fileHandler.close();

        Grid tempU(Uarr[0], Uarr[1]);
        Grid tempR(Rarr[0], Rarr[1]);

        path = GCW_gasp(tempU, tempR, tempWeight);
    }
    fillTrees(path, false);
    memset(&weight[0], 0xff, sizeof(weight[0]) * weight.size());

    if (selectedSolver == Solver::DSA) {
        fileHandler.open("../data.csv", std::ios::in | std::ios::out | std::ios::app);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for write");
        assignWeights(R, D);

        for (auto x = 0; x < GRID_WIDTH; x++) {
            for (auto y = 0; y < GRID_HEIGHT; y++) {
                fileHandler << std::to_string(weight[x][y]) << ",";
            }
        }
        fileHandler << "\n";

        fileHandler.close();

        path = dsp(R, D);
    }
    else if (selectedSolver == Solver::GA) {
        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line5;

        Go2Line(fileHandler, 4);
        fileHandler >> line5;

        std::stringstream convertor(line5);
        std::string token; // somewhere to put the comma separated value
        std::vector<uint8_t> Rarr;
        Rarr.reserve(2);
        while (std::getline(convertor, token, ',')) {
            Rarr.emplace_back(std::atoi(token.c_str()));
        }

        fileHandler.close();

        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line2;

        Go2Line(fileHandler, 1);
        fileHandler >> line2;

        std::stringstream convertor2(line2);
        std::string token2; // somewhere to put the comma separated value
        std::vector<uint8_t> Darr;
        Darr.reserve(2);
        while (std::getline(convertor2, token2, ',')) {
            Darr.emplace_back(std::atoi(token2.c_str()));
        }

        fileHandler.close();

        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line8;

        Go2Line(fileHandler, 7);
        fileHandler >> line8;

        std::stringstream convertor3(line8);
        std::string tokens; // somewhere to put the comma separated value
        std::array<std::array<uint8_t, GRID_HEIGHT>, GRID_WIDTH> tempWeight;
        uint x = 0;
        uint y = 0;
        while (std::getline(convertor3, tokens, ',')) {
            if (y < GRID_HEIGHT) {
                tempWeight[x][y] = std::atoi(tokens.c_str());
                y++;
            }
            else if (x < GRID_WIDTH) {
                y = 0;
                x++;
                tempWeight[x][y] = std::atoi(tokens.c_str());
                y++;
            }
        }

        fileHandler.close();

        Grid tempR(Rarr[0], Rarr[1]);
        Grid tempD(Darr[0], Darr[1]);

        path = GCW_gasp(tempR, tempD, tempWeight);
    }
    fillTrees(path, true);
    memset(&weight[0], 0xff, sizeof(weight[0]) * weight.size());

    if (selectedSolver == Solver::DSA) {
        fileHandler.open("../data.csv", std::ios::in | std::ios::out | std::ios::app);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for write");
        assignWeights(U, L);

        for (auto x = 0; x < GRID_WIDTH; x++) {
            for (auto y = 0; y < GRID_HEIGHT; y++) {
                fileHandler << std::to_string(weight[x][y]) << ",";
            }
        }
        fileHandler << "\n";

        fileHandler.close();

        path = dsp(U, L);
    }
    else if (selectedSolver == Solver::GA) {
        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line4;

        Go2Line(fileHandler, 3);
        fileHandler >> line4;

        std::stringstream convertor(line4);
        std::string token; // somewhere to put the comma separated value
        std::vector<uint8_t> Uarr;
        Uarr.reserve(2);
        while (std::getline(convertor, token, ',')) {
            Uarr.emplace_back(std::atoi(token.c_str()));
        }

        fileHandler.close();

        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line3;

        Go2Line(fileHandler, 2);
        fileHandler >> line3;

        std::stringstream convertor1(line3);
        std::string tokenn; // somewhere to put the comma separated value
        std::vector<uint8_t> Larr;
        Larr.reserve(2);
        while (std::getline(convertor1, tokenn, ',')) {
            Larr.emplace_back(std::atoi(tokenn.c_str()));
        }

        fileHandler.close();

        fileHandler.open("../data.csv", std::ios::in);

        if (!fileHandler)
            throw std::runtime_error("Unable to open file for read");

        std::string line9;

        Go2Line(fileHandler, 8);
        fileHandler >> line9;

        std::stringstream convertor3(line9);
        std::string tokens; // somewhere to put the comma separated value
        std::array<std::array<uint8_t, GRID_HEIGHT>, GRID_WIDTH> tempWeight;
        uint x = 0;
        uint y = 0;
        while (std::getline(convertor3, tokens, ',')) {
            if (y < GRID_HEIGHT) {
                tempWeight[x][y] = std::atoi(tokens.c_str());
                y++;
            }
            else if (x < GRID_WIDTH) {
                y = 0;
                x++;
                tempWeight[x][y] = std::atoi(tokens.c_str());
                y++;
            }
        }

        fileHandler.close();

        Grid tempL(Larr[0], Larr[1]);
        Grid tempU(Uarr[0], Uarr[1]);

        path = GCW_gasp(tempU, tempL, tempWeight);
    }
    fillTrees(path, false);
    memset(&weight[0], 0xff, sizeof(weight[0]) * weight.size());

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