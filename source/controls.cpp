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


    // move foward
    if (isKeyHeld(GLFW_KEY_W)) {
        camera->moveForward();
    }
    // move backwards
    if (isKeyHeld(GLFW_KEY_S)) {
        camera->moveBackward();
    }
    // move left
    if (isKeyHeld(GLFW_KEY_A)) {
        camera->moveLeft();
    }
    // move right
    if (isKeyHeld(GLFW_KEY_D)) {
        camera->moveRight();
    }
    // move up
    if (isKeyHeld(GLFW_KEY_SPACE)) {
        camera->moveUp();
    }
    // move down
    if (isKeyHeld(GLFW_KEY_LEFT_CONTROL)) {
        camera->moveDown();
    }
    // change movement speed while key is held
    //? change in movement speed could bew calculated in class
    if (isKeyHeld(GLFW_KEY_LEFT_SHIFT)) {
        //speed up camera if pressed
        camera->sprint();
    }
    else {
        //slow down camera if not pressed
        camera->walk();
    }

    // test key for testing new features or functions
    if (isKeyPressed(GLFW_KEY_T)) {
        // scene.SampleHeight(scene.camera.get()->getPosition());
    }
    scene.SampleHeight(scene.camera.get()->getPosition());

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