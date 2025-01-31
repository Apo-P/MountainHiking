#include <mesh.hpp>


class GameEngine;

class PhysicsEngine;

/// @brief An abstract class for other object implementations to inherit from
class Object {
    private:
        /// @brief if object exists (if not should be deleted)
        bool exists;
        
        /// @brief object's model matrix , BaseModelMatrix
        glm::mat4 modelMatrix, baseModelMatrix;

    public:
    
        /// @brief object's mesh
        std::shared_ptr<Mesh> mesh = nullptr;
        
    public:
        /// @brief constructor
        Object();

        /// @brief an update method
        virtual void update(GameEngine& engine){};
        /// @brief a draw method
        virtual void draw(Renderer& renderer){};
        /// @brief Set ModelMatrix
        /// @param newModelMatrix Model Matrix to set
        void setModelMatrix(glm::mat4 newModelMatrix);
        /// @brief Get Complete ModelMatrix
        /// @return modelMatrix * baseModelMatrix
        glm::mat4 getModelMatrix();
        /// @brief Permanently apply a transformation to baseModelMatrix
        /// @param transformation Transformation to Apply
        void applyTransformation(glm::mat4 transformation);

};