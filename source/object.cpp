#include <object.hpp>


/// @brief Constructor sets exists, modelMatrix, baseModelMatrix to starting values once called
Object::Object(): exists(true), modelMatrix(glm::mat4(1)), baseModelMatrix(glm::mat4(1)) {
}

void Object::setModelMatrix(glm::mat4 newModelMatrix)
{
    modelMatrix = newModelMatrix;
}

glm::mat4 Object::getModelMatrix()
{
    return modelMatrix * baseModelMatrix;
}

void Object::applyTransformation(glm::mat4 transformation)
{
    baseModelMatrix = transformation * baseModelMatrix;
    
    // if we have a collider update coords
}