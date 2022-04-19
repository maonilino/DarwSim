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
 * @brief Type used for the map and species colour
 */
enum class Colour { brown, green, yellow };

/**
 * @brief Base graphics class used for renderers. This is the building block for the
 * renderer plugin(s).
 * @attention Fremwork user should implement custom drawings datatypes
 * @attention It is necesary to compile this (together with the plugin implementation)
 * as a shared lib, and link the required constructors with dlsym()
 * @todo Fix doxygen collaboration diagram error for shader smart pointer
 */
class Graphics {
  public:
    /**
     * @brief Drawings datatype. Could be modified by framework user. This structure is
     * only suitable for openGL renderer
     * @todo document the members and encapsulate so that framework user cannot use
     */
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
    };
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
    /**
     * @brief Default constuctor for Graphics object
     * used by the sprite renderer.
     *
     */
    Graphics() = default;

    /**
     * @brief Destroy the Graphics object
     *
     */
    virtual ~Graphics();

    /**
     * @brief Clear colour background
     * @note not necessarily needed as we use textured background
     */
    virtual void clearBackground(Colour colour) const noexcept;
    /**
     * @brief draws the elements in the buffers
     * @todo templarize function for the framework user to be able to use custom
     * drawings datatypes
     */
    virtual void drawAll(const std::vector<Drawings>& drawingList) noexcept = 0;
    /**
     * @brief draws a single object
     * @todo templarize function for the framework user to be able to use custom
     * drawings datatypes
     * @param drawing struct of the object to draw
     */
    virtual void drawSingle(const Drawings& drawing) const noexcept = 0;
    /**
     * @brief helper function to use shaders
     *
     */
    virtual void useShader() const noexcept;

  protected:
    /**
     * @brief Sets the texture for the element. Protected so to keep encapsulation
     *@warning this function is called at every frame when we draw all the sprites. If
     * we create new textures at every funcyion call it will eat up all the memory and
     * all the cpu
     *
     * @param texture Texture to be set.
     */
    virtual void setTexture(const Texture2D&) noexcept;

    std::unique_ptr<Shader> shader; // Composition relashionship
    Texture2D* texture = nullptr;
    unsigned int VBO; // Vertex buffer object
    unsigned int VAO; // Vertex array object
    unsigned int EBO; // Element buffer object
};