#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "shader.hpp"
#include "mesh.hpp"

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
    public:

        Renderer();

        void SimpleRender(const Mesh& meshObj);

        void changeMode(RenderModes new_mode);

        ~Renderer();
};

#endif