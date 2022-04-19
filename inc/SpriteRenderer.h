#pragma once

#ifdef unix
#include "Graphics.h"
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief Sprite Renderer class used for managing the sprite graphics. OpenGL
 * Implementation of the Graphics class. This is a plugin (i.e., implementation could
 * drastically change). Thus, it is necesary to compile this as a shared lib, and link
 * the required constructors with dlsym()
 */
class SpriteRenderer : public Graphics {
  public:
    SpriteRenderer(SpriteRenderer&) = delete;
    SpriteRenderer& operator=(SpriteRenderer&) = delete;

    SpriteRenderer();
    virtual ~SpriteRenderer();

    /**
     * @brief draws all the sprites
     */
    virtual void drawAll(const std::vector<Drawings>& drawingList) noexcept override;
    virtual void drawSingle(const Drawings& drawing) const noexcept override;

  private:
    unsigned int quadVAO;
};

/**
 * @brief Entry point for the Sprite class
 * @param texturePath
 */
extern "C" SpriteRenderer* create_sprite();

extern "C" void delete_sprite(SpriteRenderer* sprite);