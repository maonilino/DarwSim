#pragma once

#ifdef unix
#include "Graphics.h"
#include "SpriteRenderer.h" // mindful of double include (should be ok)
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief abstract class for giving the interface of the functions that need to be
 * implemented by the environment.
 * @attention users of this framework should use dlsym() to link with their plugin
 * functions and verify
 *
 * @todo handle shared library with smart pointers at your own risk. On windows, the
 * heap memory behaves differently between build type
 */
class Simulation {
  public:
    Simulation(Simulation&) = delete;
    Simulation& operator=(Simulation&) = delete;

    Simulation();
    virtual ~Simulation();

    /**
     * @brief Set the arguments for the simulation
     * @param options array of strings containing arguments
     */
    virtual void setArguments(const std::vector<std::string>& options);

    /**
     * @brief Main simulation loop. Here we run the program.
     *
     */
    virtual void runSimulation() noexcept = 0;
    /**
     * @brief configure the map/predator and other inheritants. We generate the random
     * terrain, create the random species...
     */
    virtual void configure() noexcept = 0;
    /**
     * @brief Adds a drawing to the hash
     */
    virtual void addDrawing(SpriteRenderer::Drawings&& drawing) noexcept = 0;

  private:
    /**
     * @brief Function pointer for the entry point of the window
     *
     * @param title-title of the window
     * @param width width of the window
     * @param height height of the window
     *
     * @return std::unique_ptr<Window>: unique pointer to the newly created window obj
     */
    Window* (*createWindow)(const char* title, const GLint width, const GLint height);
    void (*deleteWindow)(Window*);

    void* winHandle;   // Handle for the dynamic shared object (shared lib)
    void* graphHandle; // Handle for the dynamic shared object (shared lib)

  protected:
    /**
     * @brief entry point for the sprite renderer
     * @return pointer to the newly created sprite object on the heap, unique smart
     * pointer
     */
    SpriteRenderer* (*createSpriteRenderer)();
    void (*deleteSpriteRenderer)(SpriteRenderer*);

    Window* window;                 // Aggregation relashionship
    SpriteRenderer* spriteRenderer; // Aggregation relashionship

    Colour colour; // Holding the colour for the currently active simulation

    // array for storing all the drawings for the map. Should be
    // in this class so that we have a single runSimulation fuction
    std::vector<Graphics::Drawings> mapDrawings;
};
