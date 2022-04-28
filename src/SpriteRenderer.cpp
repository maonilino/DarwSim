#define STB_IMAGE_IMPLEMENTATION // mindful of the include order! this must be defined
                                 // here before including stb_image!
#include "SpriteRenderer.h"

SpriteRenderer* create_sprite()
{
    return new SpriteRenderer();
}

void delete_sprite(SpriteRenderer* sprite)
{
    delete sprite;
}

SpriteRenderer::SpriteRenderer()
    : Graphics<OpenGL::Drawings>()
{
    // \warning alpha premultiplication needed!!! Otherwise nil alpha will be rendered
    // as white. Remove if not needed!
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader = std::make_unique<Shader>(
        "../annex/Shaders/vertexShader.vert", "../annex/Shaders/fragmentShader.frag");

    // texture = std::make_unique<Texture2D>(texturePath, true);

    // use orthographic projection
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIDTH),
        static_cast<float>(HEIGHT), 0.0f, -1.0f, 1.0f);
    shader->SetInteger("image", 0);
    shader->SetMatrix4("projection", projection, true); // bind the shader!!

    std::vector<float> vertices = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f};

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0],
        GL_STATIC_DRAW);

    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &quadVAO);
}

void SpriteRenderer::useShader() const noexcept
{
    shader->use();
}

void SpriteRenderer::setTexture(const Texture2D& text) noexcept
{
    texture = &const_cast<Texture2D&>(text); // safe only if we don't modify it
}

void SpriteRenderer::drawAll(const std::vector<OpenGL::Drawings>& drawingList) noexcept
{
    for (auto& i : drawingList) {
        setTexture(i.texture);
        drawSingle(i);
    }
}

void SpriteRenderer::drawSingle(const OpenGL::Drawings& drawing) const noexcept
{
    // prepare transformations
    shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(drawing.position, 0.0f));

    model = glm::translate(
        model, glm::vec3(0.5f * drawing.size.x, 0.5f * drawing.size.y, 0.0f));
    model =
        glm::rotate(model, glm::radians(drawing.rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(
        model, glm::vec3(-0.5f * drawing.size.x, -0.5f * drawing.size.y, 0.0f));

    model = glm::scale(model, glm::vec3(drawing.size, 1.0f));

    shader->SetMatrix4("model", model);
    shader->SetVector3f("spriteColor", drawing.color);

    glActiveTexture(GL_TEXTURE0);
    texture->Bind();

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}