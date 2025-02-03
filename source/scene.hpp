#ifndef SCENE_HPP
#define SCENE_HPP

#include <common.hpp>

#include <camera.hpp>
#include <object.hpp>

/// @brief a collection of objects and camera in a scene
struct Scene {
    /// @brief active camera of scene
    std::shared_ptr<Camera> camera;
    /// @brief vector of objects in scene //? could be a map for faster access?
    std::vector<std::shared_ptr<Object>> objects;


public:
    Scene() = delete;
    /// @brief constructor that takes the arguments
    /// @param camera camera of scene
    Scene(
        const std::shared_ptr<Camera> camera
    ):
        camera(camera) //set camera 
        // objects(objects) //set objects
    {};


    /// @brief updates all scene components
    /// @param deltaTime time elapsed since last update
    void update(const float deltaTime);


};

#endif