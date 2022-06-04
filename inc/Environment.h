#pragma once

#ifdef unix
#include "CLI11.hpp"
#include "Simulation.h"
#include "MapGenerator.h"
#include "SpriteRenderer.h" // mindful of double include (should be ok)
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief Environment class used for setting up the map
 * @attention users of this framework should use dlsym() to link with their plugin
 * functions and verify

 * @todo handle shared library with smart pointers at your own risk. On windows, the
 * heap memory behaves differently between build type
 *
 */
class Environment : public Simulation<OpenGL::Drawings> {
  public:
    Environment(Environment&) = delete;
    Environment& operator=(const Environment&) = delete;

    Environment() = delete;
    Environment(int, char**);
    /**
     * @brief Construct a new Environment object and sets the background of the map
     *
     * @param colour colour of the environment to set
     * @note this class inherits from an abstract class, be mindful of what constructor
     * you need from the base class
     */
    // Environment(const Colour colour);
    virtual ~Environment() = default;

    /**
     * @copydoc Simulation::runSimulation()
     */
    virtual void runSimulation() noexcept override;
    /**
     * @copydoc Simulation::configure()
     */
    virtual void configure() noexcept override;

    CLI::App app; // CLI parser. Public so that it can be accessed in main.

  private:
    void* mapHandle;
    MapGenerator* (*createMapDSA)();
    MapGenerator* (*createMapGA)(const uint16_t, const uint16_t);
    std::unique_ptr<MapGenerator> map = nullptr;

    void* winHandle; // Handle for the dynamic shared object (shared lib)
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
    std::unique_ptr<Window> window; // Aggregation relashionship

    void* graphHandle; // Handle for the dynamic shared object (shared lib)
    /**
     * @brief entry point for the sprite renderer
     * @return pointer to the newly created sprite object on the heap, unique smart
     * pointer
     */
    SpriteRenderer* (*createSpriteRenderer)();
    std::unique_ptr<SpriteRenderer> spriteRenderer; // Aggregation relashionship

    std::string solverString;
    Solver solver = Solver::DSA;
};

/**
 * @brief Entry point of the Simulation implementation for the OpenGL renderer.
 * @param arguments List of arguments for the application to be run. Here, we could
 * pass information about the solver to use, or other benchmarking features
 */
extern "C" Environment* create_environment(int, char**);