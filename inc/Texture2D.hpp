#pragma once

#ifdef unix
#include "Window.h" // contains all the needed includes
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stb/stb_image.h>
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/**
 * @brief Texture class, used for loading the textures properly
 *
 * @todo Proper encapsulation
 */
class Texture2D {
  public:
    Texture2D(const Texture2D&) = default;
    Texture2D& operator=(Texture2D&) = delete;
    Texture2D() = delete; // disable default constructor, must not be used
    // holds the ID of the texture object, used for all texture operations to reference
    // to this particlar texture
    unsigned int ID;
    // texture image dimensions
    int Width, Height; // width and height of loaded image in pixels
    // texture Format
    unsigned int Internal_Format; // format of texture object
    unsigned int Image_Format;    // format of loaded image
    // texture configuration
    unsigned int Wrap_S;     // wrapping mode on S axis
    unsigned int Wrap_T;     // wrapping mode on T axis
    unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
    // std::string Path;        // path of the texture
    /**
     * @brief constructor (sets default texture modes)
     *
     * @param texturePath
     * @param alpha
     */
    Texture2D(const char* texturePath, bool alpha)
        : Width(0)
        , Height(0)
        // , Path(texturePath)
        , Internal_Format(GL_RGB)
        , Image_Format(GL_RGB)
        , Wrap_S(GL_REPEAT)
        , Wrap_T(GL_REPEAT)
        , Filter_Min(GL_LINEAR)
        , Filter_Max(GL_LINEAR)
    {
        glGenTextures(1, &ID);
        if (alpha) {
            Internal_Format = GL_RGBA;
            Image_Format = GL_RGBA;
        }
        // load image
        int nrChannels;
        // stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(texturePath, &Width, &Height, &nrChannels, 0);

        // create Texture
        glBindTexture(GL_TEXTURE_2D, this->ID);
        glTexImage2D(GL_TEXTURE_2D, 0, Internal_Format, Width, Height, 0, Image_Format,
            GL_UNSIGNED_BYTE, data);
        // set Texture wrap and filter modes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);
        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }

    ~Texture2D(){};

    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};