#include "texture.hpp"

void Texture::doLoad(const std::string imagePath) {
    std::cout << "Reading image: " << imagePath << std::endl;


    // flags we want
    uint flags = SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS;

    //Load Image File Directly into an OpenGL Texture
    textureId = SOIL_load_OGL_texture
    (
        imagePath.c_str(),
        SOIL_LOAD_RGB,
        SOIL_CREATE_NEW_ID,
        flags
    );

    // error check
    if (textureId == 0) {
        std::cout << "SOIL loading error: " << SOIL_last_result() << std::endl;
    }
};

void Texture::bind(int texUnitPosition) {
    //? should this be done by renderer?
    // Activate requested texture unit
    glActiveTexture(GL_TEXTURE0 + texUnitPosition);

    // Bind this texture to activated texture unit
    glBindTexture(GL_TEXTURE_2D, textureId);

    // store position
    //? is it needed?
    texturePosition = texUnitPosition;
};