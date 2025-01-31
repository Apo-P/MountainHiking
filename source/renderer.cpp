#include <iostream>

#include <renderer.hpp>


Renderer::Renderer(){
    std::cout << "Renderer class made" << std::endl;

    // compile shaders
    simpleShader = std::make_shared<Shader>("shaders/simpleVertex.vert", "shaders/simpleFragment.frag");

    // allocate Memory for UBO's

    // Allocate memory for view and projection matrices
    glGenBuffers(1, &VPmatricesUBO);

    // bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, VPmatricesUBO);
    // ? STATIC OR DYNAMIC DRAW for UBO?
    // allocate memory
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // bind the UBO to a binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, VP_UBO_BINDING_POINT, VPmatricesUBO);

}

Renderer::~Renderer(){
    std::cout << "Renderer class Deleted" << std::endl;
}

void Renderer::SimpleRender(std::shared_ptr<Mesh> meshObj){
    // std::cout << "Renderer Render called" << std::endl;

    //bind shader
    simpleShader->bind();

    // bind VAO
    meshObj.get()->bind();

    // draw mesh
    meshObj.get()->draw(*this);
}

void Renderer::SimpleRender(std::shared_ptr<Object> obj){

    // 1.Bind shader
    simpleShader->bind();

    // 2.Bind VAO
    // 3.Bind texture
    // 4.Send Uniforms
    // 5.Draw Triangles

    obj->draw(*this);

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