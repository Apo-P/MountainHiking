#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "common.hpp"

#include "shader.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "scene.hpp"
#include "light.hpp"
#include "assetManager.hpp"

/// @brief Possible RenderModes for Renderer
//? could later add map to cast RenderMode to GLenum
enum class RenderModes{
                wireFrame,
                Normal
            };

// class GameEngine; // this in order to be able to store window without messing up file references

// test render cube //!remove later
#include"testCube.hpp"

class Renderer{
        
    private:
        std::shared_ptr<Shader> simpleShader;
        

        const int MAX_DIR_LIGHTS = 1; // 1 directional light
        const int MAX_POINT_LIGHTS = 1; // 1 point light
        

        RenderModes mode = RenderModes::Normal; //Default is normal
        
        GLuint VPmatricesUBO, LightsUBO;

        std::shared_ptr<GLFWwindow> window;

        // TODO: generalize this for other UBO's
        /// @brief make UBO for view and projection matrix
        void makeUBO_VP();

        void makeUBO_Lights();
    public:

        bool DebugNormals = false; // change to private later
        std::shared_ptr<Shader> testShader;
        std::shared_ptr<Shader> skyboxShader;
        std::shared_ptr<Shader> pbrShader;
        std::shared_ptr<Shader> normalDebugShader;
        std::shared_ptr<Shader> terrainShader;

        //! scene should have access to these
        /// @brief send view matrix to shaders
        /// @param viewMatrix view matrix to send
        void sendViewMatrix(glm::mat4 viewMatrix);
        /// @brief send projection matrix to shaders
        /// @param projectionMatrix projection matrix to send
        void sendProjectionMatrix(glm::mat4 projectionMatrix);

        /// @brief send list of lights
        /// @param directionalLights list of directional lights
        void sendDirectionalLights(std::vector<std::shared_ptr<DirectionalLight>>  directionalLights);

        /// @brief send list of lights
        /// @param pointLights list of point lights
        void sendPointLights( std::vector<std::shared_ptr<PointLight>> pointLights);

        /// @brief send list of lights
        /// @param directionalLights list of directional lights
        /// @param pointLights list of point lights
        void sendLights(std::vector<std::shared_ptr<DirectionalLight>>  directionalLights, std::vector<std::shared_ptr<PointLight>> pointLights);

        //for testing
        void bindShader(){simpleShader.get()->bind(); };
        void unbindShader(){simpleShader.get()->unbind(); };

    public:

        Renderer(std::shared_ptr<GLFWwindow> new_window);

        /// @brief simple render a mesh obj
        /// @param meshObj mesh to render
        void SimpleRender(std::shared_ptr<Mesh> meshObj);
        void SimpleRender(std::shared_ptr<Mesh> meshObj, glm::mat4 modelMatrix);
        void SimpleRender(std::shared_ptr<Object> obj);

        
        //test texture for those that dont have one yet
        // std::shared_ptr<Texture> testTexture = std::make_shared<Texture>("resources/textures/grass1.png");
        // test render
        void testRender(std::shared_ptr<Model> obj);

        //test 
        // BE CAREFULL WE DONT CHECK ITS A SKYBOX FOR NOW! (so we dont know if it has a cubemap)
        void renderSkybox(std::shared_ptr<Model> skybox);

        //test
        void renderPBR(std::shared_ptr<Model> obj, const glm::vec3 &cameraPos);

        void renderTerrain(std::shared_ptr<TerrainChunk> terrainChunk, const glm::vec3 &cameraPos);


        /// @brief simple render a object
        /// @param obj object to render
        void SimpleRender(std::shared_ptr<Model> obj);

        /// @brief simple render a scene
        /// @param scene scene to render
        void SimpleRender(std::shared_ptr<Scene> scene);

        /// @brief change rendering mode
        /// @param new_mode new mode to use
        void changeMode(RenderModes new_mode);

        /// @brief get UBO of VP
        /// @return returns UBO
        GLuint getUBO_VP() { return VPmatricesUBO; };

        /// @brief get UBO of Lights
        /// @return returns UBO
        GLuint getUBO_Lights() { return LightsUBO; };

        glm::vec2 getWindowSize();

        ~Renderer();
};

#endif