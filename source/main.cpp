#include <iostream>
#include <cstdlib>


#include <vector>
#include <string>
#include <cstring> //for memcmp
#include <map>
#include <memory>

// for file streams
#include <fstream>
#include <sstream>

//include glew before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

//include headers
#include <renderer.hpp>
#include <shader.hpp>
#include <mesh.hpp>

#define W_WIDTH 1024
#define W_HEIGHT 768
#define W_TITLE "MountainHiking"


// ! Be carefull with scope
using std::vector,glm::vec3;

// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/string_cast.hpp>

GLFWwindow* initialize_window(){

    // check if glfw Initialized
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW\n");
    }

    // Configure GLFW with hints
    // Set OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Set OpenGL Core profile to use all new functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // create window
    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, W_TITLE, NULL, NULL);

    // Check if window opened
    if (window == NULL) {
        glfwTerminate();
        throw std::runtime_error(std::string(std::string("Failed to open GLFW window.") +
                            " If you have an Intel GPU, they are not 3.3 compatible." +
                            "Try the 2.1 version.\n"));
    }

    // Introduce the indow to the current context
    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW so it configures OpenGl (Could use GLAD here!)
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLEW\n");
    }


    //Specify the viewport of openGL in the window
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    // Set background color (gray) [r, g, b, a]
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    return window;

}

class GameEngine{
    public:
        GameEngine(){};
};


/// @brief test cube object
class Cube:public Object {
    public:
        /// @brief constructor
        /// ! todo remember to add texture here later
        Cube(GameEngine& eng, std::string modelMesh);

        /// @brief an update method
        virtual void update(GameEngine& engine) override;
};




Cube::Cube(GameEngine& eng, std::string modelMesh){

    mesh = std::static_pointer_cast<Mesh>(std::make_shared<Mesh>("resources/models/cube.obj"));

}

void Cube::update(GameEngine& engine){

}

#include <glm/gtx/string_cast.hpp>


int main()
{
    std::cout << "Hello World!" << std::endl;
    try
    {
        //Enter Initializers
        
        // initialize window
        GLFWwindow* window = initialize_window();

        // initialize graphics pipeline / renderer
        Renderer renderer;

        // Test wireframe mode
        renderer.changeMode(RenderModes::wireFrame);

        // initialize and load meshes and models

        // initialize other systems



        vector<VertexData> triangle_vertices = {
            VertexData(vec3(-0.5f, -0.5f, 0.0f)),
            VertexData(vec3( 0.5f, -0.5f, 0.0f)),
            VertexData(vec3( 0.0f,  0.5f, 0.0f))
        };

        std::shared_ptr<Mesh> triangle = std::make_shared<Mesh>(triangle_vertices);

        GameEngine gameEngine;

        #define eng_getObjectTyped(obj_n, obj_t) std::static_pointer_cast<obj_t>(eng.getObject(obj_n))

        std::shared_ptr<Object> cube = std::static_pointer_cast<Object>(std::make_shared<Cube>(gameEngine,"resources/models/cube.obj"));


        //Enter main loop
        do
        {
            //calculate dt

            // Poll mouse and keyboard
            glfwPollEvents();

            // Clear Buffers / Screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Event handling / Update game state

            // Render into framebuffer

            renderer.SimpleRender(triangle);
            renderer.SimpleRender(cube);


            // Swap Buffers
            glfwSwapBuffers(window);
        } 
        while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                    glfwWindowShouldClose(window) == 0);
        
        
        //Enter Exit procedure

        // Terminate glfw before exiting
        glfwDestroyWindow(window); //? Terminate the window. Is it necessary if glfwTerminate() will delete all windows?
        glfwTerminate();

    } catch (std::exception& ex) {
        // If error occurred print error and terminate program
        std::cout << ex.what() << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}