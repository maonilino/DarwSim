#pragma once

#ifdef unix
#include "Graphics.h"
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif


// Abstract class for giving the interface of the functions that need to be
// implemented by the environment.
template <typename drawing_type> class Simulation {
  public:
    Simulation(Simulation&) = delete;
    Simulation& operator=(Simulation&) = delete;

    Simulation() = default;
    virtual ~Simulation() = default;

    /**
     * @brief Set the arguments for the simulation
     * @param options array of strings containing arguments
     */
    // virtual void setArguments(const std::vector<std::string>& options);

    // Main simulation loop. Here we run the program.
    virtual void runSimulation() noexcept = 0;
    /**
     * @brief configure the map and other inheritants. We generate the random
     * terrain, create the random species...
     */
    virtual void configure() noexcept = 0;

  private:

  protected:
    // array for storing all the drawings for the map. Should be
    // in this class so that we have a single runSimulation fuction
    std::vector<drawing_type> mapDrawings;
};
