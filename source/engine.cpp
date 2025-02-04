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
class Cube:public Model {
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

class Plane {

    private:
    int width;
	int height;
	int widthSegments;
	int heightSegments;

    

    std::vector<VertexData> createVertices(float width = 1, float height = 1, int widthSegments = 1, int heightSegments = 1);

    public:
    // TODO: make getter
    std::shared_ptr<Mesh> mesh;

    Plane(float width = 1, float height = 1, int widthSegments = 1, int heightSegments = 1);
};

std::vector<VertexData> Plane::createVertices(float width, float height, int widthSegments, int heightSegments) {
    //to have it go from -half_width to +half_width in model space
    float width_half = width / 2;
    float height_half = height / 2;

    // store segments
    int gridX = widthSegments;
    int gridY = heightSegments;

    // add +1 to create enough vertices
    int gridX1 = gridX + 1;
    int gridY1 = gridY + 1;

    // calculate segment width and height
    float segment_width = width / gridX;
    float segment_height = height / gridY;

    // create arrays for vertex data
    std::vector<glm::vec3> indices;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;


    float x,y;
    for(int iy = 0; iy < gridY1; iy++) {

        y = iy * segment_height - height_half;

        for(int ix = 0; ix < gridX1; ix++) {

            x = ix * segment_width - width_half;

            positions.push_back(glm::vec3(x, -y, 0));

            normals.push_back(glm::vec3(0, 0, 1));

            uvs.push_back( glm::vec2( ix / gridX ));
            uvs.push_back( glm::vec2( 1 - ( iy / gridY )) );

        }

    }

    // indice data
    unsigned int a,b,c,d;
    for (int iy = 0; iy < gridY; iy ++ ) {

        for (int ix = 0; ix < gridX; ix ++ ) {

            a = ix + gridX1 * iy;
            b = ix + gridX1 * ( iy + 1 );
            c = ( ix + 1 ) + gridX1 * ( iy + 1 );
            d = ( ix + 1 ) + gridX1 * iy;

            indices.push_back(glm::vec3 (a, b, d) );
            indices.push_back(glm::vec3 (b, c, d) );

        }

    }

    // pack data to vertex data
    std::vector<VertexData> vertexData;
    for (auto index :indices) {
        vertexData.push_back(VertexData(positions.at(index.x), uvs.at(index.x) ,normals.at(index.x)));
        vertexData.push_back(VertexData(positions.at(index.y), uvs.at(index.y) ,normals.at(index.y)));
        vertexData.push_back(VertexData(positions.at(index.z), uvs.at(index.z) ,normals.at(index.z)));
    }

    return vertexData;
}

Plane::Plane(float width, float height, int widthSegments, int heightSegments) : 
        width(width),
        height(height),
        widthSegments(widthSegments),
        heightSegments(heightSegments) {

        std::vector<VertexData> vertexData;

        vertexData = createVertices(width, height, widthSegments, heightSegments);

        mesh = std::make_shared<Mesh>(vertexData);


        };


class Terrain: public Model {
    public:
        /// @brief constructor
        /// ! add terrain location for loading terrain
        Terrain(int size);

        float chunkSize=10;

        /// @brief create chunk at x,z
        void createChunk(int x,int z){
            glm::vec2 offset = glm::vec2(x * chunkSize, z * chunkSize);
        };
};

Terrain::Terrain(int size) {
    // createChunk(size);
}

//generating func
class Generator {

    static double sat(double x){
      return glm::min(glm::max(x, 0.0), 1.0);
    }

    public:
    Generator(){};

    static const int chunkCenter = 250;
    static const int maxHeight = 128;
    // static glm::vec2 chuckStartinPoint = glm::vec2(0,0);
    

    static float Get(float x, float y) {
        float distance = glm::distance(glm::vec2(0,0), glm::vec2(x,y));

        // test
        float t = sat(1.5);

        // h = height
        float h = 1.0 - sat(distance / chunkCenter);
        h = h * h * h * (h * (h * 6 - 15) + 10);

        //test 
        if (distance <200){
         t=t+1;   
        }

        // return height, normalization
        float height = h * maxHeight; //max height
        return height;//128; //, 1;
    }

};


// #include <functional> // for std::function
// could using this instead of typedef 

#include <utility> // for std::pair

// define a pointer to a function that takes 2 floats and return 1 float
typedef float (*generatorGet) (float, float);


class HeightGenerator {

