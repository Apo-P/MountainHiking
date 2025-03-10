#ifndef MODEL_HPP
#define MODEL_HPP

#include "common.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "material.hpp"

#include "assetManager.hpp"


class GameEngine;

class PhysicsEngine;

/// @brief An abstract class for other object implementations to inherit from
class Model {
    protected:
        /// @brief if object exists (if not should be deleted)
        bool exists;
        
        /// @brief object's model matrix , BaseModelMatrix
        /// Model matrix can be used for temporary animations 
        /// Base model matrix is object's position model matrix
        glm::mat4 modelMatrix, baseModelMatrix;

    public:
    
        /// @brief object's mesh
        std::shared_ptr<Mesh> mesh = nullptr;

        // object's texture, inherited models could have more than one
        std::shared_ptr<Texture> texture = nullptr;

        // object's material, inherited models could have more than one
        std::shared_ptr<Material> material = nullptr;

    protected:
        //! bind and draw are virtual for now but i dont thinks they need to be
        
        // Bind and draw should be only be accessed by renderer (friend class)

        /// @brief 
        virtual void bind();

        /// @brief a draw method draws the mesh
        virtual void draw(Renderer& renderer);
        
    public:
        /// @brief constructor
        //? could also add constructor that takes input (example base model matrix)
        Model();

        /// @brief an update method
        virtual void update(GameEngine& engine){};

        /// @brief Set ModelMatrix
        /// @param newModelMatrix Model Matrix to set
        void setModelMatrix(glm::mat4 newModelMatrix);
        /// @brief Get Complete ModelMatrix
        /// @return modelMatrix * baseModelMatrix
        glm::mat4 getModelMatrix();

        /// @brief Permanently apply a transformation to baseModelMatrix
        /// @param transformation Transformation to Apply
        void applyTransformation(glm::mat4 transformation);


        // Renderer is a friend class and should be able to access bind and draw
        friend Renderer; 

};

#endif