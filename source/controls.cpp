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
    double mouseX=0, mouseY=0;

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
    else if (glfwGetKey(window.get(), GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE){
        //slow down camera if not pressed
        camera->walk();
    }

    // test key for testing new features or functions
    if (isKeyPressed(GLFW_KEY_T)) {
        scene.SampleHeight(scene.camera.get()->getPosition());
    }
    // scene.SampleHeight(scene.camera.get()->getPosition());

    // another test key for testing new features or functions
    if (isKeyPressed(GLFW_KEY_U)) {
        //Todo make this a function
        // test live changing
        if (scene.testChunk){
            std::cout << "pressed U" <<std::endl;
            printModifiableValues();
        }
    }


    // change modifiable value
    if (isKeyPressed(GLFW_KEY_LEFT_BRACKET)) {
        changeModifyIndex(currentModifiableIndex, -1);
    }
    if (isKeyPressed(GLFW_KEY_RIGHT_BRACKET)) {
        changeModifyIndex(currentModifiableIndex, 1);
    }

    if (isKeyPressed(GLFW_KEY_MINUS)) {
        // subtract value
        modifyTerrainValues(scene.testChunk->getHeightGenerator(), currentModifiableIndex, false);
        // recalculate height
        scene.testChunk->recalculateHeight();
    }
    else if (isKeyPressed(GLFW_KEY_EQUAL)) {
        // add value
        modifyTerrainValues(scene.testChunk->getHeightGenerator(), currentModifiableIndex, true);
        // recalculate height
        scene.testChunk->recalculateHeight();
    }
    

}

void Controls::changeModifyIndex(int &index, int amount){
    index += amount;

    int  min=modifiableValues::MinEnum;
    int  max=modifiableValues::MaxEnum;

    // Check if value is outside allowed limits
    index = std::max(index, min); // get MinEnum if current is below min
    index = std::min(index, max); // get MaxEnum if current is above max

    // say what we changed to
    std::cout << "Now modifying:" << modifiableValues::valueNames[index] << std::endl;
}

// get heightgenerator to modify
void Controls::modifyTerrainValues(HeightGenerator &heightGenerator, int valueIndex, bool addAmount){
 
    // get amount to modify by (amount is preset)
    float amount=0;

    // if we are modifying hill
    if(valueIndex <=modifiableValues::hillHeight) {
        amount = 10;
    }
    // if we are modifying simplex
    else if(valueIndex <=modifiableValues::smpExponentiation) {
        if(valueIndex == modifiableValues::smpNoiseScale) {
            amount = 10;
        }
        else if(valueIndex == modifiableValues::smpPersistence) {
            amount = 0.1;
        }
        else {
            amount = 1;
        }
    }

    // check if we are adding amount
    float newValue=0;
    if(addAmount){
        newValue = values[valueIndex] + amount;
        std::cout << "added:" << amount;
    }
    else{
        newValue = values[valueIndex] - amount;
        std::cout << "subtracted:" << amount;
    }

    // check if value is within boundaries

    //if yes 
    std::cout << " new value is:" << newValue << std::endl;
    values[valueIndex] = newValue;

    // create new heightmap
    //? or call correct update



    // if we are modifying hill
    if(valueIndex <=modifiableValues::hillHeight){
        heightGenerator.updateHill(glm::vec2(values[modifiableValues::hillPosX], values[modifiableValues::hillPosY]), values[modifiableValues::hillRadius], values[modifiableValues::hillHeight]);
    }
    else if (valueIndex <=modifiableValues::smpExponentiation){
        heightGenerator.updateSimplex(values[modifiableValues::smpNoiseScale], values[modifiableValues::smpPersistence], values[modifiableValues::smpOctaves], values[modifiableValues::smpExponentiation]);
    }

}

void Controls::printModifiableValues(){
    std::cout << "printing modifiable values" << std::endl;
    for( int valueIndex = modifiableValues::MinEnum; valueIndex <= modifiableValues::MaxEnum; valueIndex++){

        std::cout << modifiableValues::valueNames[valueIndex] << ":" << values[valueIndex] << std::endl;

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
        //commented to be sure because I saw weird behaviour
        // handleMouse(scene, deltaTime);
        // handleKeyboard(scene, deltaTime);
}