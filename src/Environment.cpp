#define STB_IMAGE_IMPLEMENTATION // mindful of the include order! this must be defined
                                 // here before including stb_image!
#include "Environment.h"

Environment* create_environment(std::vector<std::string>& arguments)
{
    return new Environment(arguments);
}

Environment::Environment(std::vector<std::string>& arguments)
    : Simulation<OpenGL::Drawings>()
    , map()
{
    bool solver = 0;
    bool ga = 0;
    for (auto& i : arguments) {
        if (i == "--solver") {
            solver = true;
            continue;
        }
        else if ((i == "dsa" || i == "DSA") && solver)
            options["solver"] = static_cast<uint8_t>(Solver::DSA);
        else if ((i == "ga" || i == "GA") && solver) {
            ga = true;
            options["solver"] = static_cast<uint8_t>(Solver::GA);
        }
        else if (solver && ga) {
            try {
                options["population"] = std::stoi(i);
            }
            catch (const std::exception& e) {
                std::string error{
                    i + ", invalid argument for ga solver. Please specify a numeral"};
                throw std::invalid_argument(error);
            }
        }
        else if(!solver) {
            throw std::invalid_argument("Unknown argument: " + i);
        }
    }

    winHandle = dlopen("./src/libDarwSimWin.so", RTLD_LAZY);
    if (!winHandle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    createWindow =
        (Window * (*)(const char* title, const GLint width, const GLint height))
            dlsym(winHandle, "create_window");

    graphHandle = dlopen("./src/libDarwSimSprite.so", RTLD_LAZY);
    if (!graphHandle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    createSpriteRenderer =
        (SpriteRenderer * (*)()) dlsym(graphHandle, "create_sprite");

    mapHandle = dlopen("./src/libDarwSimMap.so", RTLD_LAZY);
    if (!mapHandle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    createMapDSA = (MapGenerator * (*)()) dlsym(mapHandle, "create_dsa_map");
    createMapGA =
        (MapGenerator * (*)(const uint16_t)) dlsym(mapHandle, "create_ga_map");

    window.reset(createWindow("DarwSim Viewer", WIDTH, HEIGHT));
    spriteRenderer.reset(createSpriteRenderer());
    if (options.contains("solver")) {
        if (options["solver"] == static_cast<uint8_t>(Solver::DSA)) {
            map.reset(createMapDSA());
        }
        else if (options["solver"] == static_cast<uint8_t>(Solver::GA)) {
            if (options.contains("population"))
                map.reset(createMapGA(options["population"]));
            else
                throw std::invalid_argument(
                    "Population size not specified for the GA solver");
        }
    }
    else
        map.reset(createMapDSA());

    // since this is a temp object, in this case it is an rvalue, thus, we should
    // use move operator to save ressources
    mapDrawings.emplace_back("../annex/Textures/map_Texture.jpg", false,
        glm::vec2(0.0f, 0.0f), glm::vec2(WIDTH, HEIGHT));

    configure();
}

// Environment::~Environment()
// {
//     // deleteSpriteRenderer(spriteRenderer);
//     // deleteWindow(window);
// }

void Environment::runSimulation() noexcept
{
    // render loop
    // -----------
    while (!window->windowShouldClose()) {
        // input
        // -----

        // render
        // ------
        // following line creates leaks with glew?
        // spriteRenderer->clearBackground(colour);

        // draw
        // ----
        spriteRenderer->drawAll(mapDrawings);

        // // glfw: swap buffers and poll IO events
        window->swapBuffers();
    }
}

void Environment::configure() noexcept
{
    OpenGL::Drawings testmountain("../annex/Textures/mountains.png", true,
        map->generateMountains(), glm::vec2(252.0f, 224.0f));
    mapDrawings.emplace_back(std::move(testmountain));

    std::random_device rd;
    std::mt19937 mt(rd());

    Solver solver = Solver::DSA;
    if (options.contains("solver")) {
        switch (options["solver"]) {
        case 0:
            solver = Solver::DSA;
            break;

        case 1:
            solver = Solver::GA;
            break;

        default:
            break;
        }
    }
    auto forrest = map->generateForrest(solver);
    for (auto& i : forrest) {
        std::uniform_int_distribution<uint8_t> dist(1, 3);
        auto texture = "../annex/Textures/tree_" + std::to_string(dist(mt)) + ".png";
        mapDrawings.emplace_back(texture.c_str(), true, i);
    }

    // population.emplace_back(createTrees());
}
