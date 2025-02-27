#include "skybox.hpp"

Skybox::Skybox() {

    mesh = std::make_shared<Mesh>("resources/models/skybox.obj");

    //! be carefull with order!
    texture = std::make_shared<Cubemap>(std::vector<std::string>{
        "resources/cubemaps/skybox/right.jpg",
        "resources/cubemaps/skybox/left.jpg",
        "resources/cubemaps/skybox/top.jpg",
        "resources/cubemaps/skybox/bottom.jpg",
        "resources/cubemaps/skybox/front.jpg",
        "resources/cubemaps/skybox/back.jpg",
    });
}