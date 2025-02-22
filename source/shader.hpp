#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>

/// @brief  shader class stores shader program id and uniform locations. It can be bound 
class Shader{

    public:
        // allowed uniforms
        // enum starts from 0
        enum class uniforms{
            ModelMatrix=0, //! be carefull order of uniforms needs to be the same as the order in Uniform names
            Terrain=1,

            VPmatrix=100   //! UBO OBJECTS Have 100 in front to distinguish them, last digit need to be the same as binding point
        };

        //? should UBOs be different from uniforms?
        //? if so we could overload functions

        enum class layoutPosition{
            POSITION = 0,
            NORMAL = 1,
            UV = 2
        };


        // where uniform block index will be bounded to
        enum class bindingPoints{
            VPmatrix = 0
        };

        //! Macro to cast enum to int Carefull with scope
        //? can this be integrated in enum class with a wrapper?
        // #define castEnumInt(enum) static_cast<int>(enum)
        
    private: 

        /// @brief searches for UBO binding point using uniform Enum
        /// @param uniformUBO uniform Enum
        /// @return Returns binding point int
        int getBindingPoint(Shader::uniforms uniformUBO);

        /// @brief searches for UBO binding point using Name
        /// @param UBOName UBO Name
        /// @return Returns binding point int
        int getBindingPoint(std::string UBOName);


    private:
        /// Shader program id
        GLuint programId;

        /// TODO: Uniform locations 
        // ? Should these be in a map or get method that takes name of variable as input
        // ? so we dont have to have empty variables for shaders that dont use them?

        // Locations(indexes)
        std::unordered_map<std::string, GLuint> uniform_locations;

        /// @brief list of possible uniform names
        std::string uniformNames[2] = {
            "M",
            "Terrain"
        };
        /// @brief list of possible UBO names
        std::string UBONames[1] = {
            "VPmatrices"
        };

        ///TODO: Remove these variables as they are stores in the map 

        /// ModelLocation
        GLuint Mlocation;
        // View and projection matrices UBO location
        GLuint VPmatricesLocation;

    
        /// @brief Takes code location to create shader program
        /// @param vertexFile vertex shader location
        /// @param fragmentFile fragment shader location
        /// @param geometryFile geometry shader location, if Specified
        void createShaderProgram(const std::string vertexFile, const std::string fragmentFile, const std::string geometryFile="");
        

        std::string getUniformName(Shader::uniforms uniformTarget);

        /// @brief Gets locations of uniform variables and stores them
        /// Note any location not found will be -1!
        void getUniformLocations();


    public:

        /// @brief Create shader program from shader files
        /// @param vertexFile vertex shader location
        /// @param fragmentFile fragment shader location
        /// @param geometryFile geometry shader location, if Specified
        Shader(const std::string vertexFile, const std::string fragmentFile, const std::string geometryFile="");
        
        /// @brief Binds this shaderProgram to gpu (locks current state)
        void bind() const;
        /// @brief Unbinds this shaderProgram to gpu
        void unbind() const;
        /// @brief Releases this shaderProgram from gpu (deletes)
        void release();

        /// @brief Send a uniform matrix
        /// @param uniformTarget The target uniform
        /// @param matrix Matrix data
        void sendUniform(Shader::uniforms uniformTarget, const glm::mat4 &matrix);

        void sendUniform(Shader::uniforms uniformTarget, const bool &matrix);

        GLuint getProgramId(){ return programId; };
};

#endif
