#pragma once

#ifdef unix
#include "Graphics.h"
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief
 */
template<typename coordinate_type>
class Map {
  public:
    Map(const Map&) = delete;
    Map& operator=(Map&) = delete;
    Map() = default;
    virtual ~Map() = default;
    
    virtual std::vector<coordinate_type> generateForrest() noexcept = 0;

  private:
};