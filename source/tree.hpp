#ifndef TREE_HPP
#define TREE_HPP

#include "model.hpp"

class Tree : public Model {
    public:

        static const float minRadius; // 3 is about the size of the test tree

        Tree(){
            mesh = std::static_pointer_cast<Mesh>(std::make_shared<Mesh>("resources/models/mySimpleTree.obj"));

            this->baseModelMatrix = glm::scale(glm::mat4(1), glm::vec3(0.5));

            // be carefull if no texture we will have a segmentation fault in texture renderer for now, maybe add check if obj has texture else use something else
            this->texture = AssetManager::LoadTexture("simpleTreeTexturePallet.png", "resources/textures/simpleTreeTexturePallet.png"); //!use file name as key for now 
        }

};

#endif