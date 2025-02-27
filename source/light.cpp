#include "light.hpp"

std::vector<glm::vec4> PointLight::data() {
    std::vector<glm::vec4> data = {
        glm::vec4(color, power), // color and power
        glm::vec4(position, 1) // position
    };

    return data;
}

std::vector<glm::vec4> DirectionalLight::data() {
    std::vector<glm::vec4> data = {
        glm::vec4(color, power), // color and power
        glm::vec4(forward(), 0) // direction
    };

    return data;
}
