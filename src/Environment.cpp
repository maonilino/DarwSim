#define STB_IMAGE_IMPLEMENTATION // mindful of the include order! this must be defined
                                 // here before including stb_image!
#include "Environment.h"

Environment* create_environment(int argc, char** argv)
{
    return new Environment(argc, argv);
}

Environment::Environment(int argc, char** argv)
    : Simulation<OpenGL::Drawings>()
    , map()
    , app{"Welcome to DarwSim", "DarwSim"}
{
    uint16_t popSize = 23;
    uint16_t generations = 43;
    app.add_option("-s,--solver", solverString,
           "Solver to be used (DSA or GA). DSA by default")
        ->default_str("DSA");
    app.add_flag(
           "--gen-size", generations, "Number of generations to use. 43 By default")
        ->expected(1, 999)
        ->default_val(43);
    app.add_flag("--pop-size", popSize, "Number of initial populations. 23 by default")
        ->expected(10, 300)
        ->default_val(23);

    try {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError& e) {
        exit(app.exit(e));
    }

    if (solverString == "DSA" || solverString == "dsa")
        solver = Solver::DSA;
    else if (solverString == "GA" || solverString == "ga")
        solver = Solver::GA;
    else {
        throw std::invalid_argument(
            "Available solvers: GA, DSA. Solver unknown: " + solverString);
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
    createMapGA = (MapGenerator * (*)(const uint16_t, const uint16_t))
        dlsym(mapHandle, "create_ga_map");

    window.reset(createWindow("DarwSim Viewer", WIDTH, HEIGHT));
    spriteRenderer.reset(createSpriteRenderer());
    if (solver == Solver::DSA) {
        map.reset(createMapDSA());
    }
    else if (solver == Solver::GA) {
        map.reset(createMapGA(popSize, generations));
    }

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

    auto forrest = map->generateForrest(solver);
    for (auto& i : forrest) {
        std::uniform_int_distribution<uint8_t> dist(1, 3);
        auto texture = "../annex/Textures/tree_" + std::to_string(dist(mt)) + ".png";
        mapDrawings.emplace_back(texture.c_str(), true, i);
    }

    // population.emplace_back(createTrees());
}
