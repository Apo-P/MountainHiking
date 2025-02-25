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