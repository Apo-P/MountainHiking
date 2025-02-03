#include <object.hpp>

/// @brief takes euler angles and converts them to a quaternion
/// @param pitch Rotation around X
/// @param yaw Rotation around Y
/// @param roll Rotation around Z
/// @return normalized quaternion
glm::quat eulerToQuat(const float pitch, const float yaw, const float roll) {
    glm::quat rotationX = glm::angleAxis(pitch, glm::vec3(1,0,0));
    glm::quat rotationY = glm::angleAxis(yaw, glm::vec3(0,1,0));
    glm::quat rotationZ = glm::angleAxis(roll, glm::vec3(0,0,1));

    return normalize(rotationZ * rotationY * rotationX);
}

/// @brief Default constructor
/// @param position position
/// @param orientation orientation
/// @param scale scale
Object::Object(const glm::vec3 position, const glm::quat orientation, const glm::vec3 scale) {
    this->position = position;
    setOrientation(orientation);
    this->scale = scale;
}


// Cartesian vectors

glm::vec3 Object::up() const {
    return orientation * glm::vec3(0,1,0); //+y is up
}
glm::vec3 Object::forward() const {
    return orientation * glm::vec3(0,0,-1); //-z is front
}
glm::vec3 Object::right() const {
    return orientation * glm::vec3(1,0,0); //+x is right
}

// Setters 

void Object::setPosition(const glm::vec3 position) {
    this->position = position;
}
void Object::setOrientation(const glm::quat orientation) {
    this->orientation = glm::normalize(orientation);
}
void Object::setScale(const glm::vec3 scale) {
    this->scale = scale;
}


// Transformations

void Object::translate(const glm::vec3 translation) {
    // update position
    position += translation;
}
void Object::rotate(const glm::quat rotation) {
    // update rotation
    glm::quat rotationQuat = glm::normalize(rotation);

    orientation = glm::normalize(rotationQuat * orientation);
}
void Object::rotate(const float angle, const glm::vec3 axis) {
    // update rotation
    glm::quat rotationQuat = glm::normalize(angleAxis(angle, axis));

    orientation = glm::normalize(rotationQuat * orientation);
}
void Object::changeScale(const glm::vec3 scaleFactor) {
    // update scale
    scale += scaleFactor;
}
