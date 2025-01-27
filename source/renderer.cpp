#include <iostream>

#include <renderer.hpp>


Renderer::Renderer(){
    std::cout << "Renderer class made" << std::endl;

    simpleShader = std::make_shared<Shader>("shaders/simpleVertex.vert", "shaders/simpleFragment.frag");

}

Renderer::~Renderer(){
    std::cout << "Renderer class Deleted" << std::endl;
}

void Renderer::SimpleRender(const Mesh& meshObj){
    // std::cout << "Renderer Render called" << std::endl;

    //bind shader
    simpleShader->bind();

    // draw model
    meshObj.draw(glm::mat4(0), simpleShader);
}

void Renderer::changeMode(RenderModes new_mode){
    mode = new_mode;

    switch (mode)
    {
        case RenderModes::wireFrame:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //GL_LINE is wireframe mode
            break;

        case RenderModes::Normal:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_FILL is normal mode
            break;

    }
    
}