#pragma once

#ifdef unix
#include "Graphics.h"
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief
 *
 */
class Map {
  public:
    Map(const Map&) = delete;
    Map& operator=(Map&) = delete;
    Map();
    virtual ~Map() = default;

    virtual std::vector<glm::vec2> generateForrest() noexcept = 0;

  private:
};