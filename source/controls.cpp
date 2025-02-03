#include "controls.hpp"


bool Controls::isKeyPressed(int key) {
    if (keyCooldown < 0) return glfwGetKey(window.get(), key) == GLFW_PRESS;
    return false;
}

bool Controls::isKeyHeld(int key) {
    return glfwGetKey(window.get(), key) == GLFW_PRESS;
}

void Controls::handleMouse(const Scene& scene ,const float deltaTime) {

    if(WSL){
        std::cout << "we are on WSL! Not handling mouse!" << std::endl;
        return;
    }

    // TODO: handle mouse

}

void Controls::handleKeyboard(const Scene& scene, const float deltaTime) {
    if(WSL){
        std::cout << "we are on WSL! Not handling keyboard!" << std::endl;
        return;
    }
    // TODO: handle keyboard
}


void Controls::handleWSL(const Scene& scene, const float deltaTime) {
    // std::cout << "handling wsl" << std::endl;

    // window to poll
    GLFWwindow* pollWindow = window.get();

    // get window width and height
    int width, height;
    glfwGetWindowSize(pollWindow, &width, &height);

    // mouse handling

    // simulate mouse position
    double mouseX, mouseY;

    // viewspeed
    double viewspeed = 1.0;

    // Look right
    if (isKeyHeld(GLFW_KEY_KP_6)) {
        mouseX = +viewspeed;
    }
    // look left
    if (isKeyHeld(GLFW_KEY_KP_4)) {
        mouseX = -viewspeed;
    }
    // look down
    if (isKeyHeld(GLFW_KEY_KP_5)) {
        mouseY = +viewspeed;
    }
    // look up
    if (isKeyHeld(GLFW_KEY_KP_8)) {
        mouseY =  -viewspeed;
    }

    // left mouse press 
    if (isKeyPressed(GLFW_KEY_KP_7)) {
        std::cout << "LEFT MOUSE BUTTON PRESSED" << std::endl;
    }

    
    // Pass to camera
    float pitch = -mouseY * viewspeed * deltaTime;
    float yaw = -mouseX * viewspeed * deltaTime;

    scene.camera.get()->look(pitch, yaw); 

    // keyboard handling
    
    Camera* camera = scene.camera.get();
    float movementSpeed = 1.0f;


    // move foward
    if (isKeyPressed(GLFW_KEY_W)) {
        //TODO move these to a function
        // get forward vector
        glm::vec3 forwardVector = camera->forward();

        //! change velocity to be smooth and later update, but for now translate immediately
        camera->translate(forwardVector * movementSpeed);

    }
    // move backwards
    if (isKeyPressed(GLFW_KEY_S)) {
        //TODO move these to a function
        // get forward vector
        glm::vec3 forwardVector = camera->forward();

        //! change velocity to be smooth and later update, but for now translate immediately
        camera->translate( -forwardVector * movementSpeed);
    }
    // move left
    if (isKeyPressed(GLFW_KEY_A)) {
        //TODO move these to a function
        // get right vector
        glm::vec3 rightVector = camera->right();

        //! change velocity to be smooth and later update, but for now translate immediately
        camera->translate( -rightVector * movementSpeed);
    }
    // move right
    if (isKeyPressed(GLFW_KEY_D)) {
        //TODO move these to a function
        // get right vector
        glm::vec3 rightVector = camera->right();

        //! change velocity to be smooth and later update, but for now translate immediately
        camera->translate( rightVector * movementSpeed);
    }

}


void Controls::handleInputs(const Scene& scene, const float deltaTime) {

    // calculate if keys are on cooldown
    keyCooldown = keyCooldown < 0 ? KEY_COOLDOWN_TIME : keyCooldown - deltaTime; 

    // check input mode
    if(WSL){
        // if on wsl
        // std::cout << "we are on WSL! changing input mode!" << std::endl;
        handleWSL(scene, deltaTime);
        return;
    }
        // if on normal environment
        std::cout << "we are on normal hardware! changing input mode!" << std::endl;
        handleMouse(scene, deltaTime);
        handleKeyboard(scene, deltaTime);
}