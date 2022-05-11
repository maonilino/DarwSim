#pragma once

#ifdef unix
#include <dlfcn.h>
#include <vector>
#include <unordered_map>
#include "Shader.hpp"
#include "Texture2D.hpp"
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief Base graphics class used for renderers. This is the building block for the
 * renderer plugin(s).
 * @attention Fremwork user should implement custom drawings datatypes
 * @attention It is necesary to compile this (together with the plugin implementation)
 * as a shared lib, and link the required constructors with dlsym()
 */
template <typename drawing_type> class Graphics {
  public:
    Graphics(Graphics&) = delete;            // Disable copy constructor
    Graphics& operator=(Graphics&) = delete; // Disable copy assignment operator

    /**
     * @brief Construct a new Graphics object. Useed for testing purposes
     *
     * @param vertices vertices coordinate
     * @param indices indices for EBO
     *
     */
    Graphics(std::vector<float>& vertices, std::vector<unsigned int>& indices);
    Graphics() = default;

    virtual ~Graphics() = default;

    /**
     * @brief Clear colour background
     * @note not necessarily needed as we use textured background
     */
    // virtual void clearBackground(Colour colour) const noexcept;
    /**
     * @brief draws the elements in the buffers
     */
    virtual void drawAll(const std::vector<drawing_type>& drawingList) noexcept = 0;
    /**
     * @brief draws a single object
     * @param drawing struct of the object to draw
     */
    virtual void drawSingle(const drawing_type& drawing) const noexcept = 0;

  protected:
};