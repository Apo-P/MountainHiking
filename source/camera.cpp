
#include <camera.hpp>

Camera::Camera(const glm::vec3 position, const glm::quat orientation) :position(position), orientation(orientation) {
    updateViewMatrix();
    updateProjectionMatrix();
}

// Cartesian vectors
glm::vec3 Camera::up() const {
    return orientation * glm::vec3(0,1,0); //+y is up
}
glm::vec3 Camera::forward() const {
    return orientation * glm::vec3(0,0,-1); //-z is front
}
glm::vec3 Camera::right() const {
    return orientation * glm::vec3(1,0,0); //+x is right
}

// Transformations
void Camera::translate(const glm::vec3 translation) {
    // update position
    position += translation;

    // update view matrix
    updateViewMatrix();
}
void Camera::rotate(const glm::quat rotation) {
    // update rotation
    glm::quat rotationQuat = glm::normalize(rotation);

    orientation = glm::normalize(rotationQuat * orientation);

    // update view matrix
    updateViewMatrix();
}
void Camera::rotate(const float angle, const glm::vec3 axis) {
    // update rotation
    glm::quat rotationQuat = glm::normalize(angleAxis(angle, axis));

    orientation = glm::normalize(rotationQuat * orientation);

    // update view matrix
    updateViewMatrix();
}
void Camera::changeScale(const glm::vec3 scaleFactor) {
    // update scale
    scale += scaleFactor;

    // update view matrix
    updateViewMatrix();
}

void Camera::look(const float pitch, const float yaw) {
    // calculate new orientation

    glm::quat pitchRotation = glm::angleAxis(pitch, glm::vec3(1,0,0));
    glm::quat yawRotation = glm::angleAxis(yaw, glm::vec3(0,1,0));

    orientation = yawRotation * orientation * pitchRotation;

    if (glm::dot(up(), glm::vec3(0,1,0)) <= 0) {
        glm::vec3 newForward = glm::vec3(0,1,0);
        if (glm::dot(forward(), glm::vec3(0,-1,0)) > 0.99) {
            newForward = glm::vec3(0,-1,0);
        }

        orientation = glm::normalize(glm::quatLookAt(newForward, up()));
    }

    // update view matrix
    updateViewMatrix();
}


void Camera::lookAt(const glm::vec3 target, const glm::vec3 up, const glm::vec3 alternativeUp) {
    // calculate new orientation

    // make sure target isn't in the same postion
    assert(target != position);

    glm::vec3 direction = target - position;

    if(glm::length(direction) < 0.0001) {
        orientation = glm::quat(1, 0, 0, 0);
        return;
    }

    direction = glm::normalize(direction);

    // Is the normal up (nearly) parallel to direction?
    if(glm::abs(glm::dot(direction, up)) > .9999f) {
        orientation = glm::normalize(glm::quatLookAt(direction, alternativeUp));
    }
    else {
        orientation = glm::normalize(glm::quatLookAt(direction, up));
    }

    // update view matrix
    updateViewMatrix();
}


Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
    setPosition(position);
    lookAt(target, up);
    updateViewMatrix();
    updateProjectionMatrix();
};

void Camera::updateViewMatrix() {
    viewMatrix = glm::lookAt(position, position + forward(), up());
}

void Camera::updateProjectionMatrix() {
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearCP, farCP);
}


void Camera::updateAspectRatio(glm::vec2 width_height) {
    
    aspectRatio = (float) width_height.x / (float) width_height.y;

    updateProjectionMatrix();
}

void Camera::zoom(const float amount) {
    fov += amount;

    updateProjectionMatrix();
}


void Camera::setPosition(const glm::vec3 position) {
    this->position = position;
    updateViewMatrix();
}

void Camera::setOrientation(const glm::quat orientation) {
    this->orientation = glm::normalize(orientation);
    updateViewMatrix();
}
