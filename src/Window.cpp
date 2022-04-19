#include "Window.h"

Window* create_window(const char* TITLE, const GLint Width, GLint Height)
{
    return new Window(TITLE, Width, Height);
}

void destroy_window(Window* win)
{
    delete win;
}

/**
 * @brief Make sure the viewport matches the new window dimensions;
 * note that Width and height will be significantly larger than
 * specified on retina displays
 *
 * @param window The window that this callback belongs to
 * @param Width The new Width of the window?
 * @param height The new height of the window?
 *
 */
void framebuffer_size_callback(GLFWwindow* window, int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

Window::Window(const char* TITLE, const GLint Width, const GLint Height)
{
    // glfw: init and config
    // ---------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWin = glfwCreateWindow(Width, Height, TITLE, NULL, NULL);
    if (glfwWin == NULL) {
        // TODO: error reporting
        glfwTerminate();
    }

    glfwMakeContextCurrent(glfwWin);
    glfwSetFramebufferSizeCallback(glfwWin, framebuffer_size_callback);

    // glewExperimental = GL_TRUE;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        // TODO: error reporting
    }


}

int Window::windowShouldClose() noexcept
{
    return glfwWindowShouldClose(glfwWin);
}

void Window::swapBuffers() noexcept
{
    // glfw: swap buffers
    // ------------------
    glfwSwapBuffers(glfwWin);
    glfwPollEvents();
}

Window::~Window()
{
    glfwTerminate();
}
