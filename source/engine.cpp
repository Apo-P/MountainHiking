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

    // allow window to be maximised
    glfwSetWindowAttrib(new_window, GLFW_MAXIMIZED, GLFW_TRUE);


    // Some GLFW options //! these should be initialized in controls

    // window keys to make sure states stay the same
    // When sticky keys mode is enabled, the pollable state of a key will remain the same until polled again
    glfwSetInputMode(new_window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited movement
    //! this wont work on WSL (because we dont have cursor access?)
    //! If cursor is disabled we cant maximisize screen
    glfwSetInputMode(new_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // window mouse check if enables raw input
    if (glfwRawMouseMotionSupported()) {
        std::cout << "rawmouse supported!" <<std::endl;
        glfwSetInputMode(new_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    //! if we are on wsl activate WSL bool


    //! these enables should be in renderer

    // Enable back face culling
    //! this will NOT render back faces of objects (even if we are inside them)
    glEnable(GL_CULL_FACE);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);  


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

// Test cube --------

Cube::Cube(GameEngine& eng, std::string modelMesh){

    std::string modelLocation = "resources/models/cube.obj";

    if(!modelMesh.empty()){
        modelLocation = modelMesh;
    }

    mesh = std::static_pointer_cast<Mesh>(std::make_shared<Mesh>(modelLocation));

    texture = std::make_shared<Texture>("resources/textures/grass1.png");

}

void Cube::update(GameEngine& engine){

}

class Circle : public Model {
    public:
        Circle(){
            mesh = std::static_pointer_cast<Mesh>(std::make_shared<Mesh>("resources/models/testCircle.obj"));

            // be carefull if no texture we will have a segmentation fault in texture renderer for now, maybe add check if obj has texture else use something else
            // texture = std::make_shared<Texture>("resources/textures/grass1.png");
        }

};

class TestTree : public Model {
    public:
        TestTree(){
            mesh = std::static_pointer_cast<Mesh>(std::make_shared<Mesh>("resources/models/mySimpleTree.obj"));

            this->baseModelMatrix = glm::scale(glm::mat4(1), glm::vec3(0.5));

            // be carefull if no texture we will have a segmentation fault in texture renderer for now, maybe add check if obj has texture else use something else
            // texture = std::make_shared<Texture>("resources/textures/simpleTreeTexturePallet.png");
        }

};

class TestSphere : public Model {
    public:
        TestSphere(){
            mesh = std::static_pointer_cast<Mesh>(std::make_shared<Mesh>("resources/models/sphere.obj"));

            // this->baseModelMatrix = glm::scale(glm::mat4(1), glm::vec3(0.5));
        }
};

#include "material.hpp"

class TestCube : public Model {

    public:

        TestCube(){
            mesh = std::static_pointer_cast<Mesh>(std::make_shared<Mesh>("resources/models/cube.obj"));

            // be carefull if no texture we will have a segmentation fault in texture renderer for now, maybe add check if obj has texture else use something else
            // texture = std::make_shared<Texture>("resources/textures/grass1.png");
            material = std::make_shared<Material>("resources/materials/testRustyMetal");
        }
};

void Renderer::testRender(std::shared_ptr<Model> obj){

    //! Clean this up after testing

    // 1.Bind shader
    testShader->bind();

    // 2.Bind VAO
    obj->bind();
    // 3.Bind texture
    obj->texture->bind(0);

    
    // 4.Send Uniforms
    testShader->sendUniform(Shader::uniforms::ModelMatrix , obj->getModelMatrix());
    // 5.Draw Triangles
    obj->draw(*this);


    //debug 
    if (DebugNormals) {
        normalDebugShader->bind();
        obj->bind();
        testShader->sendUniform(Shader::uniforms::ModelMatrix , obj->getModelMatrix());
        obj->draw(*this);
    }
}

void Renderer::renderSkybox(std::shared_ptr<Model> skybox){

    //! Clean this up after testing

    // 0. change depth function for this shader
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

    // 1.Bind shader
    skyboxShader->bind();

    // 2.Bind VAO
    skybox->bind();
    // 3.Bind texture
    skybox->texture->bind();
    
    // 4.Send Uniforms

    // 5.Draw Triangles
    skybox->draw(*this);

    // 6.reset depth function
    glDepthFunc(GL_LESS); // set depth function back to default 

}

void Renderer::renderTerrain(std::shared_ptr<TerrainChunk> terrainChunk){

    // TEST RENDER -----
    // 1.Bind shader
    terrainShader->bind();
    // 2.Bind VAO
    terrainChunk->mesh->bind();
    // 3.Bind texture
    terrainChunk->textureHeight0->bind(0);
    terrainChunk->textureHeight1->bind(1);
    terrainChunk->textureHeight2->bind(2);
    
    // 4.Send Uniforms
    terrainShader->sendUniform(Shader::uniforms::ModelMatrix , glm::mat4(1)); // or send model matrix if it has one
    terrainShader->sendUniform(Shader::uniforms::Terrain , true);
    // 5.Draw Triangles
    terrainChunk->mesh->draw(*this);

    //! Temporary will change once we implement materials
    //reset terrain to false after we are done drawing terrain
    testShader->sendUniform(Shader::uniforms::Terrain , false);



    //debug 
    if (DebugNormals){
        normalDebugShader->bind();
        terrainChunk->mesh->bind();
        normalDebugShader->sendUniform(Shader::uniforms::ModelMatrix , glm::mat4(1));
        terrainChunk->mesh->draw(*this);
    }


    // END TEST RENDER ---
}

void Renderer::renderPBR(std::shared_ptr<Model> obj, const glm::vec3 &cameraPos){

    //! Clean this up after testing

    // 1.Bind shader
    pbrShader->bind();

    // 2.Bind VAO
    obj->bind();
    // 3.Bind texture
    obj->material->bind(3);
    
    // 4.Send Uniforms
    // //! change later to be done in shader class   
    GLuint programId = pbrShader->getProgramId();
    GLuint uniformLocation = glGetUniformLocation(programId, "cameraPosition");
    glUniform3fv(uniformLocation, 1, glm::value_ptr(cameraPos));

    pbrShader->sendUniform(Shader::uniforms::ModelMatrix , obj->getModelMatrix());

    // 5.Draw Triangles
    obj->draw(*this);

}

class testHeightGen : public HeightGenerator {
    std::unordered_map<std::pair<float,float>, float, FloatPairHash> makeHeightMap (std::vector<glm::vec3> positions, int seed) override{

        std::unordered_map<std::pair<float,float>, float, FloatPairHash> returnHeightMap;

        for(auto pos : positions){
        
            float x = pos.x;
            float z = pos.z;
    
            //store height
            returnHeightMap[std::make_pair(x,z)] = 0;
    
        }

        return returnHeightMap;
    }
};


//TODO add this to a header when done with testing
#include <terrainChunkManager.hpp>
#include <objectPlacer.hpp>
#include <skybox.hpp>



int GameEngine::startGame() {
    
    
    // ! Be carefull with scope
    using std::vector,glm::vec3;

    
    // Do initializers
    initialize();

    try
    {
        // testing ----

        // Test wireframe mode
        // renderer.get()->changeMode(RenderModes::wireFrame);
        // renderer.get()->DebugNormals = true;

        vector<VertexData> triangle_vertices = {
            VertexData(vec3(-0.5f, -0.5f, 0.0f)),
            VertexData(vec3( 0.5f, -0.5f, 0.0f)),
            VertexData(vec3( 0.0f,  0.5f, 0.0f))
        };

        std::shared_ptr<Mesh> triangle = std::make_shared<Mesh>(triangle_vertices);

        std::shared_ptr<Cube> cube = std::static_pointer_cast<Cube>(std::make_shared<Cube>(*this,"resources/models/cube.obj"));

        std::shared_ptr<Circle> circle = std::static_pointer_cast<Circle>(std::make_shared<Circle>());

        // set test circle to approcimate radius of test tree (about 3 )
        circle.get()->setModelMatrix(glm::scale(glm::mat4(1), glm::vec3(3)));

        std::shared_ptr<TestTree> testTree = std::static_pointer_cast<TestTree>(std::make_shared<TestTree>());
        testTree->texture = std::make_shared<Texture>("resources/textures/simpleTreeTexturePallet.png");

        //make a skybox

        std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>();


        // ChunkManager* chunkmanager = new ChunkManager(21,3);


        std::shared_ptr<Shader> testShader = renderer.get()->testShader;
        testShader->bind();
        std::shared_ptr<Shader> testNormalDebugShader = renderer.get()->normalDebugShader;
        std::shared_ptr<Texture> testTexture = std::make_shared<Texture>("resources/textures/dryDirt.png");
        std::shared_ptr<Texture> testTextureTwo = std::make_shared<Texture>("resources/textures/grass1.png");
        std::shared_ptr<Texture> testTextureThree = std::make_shared<Texture>("resources/textures/snow1.png");

        



        glm::mat4 testModelMatrix = glm::translate(glm::mat4(1),glm::vec3(0,0,0));

        HeightGenerator* gen = new HeightGenerator();
        HeightGenerator* test_gen = new testHeightGen();


        std::shared_ptr<TerrainChunk> testchunk = std::make_shared<TerrainChunk>(*gen,0,0,2000,400); // chunk size of fifty and resolution of 10 -> each grid square is 50/10
        testchunk->generateChunk();

        //load texture into chunk
        //? this should be done my terrain chunk manager or by chunk its self at creation
        //? for example chunk could have special values, flags
        testchunk->textureHeight0 = testTexture;
        testchunk->textureHeight1 = testTextureTwo;
        testchunk->textureHeight2 = testTextureThree;



        VariablePoissonDiscSampling objectPlacer;

        // be carefull with radius and size because if too large then it could take a while to load (5,500,500) is starting to push it so add only 5 tries allowed (5,500,500,5)
        std::vector<glm::vec2> treePoints; //= objectPlacer.GeneratePoints(3, 15, glm::vec2(0,0), glm::vec2(500,500),30);
        


        //! stress testing showed (3, 15, glm::vec2(startx,startz), glm::vec2(step,step),10) with step at 500 and max length being 5000 to take a minute to load
        //! testing showed random noise does better more dense forest maybe tweek simple for rolling hills
        //! making step 250 yielded better results

        //! important finding multiplying noise by some factor (3 in the test) increased density and frequency of forests

        //! final configuration as stress test takes 1,5 minutes to load

        float step = 250;

        // for(float startz=-5000; startz <0; startz +=step){
        //     for (float startx=0; startx< 5000; startx +=step){
                 
        //         std::vector<glm::vec2> newtrees = objectPlacer.GeneratePoints(3, 15, glm::vec2(startx,startz), glm::vec2(step,step),10);

        //         // Concatenate newtrees to treePoints
        //         treePoints.insert(treePoints.end(), newtrees.begin(), newtrees.end());
        //     }
        // }

        std::vector<std::shared_ptr<TestTree>> trees;
        for (auto point: treePoints) {
            std::shared_ptr<TestTree> tree = std::make_shared<TestTree>();
            //set model matrix to point location (also scale down because radius is 1 and model size is 2)

            //! carefull with the -Z for now
            float terrainY = testchunk->approximateHeight(glm::vec3(point.x,0,point.y));

            tree.get()->applyTransformation(glm::translate(glm::mat4(1), glm::vec3(point.x, terrainY ,point.y)));
            trees.push_back(tree);
        }


        // pbr test
        std::shared_ptr<TestCube> testCube = std::make_shared<TestCube>();
        testCube->applyTransformation(glm::translate(glm::mat4(1), glm::vec3(-10,10,0)));
        // test lights
        //! should be controlled by a scene
        std::vector<std::shared_ptr<DirectionalLight>> dirLights = { std::make_shared<DirectionalLight>(glm::vec3(5,5,10), eulerToQuat(glm::radians(-45.0),glm::radians(45.0),0)) }; 
        // DirectionalLight(glm::vec3(5,5,10), eulerToQuat(glm::radians(-45.0),glm::radians(180.0+45),0))
        
        std::vector<std::shared_ptr<PointLight>> pointLights =  { std::make_shared<PointLight>(glm::vec3(-8,10,10), DEFAULT_ORIENTATION, 50.0f,  glm::vec3(1,0,0)) }; // red for testing

        // set a sphere to see each light in debug mode (could later make all lights have this by default)
        for (auto &dirLight : dirLights){
            // make a new model
            std::shared_ptr<TestSphere> newModel = std::make_shared<TestSphere>();
            // translate it to current pos
            newModel->setModelMatrix(glm::translate(glm::mat4(1),dirLight->getPosition()));
            // give it to obj
            dirLight->setModel(newModel);
        }
        for (auto &pointLight : pointLights){
            // make a new model
            std::shared_ptr<TestSphere> newModel = std::make_shared<TestSphere>();
            // translate it to current pos
            newModel->setModelMatrix(glm::translate(glm::mat4(1),pointLight->getPosition()));
            // give it to obj
            pointLight->setModel(newModel);
        }

        renderer->sendLights(dirLights, pointLights);
      



        //make a camera   //?be 5 units up and look a bit down
        std::shared_ptr<Camera> mainCamera = std::make_shared<Camera>(glm::vec3(0.0f, 5.0f, 0.0f), eulerToQuat(-45,0,0));

        //make a scene
        Scene mainScene(mainCamera);

        // upload initial projection matrix 
        //! no need to change often unless if new projection matrix
        renderer.get()->sendProjectionMatrix(mainScene.camera.get()->getProjection());
        


        cube.get()->setModelMatrix(glm::translate(glm::mat4(1), vec3(0,0,-2)));

        
        mainScene.setTestChunk(*testchunk); //apparently we can pass the reference of a shared pointer without issue


        // post processing test
        const int W_WIDTH = 1024;
        const int W_HEIGHT = 768;
        std::shared_ptr<Shader> effectShader = std::make_shared<Shader>("shaders/postProcessingShaders/effects.vert", "shaders/postProcessingShaders/effects.frag");
        postProcessingEffects = std::make_unique<PostProcessor>(*effectShader.get(), W_WIDTH, W_HEIGHT);

        float keyCooldown =-1;
        float KEY_COOLDOWN_TIME = 0.5f;

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

            //! post processor start before main render
            postProcessingEffects->BeginRender();
            glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
            

            // Clear Buffers / Screen //! this should be in renderer
            // dont forget to clear depth buffer
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

            // Event handling / Update game state //! this should be in scene

            

            //! this needs to be done every frame
            renderer.get()->sendViewMatrix(mainScene.camera.get()->getView());

            // update scene

            mainScene.update(deltaTime);

            // Render into framebuffer //! this should be in renderer
            
            

            renderer.get()->SimpleRender(triangle);
            //use test render for cube
            // renderer.get()->testRender(cube);

            // test render pbr
            renderer->renderPBR(testCube, mainScene.camera->getPosition());

            // render a sphere to see each light in debug mode (could later make all lights have this by default)
            for (auto dirLight : dirLights){
                renderer->SimpleRender(dirLight);
            }
            for (auto &pointLight : pointLights){
                renderer->SimpleRender(pointLight);
            }

            renderer.get()->SimpleRender(circle);

            renderer.get()->testRender(testTree);

            for(auto& tree : trees){
                renderer.get()->SimpleRender(tree);
            }

            // renderer.get()->SimpleRender(plane->mesh);


            // for (auto keyValuePair : chunkmanager->chunks) {
            //     // get the chunk pointer for key,value pair
            //     TerrainChunk* chunk = keyValuePair.second.get();

            //     // renderer.get()->SimpleRender(chunk->mesh, glm::translate(glm::mat4(1),glm::vec3(0,0,0)));

            //     testShader->bind();
            //     chunk->mesh->bind();

            //     testTextureTwo.get()->bind(0);
            //     testTexture.get()->bind(1);
            //     testTextureThree.get()->bind(2);

            //     testShader->sendUniform(Shader::uniforms::ModelMatrix , testModelMatrix);
            //     testShader->sendUniform(Shader::uniforms::Terrain , true);
                
            //     chunk->mesh->draw(*renderer.get());

            //     testShader->sendUniform(Shader::uniforms::Terrain , false);

            //     if (renderer.get()->DebugNormals){
            //         testNormalDebugShader->bind();
            //         chunk->mesh->bind();
            //         testNormalDebugShader->sendUniform(Shader::uniforms::ModelMatrix , testModelMatrix);
            //         chunk->mesh->draw(*renderer.get());
            //     }

            // }


            
            // render terrain
            renderer->renderTerrain(testchunk);


            // render objects


            //! RENDER SKYBOX LAST
            renderer->renderSkybox(skybox);



            
            //! post processor ends after main render
            postProcessingEffects->EndRender();

            //? here scene could give flags to post preccessor 
            // manual for now
            if (keyCooldown < 0) {
                if (glfwGetKey(window.get(), GLFW_KEY_E) == GLFW_PRESS) { 
                    if (postProcessingEffects->Bobbing) postProcessingEffects->Bobbing = false;
                    else postProcessingEffects->Bobbing = true;
                }
            }
            keyCooldown = keyCooldown < 0 ? KEY_COOLDOWN_TIME : keyCooldown - deltaTime; 


            //! post processor renders effects
            postProcessingEffects->Render(glfwGetTime());

            //! then we show screen


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