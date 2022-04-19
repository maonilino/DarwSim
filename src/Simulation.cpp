#include "Simulation.h"

Simulation::Simulation()
{
    winHandle = dlopen("./src/libDarwSimWin.so", RTLD_LAZY);
    createWindow =
        (Window * (*)(const char* title, const GLint width, const GLint height))
            dlsym(winHandle, "create_window");
    deleteWindow = (void (*)(Window*))dlsym(winHandle, "destroy_window");

    graphHandle = dlopen("./src/libDarwSimSprite.so", RTLD_LAZY);
    createSpriteRenderer =
        (SpriteRenderer * (*)()) dlsym(graphHandle, "create_sprite");
    deleteSpriteRenderer =
        (void (*)(SpriteRenderer*))dlsym(graphHandle, "delete_sprite");

    window = (Window*)createWindow("DarwSim Viewer", WIDTH, HEIGHT);
    spriteRenderer = (SpriteRenderer*)createSpriteRenderer();
}

void Simulation::runSimulation() noexcept
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

void Simulation::setArguments(const std::vector<std::string>& arguments)
{
    for (auto& i : arguments) {
        if (i == "--colour")
            continue;
        if (i == "green")
            colour = Colour::green;
        if (i == "brown")
            colour = Colour::brown;
        if (i == "yellow")
            colour = Colour::yellow;
    }
}

Simulation::~Simulation()
{
    deleteSpriteRenderer(spriteRenderer);
    deleteWindow(window);
    // destroyWindow(window);
}