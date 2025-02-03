
#include <camera.hpp>

Camera::Camera(const glm::vec3 position, const glm::quat orientation) : Object(position, orientation) {
    updateViewMatrix();
    updateProjectionMatrix();
}



// Transformations
void Camera::translate(const glm::vec3 translation) {
    // update position
    Object::translate(translation);

    // update view matrix
    updateViewMatrix();
}
void Camera::rotate(const glm::quat rotation) {
    // update rotation
    Object::rotate(rotation);

    // update view matrix
    updateViewMatrix();
}
void Camera::rotate(const float angle, const glm::vec3 axis) {
    // update rotation
    Object::rotate(angle, axis);

    // update view matrix
    updateViewMatrix();
}
void Camera::changeScale(const glm::vec3 scaleFactor) {
    // update scale
    Object::changeScale(scaleFactor);

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
    Object::setPosition(position);
    updateViewMatrix();
}

void Camera::setOrientation(const glm::quat orientation) {
    Object::setOrientation(orientation);
    updateViewMatrix();
}



void Camera::moveForward() {
    // get forward vector
    glm::vec3 forwardVector = forward();

    //if we have velocity normalize it to add new direction
    if (glm::length(velocity) > 0) velocity = normalize(velocity);
    velocity += forwardVector;

    // calculate velocity
    velocity = normalize(velocity) * movementSpeed;
}

void Camera::moveBackward() {
    glm::vec3 forwardVector = forward();

    //if we have velocity normalize it to add new direction
    if (glm::length(velocity) > 0) velocity = normalize(velocity);
    velocity += -forwardVector;
    
    // calculate velocity
    velocity = normalize(velocity) * movementSpeed;
}

void Camera::moveLeft() {
    // get right vector
    glm::vec3 rightVector = right();
 
    //if we have velocity normalize it to add new direction
    if (glm::length(velocity) > 0) velocity = normalize(velocity);
    velocity += -rightVector;

    // calculate velocity
    velocity = normalize(velocity) * movementSpeed;
}

void Camera::moveRight() {
    // get right vector
    glm::vec3 rightVector = right();
 
    //if we have velocity normalize it to add new direction
    if (glm::length(velocity) > 0) velocity = normalize(velocity);
    velocity += rightVector;

    // calculate velocity
    velocity = normalize(velocity) * movementSpeed;
}

void Camera::update(const float deltaTime) {
    // apply translation
    translate(velocity * deltaTime);
    //reset velocity for next check
    velocity = glm::vec3(0);
}