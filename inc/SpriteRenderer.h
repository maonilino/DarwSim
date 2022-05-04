#pragma once

#ifdef unix
#include "Graphics.h"
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief Drawings datatype. This structure is
 * only suitable for openGL rendere
 */
namespace OpenGL {
struct Drawings {
    Texture2D texture;
    bool alpha;
    glm::vec2 position;
    glm::vec2 size;
    float rotate;
    glm::vec3 color;

    Drawings(const char* texturePath, bool alph, glm::vec2 pos,
        glm::vec2 siz = glm::vec2(21.0f, 28.0f), float rot = 0.0f,
        glm::vec3 colr = glm::vec3(1.0f))
        : texture(texturePath, alph)
        , alpha(alph)
        , position(pos)
        , size(siz)
        , rotate(rot)
        , color(colr)
    {
    }

    Drawings() = delete;
    Drawings(const Drawings&) = delete;
    Drawings& operator=(const Drawings&) = delete;
    Drawings(Drawings&&) = default;
    Drawings& operator=(Drawings&&) = delete;
};
} // namespace OpenGL

/**
 * @brief Sprite Renderer class used for managing the sprite graphics. OpenGL
 * Implementation of the Graphics class. This is a plugin (i.e., implementation could
 * drastically change). Thus, it is necesary to compile this as a shared lib, and link
 * the required constructors with dlsym()
 */
class SpriteRenderer : public Graphics<OpenGL::Drawings> {
  public:
    SpriteRenderer(SpriteRenderer&) = delete;
    SpriteRenderer& operator=(SpriteRenderer&) = delete;

    SpriteRenderer();
    virtual ~SpriteRenderer();

    // draws all the sprites
    virtual void drawAll(const std::vector<OpenGL::Drawings>& drawingList) noexcept override;
    virtual void drawSingle(const OpenGL::Drawings& drawing) const noexcept override;

  private:
    unsigned int quadVAO;
    std::unique_ptr<Shader> shader; // Composition relashionship
    Texture2D* texture = nullptr;
    unsigned int VBO; // Vertex buffer object
    unsigned int VAO; // Vertex array object
    unsigned int EBO; // Element buffer object

    // helper function to use shaders
    virtual void useShader() const noexcept;
    /**
     * @brief Sets the texture for the element. Private so to keep encapsulation
     * @warning this function is called at every frame when we draw all the sprites. If
     * we create new textures at every funcyion call it will eat up all the memory and
     * all the cpu
     *
     * @param texture Texture to be set.
     */
    virtual void setTexture(const Texture2D&) noexcept;
};

/**
 * @brief Entry point for the Sprite class
 */
extern "C" SpriteRenderer* create_sprite();

extern "C" void delete_sprite(SpriteRenderer* sprite);