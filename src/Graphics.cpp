#include "Graphics.h"

template <typename drawing_type>
Graphics<drawing_type>::Graphics(
    std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
}

// template <typename drawing_type>
// void Graphics<drawing_type>::clearBackground(Colour colour) const noexcept
// {
//     return 0;
// }

template <typename drawing_type>
void Graphics<drawing_type>::drawSingle(const drawing_type& drawing) const noexcept
{
}

// template <typename drawing_type> Graphics<drawing_type>::~Graphics()
// {
//     // glDeleteVertexArrays(1, &VAO);
//     // glDeleteBuffers(1, &VBO);
//     // glDeleteBuffers(2, &EBO);
// }