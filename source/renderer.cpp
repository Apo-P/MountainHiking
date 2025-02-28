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

void Renderer::makeUBO_Lights(){

    // Allocate memory for lights
    glGenBuffers(1, &LightsUBO);

    // calculate UBO size
    size_t lightsUBOsize;

    lightsUBOsize = MAX_DIR_LIGHTS * DirectionalLight::sizeofData();
    lightsUBOsize += MAX_POINT_LIGHTS * PointLight::sizeofData();
    lightsUBOsize += 2 * sizeof(GLuint); // dirLightsCount, pointLightsCount

    // bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, LightsUBO);
    // ? STATIC OR DYNAMIC DRAW for UBO?
    // allocate memory for UBO
    glBufferData(GL_UNIFORM_BUFFER, lightsUBOsize, NULL, GL_DYNAMIC_DRAW);
    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // bind the UBO to a binding point //? would glBindBufferRange be better?
    glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<int>(Shader::bindingPoints::Lights), LightsUBO);


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

void Renderer::sendDirectionalLights(std::vector<std::shared_ptr<DirectionalLight>> directionalLights) {

    // bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, getUBO_Lights());

    // send light data
    size_t offset = 0; //! Be carefull with offset
    for (auto& dirLight : directionalLights) {
        std::vector<glm::vec4> lightData = dirLight->data(); 
        glBufferSubData(GL_UNIFORM_BUFFER, offset, lightData.size() * sizeof(lightData[0]), lightData.data());
        offset += lightData.size() * sizeof(lightData[0]);
    }

    // send light amount
    offset = MAX_DIR_LIGHTS * DirectionalLight::sizeofData() + MAX_POINT_LIGHTS * PointLight::sizeofData();
    GLuint dirLightsCount = directionalLights.size();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, dirLightsCount * sizeof(dirLightsCount), &dirLightsCount);

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

        
void Renderer::sendPointLights( std::vector<std::shared_ptr<PointLight>> pointLights){

    // bind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, getUBO_Lights());

    // send light data
    size_t offset = MAX_DIR_LIGHTS * DirectionalLight::sizeofData();//! Be carefull with offset
    for (auto& pointLight : pointLights) {
        std::vector<glm::vec4> lightData = pointLight->data();
        glBufferSubData(GL_UNIFORM_BUFFER, offset, lightData.size() * sizeof(lightData[0]), lightData.data());
        offset += lightData.size() * sizeof(lightData[0]);
    }

    // send light amount
    offset = (MAX_DIR_LIGHTS * DirectionalLight::sizeofData() + MAX_POINT_LIGHTS * PointLight::sizeofData()) + sizeof(GLuint); //! Be carefull with offset
    GLuint pointLightsCount = pointLights.size();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, pointLightsCount * sizeof(pointLightsCount), &pointLightsCount);

    // Unbind UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

}


void Renderer::sendLights(std::vector<std::shared_ptr<DirectionalLight>>  directionalLights, std::vector<std::shared_ptr<PointLight>> pointLights){

    sendDirectionalLights(directionalLights);
    sendPointLights(pointLights);

}

Renderer::Renderer(std::shared_ptr<GLFWwindow> new_window){

    window = new_window;

    std::cout << "Renderer class made" << std::endl;

    // compile shaders
    simpleShader = std::make_shared<Shader>("shaders/debugShaders/simpleVertex.vert", "shaders/debugShaders/simpleFragment.frag");

    normalDebugShader = std::make_shared<Shader>("shaders/debugShaders/normalDebug.vert", "shaders/debugShaders/normalDebug.frag", "shaders/debugShaders/normalDebug.geom");

    testShader = std::make_shared<Shader>("shaders/debugShaders/testVertex.vert", "shaders/debugShaders/testTexture.frag");

    skyboxShader = std::make_shared<Shader>("shaders/skybox.vert", "shaders/skybox.frag");

    pbrShader = std::make_shared<Shader>("shaders/pbr.vert", "shaders/pbr.frag");

    // allocate Memory for UBO's
    makeUBO_VP();
    makeUBO_Lights();

    // set initial VP
    glm::mat4 V = glm::mat4(1);
    glm::mat4 P = glm::mat4(1);

    // to send data to UBO
    // Sending View //! This will be done every frame by camera
    sendViewMatrix(V);
    // sending projection //! this will be done whenever projection matrix changes by camera
    sendProjectionMatrix(P);

    //! IMPORTANT INTEGRATE LATER

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    testShader->bind();// don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    GLuint testShaderId = testShader->getProgramId();
    glUniform1i(glGetUniformLocation(testShaderId, "texture0"), 0); // Configure shader sampler to use correct texture unit (TEXTURE_0 in this case)
    glUniform1i(glGetUniformLocation(testShaderId, "texture1"), 1); // Configure shader sampler to use correct texture unit (TEXTURE_1 in this case)
    glUniform1i(glGetUniformLocation(testShaderId, "texture2"), 2); // Configure shader sampler to use correct texture unit (TEXTURE_2 in this case)
    // or set it via the texture class
    // ourShader.setInt("texture2", 1);

    // ! need to integrate this to init!
    // TODO Also add a shader function and maybe get rid of the requirement for uniforms (maybe keep them as an option)
    skyboxShader->bind();
    GLuint skyboxShaderId = skyboxShader->getProgramId();
    glUniform1i(glGetUniformLocation(skyboxShaderId, "skybox"), 0);

    // pbr
    pbrShader->bind();

    GLuint pbrShaderId = pbrShader->getProgramId();
    glUniform1i(glGetUniformLocation(pbrShaderId, "albedoMap"), 0);// Configure shader sampler to use correct texture unit (TEXTURE_0 in this case)
    glUniform1i(glGetUniformLocation(pbrShaderId, "normalMap"), 1);// Configure shader sampler to use correct texture unit (TEXTURE_1 in this case)
    glUniform1i(glGetUniformLocation(pbrShaderId, "metallicMap"), 2);// Configure shader sampler to use correct texture unit (TEXTURE_2 in this case)
    glUniform1i(glGetUniformLocation(pbrShaderId, "roughnessMap"), 3);// Configure shader sampler to use correct texture unit (TEXTURE_3 in this case)
    glUniform1i(glGetUniformLocation(pbrShaderId, "aoMap"), 4);// Configure shader sampler to use correct texture unit (TEXTURE_4 in this case)
    
    
    
    
    

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

void Renderer::SimpleRender(std::shared_ptr<Object> obj) {
    // 1.Bind shader
    simpleShader->bind();


    // 2.Bind VAO
    obj->model->bind();
    // 3.Bind texture
    // 4.Send Uniforms
    simpleShader->sendUniform(Shader::uniforms::ModelMatrix , obj->model->getModelMatrix());
    // 5.Draw Triangles

    obj->model->draw(*this);


    //debug 
    if (DebugNormals){
        normalDebugShader->bind();
        obj->model->bind();
        simpleShader->sendUniform(Shader::uniforms::ModelMatrix , obj->model->getModelMatrix());
        obj->model->draw(*this);
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