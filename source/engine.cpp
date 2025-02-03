#include <engine.hpp>



void GameEngine::initialize_window(int windowWidth, int windowHeight, std::string& windowTitle){

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
    GLFWwindow* new_window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);

    // Check if window opened
    if (new_window == NULL) {
        glfwTerminate();
        throw std::runtime_error(std::string(std::string("Failed to open GLFW window.") +
                            " If you have an Intel GPU, they are not 3.3 compatible." +
                            "Try the 2.1 version.\n"));
    }

    // Introduce the indow to the current context
    glfwMakeContextCurrent(new_window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW so it configures OpenGl (Could use GLAD here!)
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLEW\n");
    }

    // window resize call back
    glfwSetWindowSizeCallback(new_window, [](GLFWwindow* window, int w, int h) 
    {
        //update viewport
        glViewport(0, 0, w, h);

    });



    // Some GLFW options //! these should be initialized in controls

    // window keys to make sure states stay the same
    // When sticky keys mode is enabled, the pollable state of a key will remain the same until polled again
    glfwSetInputMode(new_window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited movement
    //! this wont work on WSL (because we dont have cursor access?)
    glfwSetInputMode(new_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // window mouse check if enables raw input
    if (glfwRawMouseMotionSupported()) {
        std::cout << "rawmouse supported!" <<std::endl;
        glfwSetInputMode(new_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    //! if we are on wsl activate WSL bool


    //Specify the viewport of openGL in the window
    glViewport(0, 0, windowWidth, windowHeight);

    // Set background color (gray) [r, g, b, a]
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    
    // // Set window initial width and height
    // GameEngine::width = windowWidth; 
    // GameEngine::height = windowHeight;

    // set window var
    window = std::shared_ptr<GLFWwindow>(new_window, DestroyGLFWwindow());

}

void GameEngine::initialize(){

    const int W_WIDTH = 1024;
    const int W_HEIGHT = 768;
    std::string W_TITLE {"MountainHiking"};

    //Enter Initializers
        
    // initialize window
    initialize_window(W_WIDTH, W_HEIGHT, W_TITLE);

    // initialize graphics pipeline / renderer
    renderer = std::make_unique<Renderer>(window);

    // initialize controller
    controller = std::make_unique<Controls>(window);


    // initialize and load meshes and models

    // initialize other systems

}


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



int GameEngine::startGame() {
    
    
    // ! Be carefull with scope
    using std::vector,glm::vec3;

    
    // Do initializers
    initialize();

    try
    {
        // testing ----

        // Test wireframe mode
        renderer.get()->changeMode(RenderModes::wireFrame);

        vector<VertexData> triangle_vertices = {
            VertexData(vec3(-0.5f, -0.5f, 0.0f)),
            VertexData(vec3( 0.5f, -0.5f, 0.0f)),
            VertexData(vec3( 0.0f,  0.5f, 0.0f))
        };

        std::shared_ptr<Mesh> triangle = std::make_shared<Mesh>(triangle_vertices);

        std::shared_ptr<Object> cube = std::static_pointer_cast<Object>(std::make_shared<Cube>(*this,"resources/models/cube.obj"));


        //make a camera
        std::shared_ptr<Camera> mainCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), DEFAULT_ORIENTATION);

        //make a scene
        Scene mainScene(mainCamera);

        // upload initial projection matrix 
        //! no need to change often unless if new projection matrix
        renderer.get()->sendProjectionMatrix(mainScene.camera.get()->getProjection());
        


        cube.get()->setModelMatrix(glm::translate(glm::mat4(1), vec3(0,0,-2)));

        

        // end of testing ----

        //Enter main loop
        do
        {
            // std::cout << "running " << std::endl;


            //calculate dt
            float current_time = glfwGetTime(); // Calculate elapsed time.
            deltaTime =  current_time - previousTime;
            previousTime = current_time;


            // Poll mouse and keyboard //! this should be in controller
            glfwPollEvents();

            controller.get()->handleInputs(mainScene, deltaTime);

            // Clear Buffers / Screen //! this should be in renderer
            glClear(GL_COLOR_BUFFER_BIT);

            // Event handling / Update game state //! this should be in scene

            //! this needs to be done every frame
            renderer.get()->sendViewMatrix(mainScene.camera.get()->getView());


            // update camera

            // Render into framebuffer //! this should be in renderer
            

            renderer.get()->SimpleRender(triangle);
            renderer.get()->SimpleRender(cube);


            // Swap Buffers //! this should be in renderer
            glfwSwapBuffers(window.get());
        } 
        while (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                    glfwWindowShouldClose(window.get()) == 0);
        
        
        //Enter Exit procedure

        // Terminate glfw before exiting
        glfwDestroyWindow(window.get()); //? Terminate the window. Is it necessary if glfwTerminate() will delete all windows?
        glfwTerminate();

    } catch (std::exception& ex) {
        // If error occurred print error and terminate program
        std::cout << ex.what() << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}