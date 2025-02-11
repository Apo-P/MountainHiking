#include "common.hpp"
#include "model.hpp"
#include "texture.hpp"

/// @brief test cube object
class Cube:public Model {

    public:
        std::shared_ptr<Texture> texture;

    public:
        /// @brief constructor
        /// ! todo remember to add texture here later
        Cube(GameEngine& eng, std::string modelMesh);

        /// @brief an update method
        virtual void update(GameEngine& engine) override;
};