    glm::vec2 offset;
    glm::vec2 radius;
    /// @brief generator
    std::shared_ptr<Generator> generator;

    double sat(double x){
      return glm::min(glm::max(x, 0.0), 1.0);
    }

    public:

    HeightGenerator(std::shared_ptr<Generator> generator, glm::vec2& new_offset, float minRadius, float maxRadius) :
        generator(generator),
        offset(new_offset)
    {
        radius = glm::vec2(minRadius, maxRadius);
    }

    std::pair< float, float > Get(const float x, const float y) {

        float distance = glm::distance(offset, glm::vec2(x,y));
        
        float normalization = 1.0 - sat(
            (distance - radius.x) / (radius.y - radius.x));


        normalization = normalization * normalization * (3 - 2 * normalization);

        float height = generator.get()->Get(x, y); 

        return {height, normalization};
    }

};

class TerrainChunkManager {

    public:
        int chunkGroup; //change later

    TerrainChunkManager(){}; //change later

};

class TerrainChunk {
    // constructor(params) {
    // this._params = params;
    // this._Init(params);
    // }

    glm::vec3 size;
    // how many faces will it have
    int resolution = 128;
    glm::vec3 offset;

    public:
    Plane* plane; //temporary

    TerrainChunk(TerrainChunkManager &TerrainManager, glm::vec3 offset=glm::vec3(0), float chunkSize=500 , float scale=1) {  //!Terrain will be controller

        // size
        size = glm::vec3(chunkSize * scale, 0, chunkSize * scale);

        //store offset
        offset = offset;

        //make plane 
        plane = new Plane(size.x, size.z, resolution, resolution);
        // sets plane's position
        // plane.position = offset

        // add plane mesh to chunk group in Terrain Manager

        // make plane into chunk
        makeChunk();

    }


    void makeChunk() {

        // for testing
        float _chunkSize =500;
        float x = 0,z=0;
        glm::vec2 offset = glm::vec2(x * _chunkSize, z * _chunkSize);

        std::vector<HeightGenerator> heightGenerators = { HeightGenerator(std::make_shared<Generator>(), offset, 100000, 100000 + 1) };

        // for every vertex
        int index=0;
        for (auto vertex :plane->mesh.get()->getVertexData()) {

            std::vector< std::pair<float, float> > heightPairs {};
            float normalization = 0;
            vertex.position.z = 0;

            if (vertex.position.x >240){

            }

            

            for (auto gen :heightGenerators) {
                heightPairs.push_back(gen.Get(vertex.position.x + offset.x, vertex.position.y + offset.y));
                
                normalization += heightPairs.at(heightPairs.size() - 1 ).second;
            }

            if (normalization > 0) {
                // TODO CHANGE THIS LATER
                float newZ=0;
                for (auto h :heightPairs) {

                    // vertex.position.z += h.first * h.second / normalization;
                    newZ +=  h.first * h.second / normalization;
                }
                if (newZ != 0) {
                    plane->mesh->updateVertexZ(index, newZ);
                }
            }
            index++;
        }


        // update mesh
        plane->mesh.get()->updateVram();


    }

};




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

        std::shared_ptr<Model> cube = std::static_pointer_cast<Model>(std::make_shared<Cube>(*this,"resources/models/cube.obj"));

        Plane* plane = new Plane(4,4,5,4);
        

        TerrainChunkManager terrain = TerrainChunkManager();

        TerrainChunk* chunk = new TerrainChunk(terrain);



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


            // update scene

            mainScene.update(deltaTime);

            // Render into framebuffer //! this should be in renderer
            

            // renderer.get()->SimpleRender(triangle);
            // renderer.get()->SimpleRender(cube);

            // renderer.get()->SimpleRender(plane->mesh);

            renderer.get()->SimpleRender(chunk->plane->mesh, glm::rotate(glm::mat4(1), (float)(-3.14/2), vec3(1,0,0)));


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