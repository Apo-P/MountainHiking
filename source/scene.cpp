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

    std::cout << "pos:(" << pos.x << "," << pos.z <<") chunkY:" << chunkY << std::endl;

    // get camera pos (we already have it as input)
    // glm::vec3 cam_pos = camera.get()->getPosition();

    // set camera y as chunkY + some height (i.e player height)
    pos.y = chunkY+5;

    camera.get()->setPosition(pos);

}