#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "common.hpp"

#include "shader.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "scene.hpp"

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
        std::shared_ptr<Shader> normalDebugShader;

        std::shared_ptr<Shader> testShader;

        RenderModes mode = RenderModes::Normal; //Default is normal
        GLuint VPmatricesUBO;

        std::shared_ptr<GLFWwindow> window;

        // TODO: generalize this for other UBO's
        /// @brief make UBO for view and projection matrix
        void makeUBO_VP();
    public:
        //! scene should have access to these
        /// @brief send view matrix to shaders
        /// @param viewMatrix view matrix to send
        void sendViewMatrix(glm::mat4 viewMatrix);
        /// @brief send projection matrix to shaders
        /// @param projectionMatrix projection matrix to send
        void sendProjectionMatrix(glm::mat4 projectionMatrix);
        //for testing
        void bindShader(){simpleShader.get()->bind(); };
        void unbindShader(){simpleShader.get()->unbind(); };

    public:

        Renderer(std::shared_ptr<GLFWwindow> new_window);

        /// @brief simple render a mesh obj
        /// @param meshObj mesh to render
        void SimpleRender(std::shared_ptr<Mesh> meshObj);
        void SimpleRender(std::shared_ptr<Mesh> meshObj, glm::mat4 modelMatrix);

        
        // test render
        void testRender(std::shared_ptr<Cube> obj);


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

        glm::vec2 getWindowSize();

        ~Renderer();
};

#endif