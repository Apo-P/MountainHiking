#include <iostream>
#include <cstdlib>

//include glew before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define W_WIDTH 1024
#define W_HEIGHT 768
#define W_TITLE "MountainHiking"

int main()
{
    std::cout << "Hello World!" << std::endl;
    try
    {
        
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


        //Enter main loop
        do
        {
            // Poll mouse and keyboard
            glfwPollEvents();

            // Clear Buffers
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap Buffers
            glfwSwapBuffers(window);
        } 
        while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                    glfwWindowShouldClose(window) == 0);
        
        
        // Terminate glfw before exiting
        //// glfwDestroyWindow(window); //Terminate the window. It is not necessary because glfwTerminate() will delete all windows
        glfwTerminate();

    } catch (std::exception& ex) {
        // If error occurred print error and terminate program
        std::cout << ex.what() << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}