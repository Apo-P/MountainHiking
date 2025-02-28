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

        // empty constructor (so cubemap can inherit without issue)
        Texture(){};
        
        //virtual so cubemap can ovveride them

        /// @brief Loads the texture using SOIL2
        /// @param imagePath path for texture
        virtual void doLoad(const std::string imagePath);

        /// @brief Binds texture to a texture unit
        /// @param texUnitPosition Texture unit to bind to
        virtual void bind(int texUnitPosition);

        /// @brief Binds texture to a texture unit 0
        virtual void bind() ;

        // delete texture from gpu
        virtual void release() {
            // delete texture from gpu
            glDeleteTextures(1, &textureId);
        }
        


};


#endif
