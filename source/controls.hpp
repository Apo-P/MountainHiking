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

        // in a struct in order to not be global
        // list of modifiable values
        struct modifiableValues {
            public:
            enum values {
                hillPosX=0,
                hillPosY,
                hillRadius,
                hillHeight,
                smpNoiseScale,
                smpPersistence,
                smpOctaves,
                smpExponentiation
            };

            // min int of enum
            static const int MinEnum = 0;//values::hillPosX;
            // max int of enum
            static const int MaxEnum = 7;//values::smpExponentiation;

            
            //  list of all modifiable value names
            static constexpr const char* valueNames[MaxEnum+1] = {
                "hillPosX",
                "hillPosY",
                "hillRadius",
                "hillHeight",
                "smpNoiseScale",
                "smpPersistence",
                "smpOctaves",
                "smpExponentiation"
            };

        
        };

        // initial values (be carefull) these should be better initialized
        // nice values from testing
        std::vector<float> values{
            250, -250, 600, 168, //hill
            500, 1 , 4, 3 // smp
        };

        // old starting values
        // 250, -250, 500, 128, //hill
        //     200, 0.5, 4, 3 // smp

        int currentModifiableIndex = modifiableValues::smpNoiseScale;

        void printModifiableValues();

        void changeModifyIndex(int &index, int amount);

        void modifyTerrainValues(HeightGenerator &heightGenerator, int valueIndex, bool addAmount);

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