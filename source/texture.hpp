#ifndef TEXTURE_HPP
#define TEXTURE_HPP


#include "common.hpp"
#include "SOIL2.h" // to use soil2 and its flags


class Texture 
{
    public:
        // texture ID
        GLuint textureId;
        // textureUnit position
        int texturePosition = 0;

        /// @brief Default constructor creates a texture from a file
        /// @param imagePath Image path
        Texture(const std::string imagePath) {
            doLoad(imagePath);
        }; 
        
        /// @brief Loads the texture using SOIL2
        /// @param imagePath path for texture
        void doLoad(const std::string imagePath);

        /// @brief Binds texture to a texture unit
        /// @param texUnitPosition Texture unit to bind to
        void bind(int texUnitPosition);

};


#endif
