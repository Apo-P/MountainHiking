#include <scene.hpp>


void Scene::update(const float deltaTime) {
    // std::cout << "updating scene" << std::endl;

    // update camera
    camera.get()->update(deltaTime);

    }