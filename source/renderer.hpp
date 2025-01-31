#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "shader.hpp"
#include "mesh.hpp"
#include "object.hpp"

/// @brief Possible RenderModes for Renderer
//? could later add map to cast RenderMode to GLenum
enum class RenderModes{
                wireFrame,
                Normal
            };

class Renderer{
        
    private:
        std::shared_ptr<Shader> simpleShader;
        RenderModes mode = RenderModes::Normal; //Default is normal
        GLuint VPmatricesUBO;
    public:

        Renderer();

        void SimpleRender(std::shared_ptr<Mesh> meshObj);
        void SimpleRender(std::shared_ptr<Object> obj);

        void changeMode(RenderModes new_mode);

        ~Renderer();
};

#endif