#include <scene.hpp>


void Scene::update(const float deltaTime) {
    // std::cout << "updating scene" << std::endl;

    // update camera
    camera.get()->update(deltaTime);

    }

void Scene::SampleHeight(glm::vec3 pos) const{
    if (!testChunk) {
        std::cout << "no test chunk!" << std::endl;
        return;
    }

    float chunkY = testChunk->approximateHeight(pos);

    //? could get the height of the 4 vertices the player touches and approximate here
    //? so if we are on the edge of a chunk we get the rest from the nearest chunk 

    std::cout << "pos:(" << pos.x << "," << pos.z <<")  posY:"<< pos.y << " chunkY:" << chunkY << std::endl;

    // get camera pos (we already have it as input)
    // glm::vec3 cam_pos = camera.get()->getPosition();

    // set camera y as chunkY + some height (i.e player height)
    pos.y = chunkY+10;

    //dont set camera pos
    // camera.get()->setPosition(pos);

}


void Scene::spawnTrees(std::shared_ptr<TerrainChunk> chunk, glm::vec2 testChunkStartPos, float ChunkSize, float ChunkRes){

    // this could be done in scene init and stored as member variable
    // init tree placer
    VariablePoissonDiscSampling treePlacer;

    // be carefull with radius and size because if too large then it could take a while to load (5,500,500) is starting to push it so add only 5 tries allowed (5,500,500,5)
    //? should we store tree points for further object spawning?
    std::vector<glm::vec2> treePoints;

    //! stress testing showed (3, 15, glm::vec2(startx,startz), glm::vec2(step,step),10) with step at 500 and max length being 5000 to take a minute to load
    //! testing showed random noise does better more dense forest maybe tweek simple for rolling hills
    //! making step 250 yielded better results

    //! important finding multiplying noise by some factor (3 in the test) increased density and frequency of forests

    // ! final configuration as stress test takes 1,5 minutes to load


    float step = ChunkRes/10; // found it looked better (however could look weird)
    
    float minRadius = Tree::minRadius; // about the size of the test tree
    float maxRadius = 5*minRadius; // found looked better

    // find spawn points
    int badMiddlesCounter = 0;
    for(float startz=-ChunkSize; startz <testChunkStartPos.y; startz +=step){
        for (float startx=testChunkStartPos.x; startx< ChunkSize; startx +=step){
                
            std::vector<glm::vec2> newtrees = treePlacer.GeneratePoints(minRadius, maxRadius, glm::vec2(startx,startz), glm::vec2(step,step),10);

            // if no trees where generated
            if (newtrees.empty()){
                badMiddlesCounter++;
                continue;
            }

            // Concatenate newtrees to treePoints
            treePoints.insert(treePoints.end(), newtrees.begin(), newtrees.end());
        }
    }

    std::cout << "found: " << badMiddlesCounter << " Bad middle points where no trees where spawned" << std::endl;
    
    // generate tree objects
    std::cout << "generating " << treePoints.size() << " points" << std::endl;
    for (auto point: treePoints) {
        std::shared_ptr<Tree> tree = std::make_shared<Tree>();
        //set model matrix to point location (also scale down because radius is 1 and model size is 2)

        //! carefull with the -Z for now
        float terrainY = chunk->approximateHeight(glm::vec3(point.x,0,point.y));

        // add external checks if point is valid

        // if it is in heavy snow dont spawn it (if its inside max optimal height zone) (we dont calculate zone here)
        if (terrainY > chunk->optimalHeight2) continue;

        // if it passed checks add it

        tree.get()->applyTransformation(glm::translate(glm::mat4(1), glm::vec3(point.x, terrainY ,point.y)));
        trees.push_back(tree);
    }

    std::cout << "spawning trees complete" << std::endl; 


}

void Scene::generateLights(){
    
    dirLights = { std::make_shared<DirectionalLight>(glm::vec3(5,5,10), eulerToQuat(glm::radians(-45.0),glm::radians(45.0),0)) }; 
    // DirectionalLight(glm::vec3(5,5,10), eulerToQuat(glm::radians(-45.0),glm::radians(180.0+45),0))
    
    pointLights =  { std::make_shared<PointLight>(glm::vec3(-8,10,10), DEFAULT_ORIENTATION, 50.0f,  glm::vec3(1,0,0)) }; // red for testing

    // could skip this 

    // set a sphere to see each light in debug mode (could later make all lights have this by default)
    for (auto &dirLight :dirLights){
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
}

