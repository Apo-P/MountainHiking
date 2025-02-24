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

int Shader::getBindingPoint(Shader::uniforms uniformUBO) {

    switch (uniformUBO)
    {
    case uniforms::VPmatrix :
        return static_cast<int>(bindingPoints::VPmatrix);
    
    default:
        throw std::runtime_error("Tried to get a UBO that doesn't have a binding point!\n");
        return -1;
    }
}

int Shader::getBindingPoint(std::string UBOName) {

    if (UBOName == std::string("VPmatrices")) {
        return static_cast<int>(bindingPoints::VPmatrix);
    }
    else {
        throw std::runtime_error("Tried to get a UBO that doesn't have a binding point!\n");
        return -1;
    }
}

std::string Shader::getUniformName(Shader::uniforms uniformTarget) {
    int targetNumber = static_cast<int>(uniformTarget);

    if (targetNumber<100) {
        return uniformNames[targetNumber];
    }
    return UBONames[targetNumber-100];
    
}

void Shader::getUniformLocations(){
    std::cout << "TODO:getting uniform locations" << std::endl;

    //? check if needed
    //  use program temporarily to be able to find locations
    // bind();

    
    // fetching and storing uniform locations
    GLuint uniform_location;
    for (auto name : uniformNames) {
        uniform_location = glGetUniformLocation(programId, name.c_str()); //c_str() Converts string to Char*
    
        if (uniform_location != -1 ){
            std::cout << "found:" << name << std::endl;
            // add to map
        }
        else {
            std::cout << "not found:" << name.c_str() << std::endl;
        }
    }

    // fetching and binding Uniforms block index to Binding point
    for (auto name : UBONames) {
        uniform_location = glGetUniformBlockIndex(programId, name.c_str()); //c_str() Converts string to Char*
    
        if (uniform_location != -1 ){
            std::cout << "found:" << name  << " binding point:" << getBindingPoint(name) << std::endl;
            // bind to binding point
            glUniformBlockBinding(programId, uniform_location, getBindingPoint(name));
        }
        else {
            std::cout << "not found:" << name.c_str() << std::endl;
        }
    }
    

    // unbind
    // unbind();

}

void Shader::bind() const {
    glUseProgram(programId);
}

void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::release() {
    glDeleteProgram(programId);
}

Shader::Shader(
    const std::string vertexFilePath,
    const std::string fragmentFilePath,
    const std::string geometryFilePath
) {
    // create shader program
    createShaderProgram(vertexFilePath, fragmentFilePath, geometryFilePath);
    
    // // get VP UBO location
    // VPmatricesLocation = glGetUniformBlockIndex(programId, "VPmatrices");

    // glUniformBlockBinding(programId, VPmatricesLocation, 0);



    getUniformLocations();
}

void Shader::sendUniform(Shader::uniforms uniformTarget, const glm::mat4 &matrix) {

    // std::cout << "TODO:sending uniform locations" << std::endl;

    std::string requestedUniform = getUniformName(uniformTarget);

    // std::cout << "requested to send:" << requestedUniform << std::endl;

    //? Maybe change to be stored in hash map instead?
    GLuint uniformLocation = glGetUniformLocation(programId, requestedUniform.c_str());

    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &matrix[0][0]);

}

void Shader::sendUniform(Shader::uniforms uniformTarget, const bool &value) {

    std::string requestedUniform = getUniformName(uniformTarget);

    // glUniformMatrix4fv(glGetUniformLocation(programId, requestedUniform.c_str()), 1, GL_FALSE, &matrix[0][0]);
    glUniform1f(glGetUniformLocation(programId, requestedUniform.c_str()), value);

}

