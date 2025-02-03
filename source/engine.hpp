#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <common.hpp>

#include <renderer.hpp>
#include <controls.hpp>


class PhysicsEngine;


class GameEngine{
    
    private:
        std::shared_ptr<GLFWwindow> window;

        std::unique_ptr<Renderer> renderer;

        std::unique_ptr<Controls> controller;

        std::shared_ptr<PhysicsEngine> physicsEngine;


        struct DestroyGLFWwindow{
            void operator()(GLFWwindow* ptr){
                glfwDestroyWindow(ptr);
            }
        };

        // static int width, height;
    
    public:
        // this should be game specific
        virtual void createContext(){};
        virtual void update(){};


        // initialize systems
        void initialize();

        // start game loop
        int startGame();

    private:
        // Initialize window
        void initialize_window(int windowWidth, int windowHeight, std::string& windowTitle);

    public:

        /// @brief last time update was done
        float previousTime;
        /// @brief delta time between previous time and current time
        float deltaTime;
        

        // Renderer should be able to call window
        friend Renderer;
        // Controls should be able to call objects
        friend Controls;

};

#endif