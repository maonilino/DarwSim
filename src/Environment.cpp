#define STB_IMAGE_IMPLEMENTATION // mindful of the include order! this must be defined
                                 // here before including stb_image!
#include "Environment.h"

Environment* create_green_environment()
{
    return new Environment(Colour::green);
}

void destroy_environment(Environment* env)
{
    delete env;
}

Environment::Environment(const Colour colour)
    : Simulation()
    , map()
{
    mapHandle = dlopen("libDarwSimMap.so", RTLD_LAZY);
    createMap = (GridMap * (*)()) dlsym(mapHandle, "create_grid_map");

    map.reset(createMap());

    this->colour = colour;

    // since this is a temp object, in this case it is an rvalue, thus, we should use
    // move operator to save ressources
    SpriteRenderer::Drawings background("../annex/Textures/green.jpg", false,
        glm::vec2(0.0f, 0.0f), glm::vec2(WIDTH, HEIGHT));
    mapDrawings.emplace_back(std::move(background));

    configure();
}

Environment::~Environment()
{
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
    // SpriteRenderer::Drawings testtree(
    //     "../annex/Textures/tree_1.png", true, glm::vec2(518.0f, 252.0f));
    // mapDrawings.emplace_back(std::move(testtree));

    auto forrest = map->generateForrest();
    for (auto& i : forrest) {
        SpriteRenderer::Drawings tree("../annex/Textures/tree_2.png", true, i);
        mapDrawings.emplace_back(std::move(tree));
    }
}

void Environment::addDrawing(SpriteRenderer::Drawings&& drawing) noexcept
{
    mapDrawings.emplace_back(drawing);
}
