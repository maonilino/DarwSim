#pragma once

#ifdef unix
#include "Simulation.h"
#include "GridMap.h"
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
class Environment : public Simulation {
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
    virtual void addDrawing(SpriteRenderer::Drawings&& drawing) noexcept override;

  private:
    void* mapHandle;
    GridMap* (*createMap)();
    std::unique_ptr<GridMap> map = nullptr;
};

extern "C" Environment* create_green_environment();

extern "C" void destroy_environment(Environment*);