// for file streams
#include <iostream>
#include <fstream>
#include <vector>

#include "shader.hpp"




/// @brief Will compile shader from file
/// @param shaderID shader id to compile
/// @param file file location
void compileShader(GLuint& shaderID, const std::string file) {
    // read shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(file, std::ios::in);
    if (shaderStream.is_open()) {
        std::string Line = "";
        while (getline(shaderStream, Line)) {
            shaderCode += "\n" + Line;
        }
        shaderStream.close();
    } else {
        throw std::runtime_error(std::string("Can't open shader file: ") + file);
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    // Compile Shader
    std::cout << "Compiling shader: " << file << std::endl;
    char const* sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);

    // Check Shader
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> shaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
        throw std::runtime_error(std::string(&shaderErrorMessage[0]));
    }
}

/// @brief Will create a full shader program with specified shaders
/// @param vertexFilePath vertex shader file location
/// @param fragmentFilePath fragment shader file location
/// @param geometryFilePath geometry shader file location
void Shader::createShaderProgram(
    const std::string vertexFilePath,
    const std::string fragmentFilePath,
    const std::string geometryFilePath
) {
    // Create the shaders
    GLuint vertId = glCreateShader(GL_VERTEX_SHADER);
    compileShader(vertId, vertexFilePath);

    GLuint fragId = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(fragId, fragmentFilePath);

    GLuint geomId = 0;
    if (geometryFilePath != "") {
        geomId = glCreateShader(GL_GEOMETRY_SHADER);
        compileShader(geomId, geometryFilePath);
    }

    // Link the program
    std::cout << "Linking shaders... " << std::endl;

    programId = glCreateProgram();

    glAttachShader(programId, vertId);
    if (geometryFilePath != "")
        glAttachShader(programId, geomId);
    glAttachShader(programId, fragId);
    glLinkProgram(programId);

    // Check the program
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMessage[0]);
        throw std::runtime_error(std::string(&programErrorMessage[0]));
    }

    // delete shader objects to free up memory (they are no longer needed)
    glDetachShader(programId, vertId);
    glDeleteShader(vertId);

    if (geometryFilePath != "") {
        glDetachShader(programId, geomId);
        glDeleteShader(geomId);
    }

    glDetachShader(programId, fragId);
    glDeleteShader(fragId);

    std::cout << "Shader program complete." << std::endl;
}

void Shader::getUniformLocations(){
    std::cout << "TODO:getting unform locations" << std::endl;
}

void Shader::bind() const {
    glUseProgram(programId);
}

void Shader::release() {
    glDeleteProgram(programId);
}

Shader::Shader(
    const std::string vertexFilePath,
    const std::string fragmentFilePath,
    const std::string geometryFilePath
) {
    createShaderProgram(vertexFilePath, fragmentFilePath, geometryFilePath);
    getUniformLocations();
}

