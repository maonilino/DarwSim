#include "Graphics.h"

Graphics::Graphics(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{

    shader = std::make_unique<Shader>(
        "../annex/Shaders/vertexShader.vert", "../annex/Shaders/fragmentShader.frag");

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0],
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0],
        GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Graphics::clearBackground(Colour colour) const noexcept
{
    switch (colour) {
    case Colour::green:
        glClearColor(0.384f, 0.447f, 0.113f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        break;

    case Colour::brown:
        glClearColor(0.556f, 0.388f, 0.243f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        break;

    case Colour::yellow:
        glClearColor(0.960f, 0.803f, 0.019f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        break;

    default:
        break;
    }
}

void Graphics::drawSingle(const Drawings& drawing) const noexcept
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Graphics::useShader() const noexcept
{
    shader->use();
}

void Graphics::setTexture(const Texture2D& text) noexcept
{
    texture = &const_cast<Texture2D&>(text);
}

Graphics::~Graphics()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(2, &EBO);
}