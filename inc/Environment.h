#pragma once

#ifdef unix
#include "Simulation.h"
#include "GridMap.h"
#include "SpriteRenderer.h" // mindful of double include (should be ok)
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief Environment class used for setting the map style (brown, yellow or green),
 * and randomly generating the terrain
 *
 * @todo Copyrights license
 */
class Environment : public Simulation<OpenGL::Drawings> {
  public:
    Environment(Environment&) = delete;
    Environment& operator=(const Environment&) = delete;

    /**
     * @brief we don't want a default constructor, as the constructor must be used to
     * initialize the environment colour (green, brown or yellow)
     */
    Environment() = delete;
    /**
     * @brief Construct a new Environment object and sets the background of the map
     *
     * @param colour colour of the environment to set
     * @note this class inherits from an abstract class, be mindful of what constructor
     * you need from the base class
     */
    Environment(const Colour colour);
    virtual ~Environment();

    /**
     * @copydoc Simulation::runSimulation()
     */
    virtual void runSimulation() noexcept override;
    /**
     * @copydoc Simulation::configure()
     */
    virtual void configure() noexcept override;
    /**
     * @copydoc Simulation::addDrawing
     * @details helper function for adding drawings to be rendered in the simulation.
     * must give implementation as it is pure virtual. But in the case of opengl as
     * renderer we can directly call emplace_back() instead.
     *
     */
    // virtual void addDrawing(OpenGL::Drawings&& drawing) noexcept override;

  private:
    void* mapHandle;
    GridMap* (*createMap)();
    std::unique_ptr<GridMap> map = nullptr;

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

    SpriteRenderer* spriteRenderer; // Aggregation relashionship
    Window* window;                 // Aggregation relashionship

    /**
     * @brief entry point for the sprite renderer
     * @return pointer to the newly created sprite object on the heap, unique smart
     * pointer
     */
    SpriteRenderer* (*createSpriteRenderer)();
    void (*deleteSpriteRenderer)(SpriteRenderer*);
};

extern "C" Environment* create_green_environment();

extern "C" void destroy_environment(Environment*);