#include <model.hpp>


/// @brief Constructor sets exists, modelMatrix, baseModelMatrix to starting values once called
Model::Model(): exists(true), modelMatrix(glm::mat4(1)), baseModelMatrix(glm::mat4(1)) {
}

void Model::setModelMatrix(glm::mat4 newModelMatrix)
{
    modelMatrix = newModelMatrix;
}

glm::mat4 Model::getModelMatrix()
{
    // be carefull with order
    return modelMatrix * baseModelMatrix;
}

void Model::applyTransformation(glm::mat4 transformation)
{
    baseModelMatrix = transformation * baseModelMatrix;
    
    // if we have a collider update coords
}

void Model::bind() {
    // bind VAO
    mesh->bind();

    // bind texture
}

void Model::draw(Renderer& renderer) {
    // draw mesh
    mesh->draw(renderer);
}
