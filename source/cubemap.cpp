#include "cubemap.hpp"


void Cubemap::doLoad(const std::vector<std::string>& cubemapFaces) {

    // std::cout << "Reading image: " << imagePath << std::endl;


    // flags we want
    uint flags = SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS;

    //Load Image File Directly into an OpenGL Cubemap
    textureId = SOIL_load_OGL_cubemap(
        cubemapFaces[0].c_str(),    //the location of the file to upload as the +x cube face
        cubemapFaces[1].c_str(),    //the location of the file to upload as the -x cube face
        cubemapFaces[2].c_str(),    //the location of the file to upload as the +y cube face
        cubemapFaces[3].c_str(),    //the location of the file to upload as the -y cube face
        cubemapFaces[4].c_str(),    //the location of the file to upload as the +z cube face
        cubemapFaces[5].c_str(),    //the location of the file to upload as the -z cube face
        SOIL_LOAD_RGB,              //Loads image as rgb
        SOIL_CREATE_NEW_ID,         //makes new texture Id
        flags                       //the flags we want
    );

    // error check
    if (textureId == 0) {
        std::cout << "SOIL loading error: " << SOIL_last_result() << std::endl;
    }
};

void Cubemap::bind() {
    //? should this be done by renderer?

    // Bind this cubemap
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

};