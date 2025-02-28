#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <common.hpp>

#include <model.hpp>


glm::quat eulerToQuat(const float pitch, const float yaw, const float roll);

class Object {

    protected:
        std::shared_ptr<Model> model;
        
        glm::vec3 position = DEFAULT_POSITION;
        glm::quat orientation = DEFAULT_ORIENTATION;
        glm::vec3 scale = DEFAULT_SCALE;

        // make Renderer a friend so he can access model
        friend Renderer;

    public:

        Object() : Object(DEFAULT_POSITION) {};
        Object(const glm::vec3 position) : Object(position, DEFAULT_ORIENTATION) {};
        Object(const glm::vec3 position, const glm::quat orientation) : Object(position, orientation, DEFAULT_SCALE) {};
        Object(const glm::vec3 position, const glm::quat orientation, const glm::vec3 scale);
        
        // Getters --

        // Direction vectors

        /// @brief get up direction vector 
        glm::vec3 up() const;
        /// @brief get forward direction vector 
        glm::vec3 forward() const;
        /// @brief get right direction vector 
        glm::vec3 right() const;

        // Matrices
        
        /// @brief get translation matrix
        glm::mat4 translationMatrix() const;
        /// @brief get rotation matrix 
        glm::mat4 rotationMatrix() const;
        /// @brief get scalling matrix 
        glm::mat4 scallingMatrix() const;
        /// @brief get model matrix 
        glm::mat4 modelMatrix() const;

        // Get positional vectors
        
        /// @brief get position vector
        glm::vec3 getPosition() const {return position;}
        /// @brief get orientation quaternion 
        glm::quat getOrientation() const {return orientation;}
        /// @brief get scale vector 
        glm::vec3 getScale() const {return scale;}


        // Setters

        //! virtual keyword is need to make sure every object that inherits 
        //! wraps the main object function in what it need

        /// @brief set objects model to a new one
        /// @param newModel new model
        virtual void setModel(std::shared_ptr<Model> newModel) { model = newModel; };

        //Bad function only use carefully. Will remove model from object
        virtual void deleteModel() { model = nullptr; };

        //? should add an update method that updates the model model matrix based on position, orientation
        // Model->void setModelMatrix(glm::mat4 newModelMatrix); set's model matrix , not base model matrix (which is used mainly for scale)

        /// @brief sets object postion
        /// @param position new position
        virtual void setPosition(const glm::vec3 position);
        /// @brief sets object orientation
        /// @param orientation new orientation
        virtual void setOrientation(const glm::quat orientation);
        /// @brief sets objects new scale
        /// @param newScale new scale
        virtual void setScale(const glm::vec3 newScale);

        /// @brief Moves object in space
        /// @param translation translation amount
        virtual void translate(const glm::vec3 translation);
        /// @brief changes object scale
        /// @param scaleFactor scale factor 
        virtual void changeScale(const glm::vec3 scaleFactor);
        /// @brief rotates object
        /// @param rotation rotation amount
        virtual void rotate(const glm::quat rotation);
        /// @brief rotates objects
        /// @param angle angle amount
        /// @param axis rotation axis
        virtual void rotate(const float angle, const glm::vec3 axis);

};

#endif