#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include "common.hpp"
#include "texture.hpp"

class Cubemap : public Texture
{
    public:

        // OpenGl cubemap textures (start as posX)
        //              Enum                Orientation
        // GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right
        // GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left
        // GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top
        // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom
        // GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back
        // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front


        /// @brief creates a cube map from a string of textures
        /// @param cubemapFaces the location of each face
        Cubemap(const std::vector<std::string>& cubemapFaces){
            doLoad(cubemapFaces);
        }; 

        /// @brief Loads each face of the cubemap
        /// @param cubemapFaces the location of each face
        void doLoad(const std::vector<std::string>& cubemapFaces);

        /// @brief Binds cubemap
        void bind() override;
};

#endif