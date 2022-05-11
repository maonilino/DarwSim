#define STB_IMAGE_IMPLEMENTATION // mindful of the include order! this must be defined
                                 // here before including stb_image!
#include "Environment.h"

Environment* create_environment()
{
    return new Environment();
}

Environment::Environment()
    : Simulation<OpenGL::Drawings>()
    , map()
{
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
    createMap = (MapGenerator * (*)()) dlsym(mapHandle, "create_grid_map");

    window.reset(createWindow("DarwSim Viewer", WIDTH, HEIGHT));
    spriteRenderer.reset(createSpriteRenderer());
    map.reset(createMap());

    // since this is a temp object, in this case it is an rvalue, thus, we should use
    // move operator to save ressources
    mapDrawings.emplace_back("../annex/Textures/map_Texture.jpg", false,
        glm::vec2(0.0f, 0.0f), glm::vec2(WIDTH, HEIGHT));

    configure();
}

Environment::~Environment()
{
    // deleteSpriteRenderer(spriteRenderer);
    // deleteWindow(window);
}

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

    auto forrest = map->generateForrest();
    for (auto& i : forrest) {
        std::uniform_int_distribution<uint8_t> dist(1, 3);
        auto texture = "../annex/Textures/tree_" + std::to_string(dist(mt)) +
        ".png"; mapDrawings.emplace_back(texture.c_str(), true, i);
    }

    // population.emplace_back(createTrees());
}
