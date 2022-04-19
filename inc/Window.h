#pragma once

#ifdef unix
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <exception>
#include <memory>
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

#define WIDTH  819
#define HEIGHT 644

/**
 * @brief plugin for the windowing system. Implemented using glfw
 *
 */
class Window {
  public:
    Window(Window&) = delete;
    Window& operator=(Window&) = delete;

    Window() = delete; // deleted as we do never use it
    /**
     * @brief Constructor initializes the window properly
     *
     * @todo Add error handling
     * @param TITLE Title of the window
     * @param WIDTH Width of the window
     * @param HEIGHT Height of the window
     */
    Window(const char* TITLE, const GLint Width, const GLint Height);

    /**
     * @brief Destroys the Window object and calls glfwTerminate()
     *
     */
    virtual ~Window();

    /**
     * @brief Calls glfwSwapBuggers and glfwPollEvents
     *
     */
    virtual void swapBuffers() noexcept;
    /**
     * @brief Check if the window should close
     *
     * @return int: 0 if it should be closed
     */
    virtual int windowShouldClose() noexcept;

  private:
    GLFWwindow* glfwWin;
};

/**
 * @brief Entry point of the Window object
 *
 * @param TITLE Title of the window
 * @param WIDTH Width of the window
 * @param HEIGHT Height of the window
 */
extern "C" Window* create_window(const char* TITLE, const GLint Width, GLint Height);

extern "C" void destroy_window(Window*);