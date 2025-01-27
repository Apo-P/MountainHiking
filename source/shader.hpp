#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>

// ! This will be inside shader file, as shader controls them
// ! this defines layout position for shaders
#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define UV_LOCATION 2

/// @brief  shader class stores shader program id and uniform locations. It can be bound 
class Shader{

    private:
        /// Shader program id
        GLuint programId;

        /// TODO: Uniform locations 
        // ? Should these be in a map or get method that takes name of variable as input
        // ? so we dont have to have empty variables for shaders that dont use them?

        /// ModelLocation
        GLuint Mlocation;

        GLuint cameraPositionLocation;

    
        /// @brief Takes code location to create shader program
        /// @param vertexFile vertex shader location
        /// @param fragmentFile fragment shader location
        /// @param geometryFile geometry shader location, if Specified
        void createShaderProgram(const std::string vertexFile, const std::string fragmentFile, const std::string geometryFile="");
        /// @brief Gets locations of uniform variables
        void getUniformLocations();

    public:

        /// @brief Create shader program from shader files
        /// @param vertexFile vertex shader location
        /// @param fragmentFile fragment shader location
        /// @param geometryFile geometry shader location, if Specified
        Shader(const std::string vertexFile, const std::string fragmentFile, const std::string geometryFile="");
        
        /// @brief Binds this shaderProgram to gpu (locks current state)
        void bind() const;
        /// @brief Releases this shaderProgram from gpu (deletes)
        void release();
};

#endif
