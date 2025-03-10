#ifndef SCENE_HPP
#define SCENE_HPP

#include <common.hpp>

#include <camera.hpp>
#include <object.hpp>

#include <terrainChunk.hpp>
#include <objectPlacer.hpp>
#include "tree.hpp"
#include "light.hpp"
#include "sphere.hpp"

/// @brief a collection of objects and camera in a scene
class Scene {
    public:
        /// @brief active camera of scene
        std::shared_ptr<Camera> camera;
        /// @brief vector of objects in scene //? could be a map for faster access?
        std::vector<std::shared_ptr<Object>> objects;

        std::vector<std::shared_ptr<Tree>> trees;
        std::vector<std::shared_ptr<Object>> vegetation;

        std::vector<std::shared_ptr<DirectionalLight>> dirLights ;
        
        std::vector<std::shared_ptr<PointLight>> pointLights ;

    
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


        TerrainChunk* testChunk;

        void setTestChunk(TerrainChunk& newChunk) {testChunk = &newChunk;};

        void SampleHeight(glm::vec3 pos) const;

        void spawnTrees(std::shared_ptr<TerrainChunk> chunk, glm::vec2 testChunkStartPos, float ChunkSize, float ChunkRes);

        void generateLights();

};

#endif