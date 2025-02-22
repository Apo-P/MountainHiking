#include <iostream>

#include <renderer.hpp>
#include <glm/gtc/type_ptr.hpp>


void Renderer::makeUBO_VP(){

    // Allocate memory for view and projection matrices
    glGenBuffers(1, &VPmatricesUBO);

    // bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, VPmatricesUBO);
    // ? STATIC OR DYNAMIC DRAW for UBO?
    // allocate memory for UBO
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // bind the UBO to a binding point //? would glBindBufferRange be better?
    glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<int>(Shader::bindingPoints::VPmatrix), VPmatricesUBO);

}

void Renderer::sendViewMatrix(glm::mat4 viewMatrix) {

    // bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, getUBO_VP());
    // send data 
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix)); //value_ptr is similar to &V[0][0]
    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::sendProjectionMatrix(glm::mat4 projectionMatrix) {

    // bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, getUBO_VP());
    // send data //! Be carefull with offset
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix)); //value_ptr is similar to &V[0][0]
    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Renderer::Renderer(std::shared_ptr<GLFWwindow> new_window){

    window = new_window;

    std::cout << "Renderer class made" << std::endl;

    // compile shaders
    simpleShader = std::make_shared<Shader>("shaders/simpleVertex.vert", "shaders/simpleFragment.frag");

    normalDebugShader = std::make_shared<Shader>("shaders/normalDebug.vert", "shaders/normalDebug.frag", "shaders/normalDebug.geom");


    testShader = std::make_shared<Shader>("shaders/testVertex.vert", "shaders/testTexture.frag");

    // allocate Memory for UBO's
    makeUBO_VP();

    // set initial VP
    glm::mat4 V = glm::mat4(1);
    glm::mat4 P = glm::mat4(1);

    // to send data to UBO
    // Sending View //! This will be done every frame by camera
    sendViewMatrix(V);
    // sending projection //! this will be done whenever projection matrix changes by camera
    sendProjectionMatrix(P);

}

glm::vec2 Renderer::getWindowSize()
{
    int width, height;
    // get framebuffer size instead of window size
    glfwGetFramebufferSize(window.get(), &width, &height);
    // glfwGetWindowSize(window.get(), &width, &height);
    return glm::vec2(width, height);
}

Renderer::~Renderer(){
    std::cout << "Renderer class Deleted" << std::endl;
}
#include <glm/gtc/matrix_transform.hpp>
void Renderer::SimpleRender(std::shared_ptr<Mesh> meshObj){
    // std::cout << "Renderer Render called" << std::endl;

    //bind shader
    simpleShader->bind();


    // bind VAO
    meshObj.get()->bind();

    //send uniforms
    simpleShader->sendUniform(Shader::uniforms::ModelMatrix , glm::scale(glm::mat4(1), glm::vec3(1)));

    // draw mesh
    meshObj.get()->draw(*this);
}
void Renderer::SimpleRender(std::shared_ptr<Mesh> meshObj, glm::mat4 modelMatrix){
    // std::cout << "Renderer Render called" << std::endl;

    //bind shader
    simpleShader->bind();


    // bind VAO
    meshObj.get()->bind();

    //send uniforms
    simpleShader->sendUniform(Shader::uniforms::ModelMatrix , modelMatrix);

    // draw mesh
    meshObj.get()->draw(*this);

    //debug 
    if (DebugNormals) {
        normalDebugShader->bind();
        meshObj.get()->bind();
        simpleShader->sendUniform(Shader::uniforms::ModelMatrix , modelMatrix);
        meshObj.get()->draw(*this);
    }
}

void Renderer::SimpleRender(std::shared_ptr<Model> obj){

    // 1.Bind shader
    simpleShader->bind();

    // 2.Bind VAO
    obj->bind();
    // 3.Bind texture
    // 4.Send Uniforms
    simpleShader->sendUniform(Shader::uniforms::ModelMatrix , obj->getModelMatrix());
    // 5.Draw Triangles

    obj->draw(*this);


    //debug 
    if (DebugNormals){
        normalDebugShader->bind();
        obj->bind();
        simpleShader->sendUniform(Shader::uniforms::ModelMatrix , obj->getModelMatrix());
        obj->draw(*this);
    }

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