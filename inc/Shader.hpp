#pragma once

#ifdef unix
#include "Window.h" // contains all the needed includes
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * This is the shader class
 *
 * @warning bind shader before setting uniforms!
 *
 */
class Shader {
  public:
    Shader(Shader&) = delete;            // Disable copy constructor
    Shader& operator=(Shader&) = delete; // Disable copy assignemnt operator

    unsigned int ID; // Used for the shader program
    /**
     * @brief Constructor generates the shader
     *
     * @param vertexPath Path of the vertex shader file
     * @param fragmentPath Path of the fragment shader file
     **/
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what()
                      << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        GLint returnedParam;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &returnedParam);
        if (!returnedParam) {
            char infoLog[512];
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::string log(infoLog);
            std::cerr << "ERROR::Vertex shader compilation failed! " << log << "\n";
        }
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &returnedParam);
        if (!returnedParam) {
            char infoLog[512];
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::string log(infoLog);
            std::cerr << "ERROR::Fragment shader compilation failed! " << log << "\n";
        }
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &returnedParam);
        if (!returnedParam) {
            char infoLog[512];
            glGetProgramInfoLog(ID, 512, nullptr, infoLog);
            std::string log(infoLog);
            std::cerr << "ERROR::Shader program linking failed! " << log << "\n";
        }
        // delete the shaders as they're linked into our program now and no longer
        // necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use()
    {
        glUseProgram(ID);
    }

    /**
     * @brief Set the Matrix 4 uniform in the shader
     *
     * @param name name of the uniform
     * @param matrix the data itself
     * @param useShader flag whether we should bind the shader or not
     *
     * @warning Could cause potential bug from dangling matrix
     */
    void SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false)
    {
        if (useShader)
            use();
        glUniformMatrix4fv(
            glGetUniformLocation(ID, name), 1, false, glm::value_ptr(matrix));
    }

    /**
     * @brief Set the Vector 3f uniform in the shader
     *
     * @param name name of the uniform
     * @param value value to be set
     * @param useShader flag whether we should bind the shader or not
     */
    void SetVector3f(const char* name, const glm::vec3& value, bool useShader = false)
    {
        if (useShader)
            use();
        glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
    }

    /**
     * @brief Set the Integer uniform in the shader program.
     *
     * @param name name of the uniform
     * @param value value to set
     * @param useShader flag whether we should bind the shader or not
     */
    void SetInteger(const char* name, int value, bool useShader = false)
    {
        if (useShader)
            use();
        glUniform1i(glGetUniformLocation(ID, name), value);
    }

    /**
     * @brief Destroy the Shader object
     *
     * @todo Cleaning up the shader programs?
     */
    ~Shader()
    {
    }
};