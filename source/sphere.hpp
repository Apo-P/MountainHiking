#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "model.hpp"

class TestSphere : public Model {
    public:
        TestSphere(){
            mesh = std::static_pointer_cast<Mesh>(std::make_shared<Mesh>("resources/models/sphere.obj"));

            // this->baseModelMatrix = glm::scale(glm::mat4(1), glm::vec3(0.5));
        }
};

#endif
