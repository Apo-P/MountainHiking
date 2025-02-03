#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <common.hpp>
#include <scene.hpp>


class Controls {
    protected:
        /// @brief If we are on WSL
        bool WSL = true;

        /// @brief window to poll for keys and mouse;
        std::shared_ptr<GLFWwindow> window;

        /// @brief if key should be on cooldown (to reduce multiple inputs in short dt)
        float keyCooldown;
        /// @brief Keycooldown time
        const float KEY_COOLDOWN_TIME = 0.5f;

    private:
        /// @brief check if key was pressed
        /// @param key key to check
        /// @return bool 
        bool isKeyPressed(int key);
        /// @brief check if key is held
        /// @param key key to check
        /// @return bool
        bool isKeyHeld(int key);

        /// @brief handle input for wsl
        /// @param deltaTime time elapsed since last call
        void handleWSL(const Scene& scene, const float deltaTime);

        /// @brief handle mouse input
        /// @param deltaTime 
        void handleMouse(const Scene& scene, const float deltaTime);

        /// @brief handle keyboard input
        /// @param deltaTime 
        void handleKeyboard(const Scene& scene, const float deltaTime);


    public:

        Controls(const std::shared_ptr<GLFWwindow> window):
            window(window)
        {std::cout << "Controller class made" << std::endl;};

        void handleInputs(const Scene& scene, const float deltaTime);

};


#endif