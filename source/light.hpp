#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "common.hpp"

#include "object.hpp"

class Light : public Object {
protected:
    glm::vec3 color = glm::vec3(1,1,1);
    float power = 10;

public:
    Light() : Light(DEFAULT_POSITION, DEFAULT_ORIENTATION, 10, glm::vec3(1,1,1)) {}
    Light(const glm::vec3 position) : Light(position, DEFAULT_ORIENTATION, 10, glm::vec3(1,1,1)) {}
    Light(const glm::vec3 position, const glm::quat rotation) : Light(position, rotation, 10, glm::vec3(1,1,1)) {}
    Light(const glm::vec3 position, const glm::quat rotation, const float power) : Light(position, rotation, power, glm::vec3(1,1,1)) {}

    // Constructor
    Light(const glm::vec3 position, const glm::quat rotation, const float power,const glm::vec3 color) : 
        Object(position, rotation), 
        color(color), 
        power(power) { }

    // return relevant data
    virtual std::vector<glm::vec4> data() = 0;
    // return size of data
    constexpr static size_t sizeofData() {
        return 2*sizeof(glm::vec4);
    }
};

class PointLight : public Light {
    public:
        //using lights constructor
        using Light::Light;

        std::vector<glm::vec4> data() override;
};

class DirectionalLight : public Light {
    public:
        // using lights constructor
        using Light::Light;

        std::vector<glm::vec4> data() override;

};

#endif