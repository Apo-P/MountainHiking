#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "common.hpp"

#include "object.hpp"


class Camera : public Object {
    
    protected:

        float nearCP = 0.1f;
        float farCP = 2750.0f;

        float fov = 45.0f;
        float aspectRatio = 4.0f / 3.0f; //16.0f/9.0f;

        // for smooth camera
        glm::vec3 velocity = glm::vec3(0);
        float movementSpeed = 25.0f;

        const float normalSpeed = 25.0f; 
        const float sprintSpeed = 150.0f;

        // if camera wants to sprint
        bool doSprint = false;

        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);

    protected:
        /// @brief update View Matrix
        void updateViewMatrix();
        /// @brief update Projection Matrix
        void updateProjectionMatrix();

    public:
        /// @brief default constructor
        Camera() : Camera(DEFAULT_POSITION, DEFAULT_ORIENTATION) {};
        /// @brief constructor 
        /// @param position camera position
        /// @param orientation camera orientation
        Camera(const glm::vec3 position, const glm::quat orientation);
        /// @brief constructor 
        /// @param position camera position
        /// @param target camera target to look
        /// @param up camera up vector
        Camera(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up);

        /// @brief returns view matrix
        /// @return view matrix
        glm::mat4 getView() const {return viewMatrix;}
        /// @brief returns projection matrix
        /// @return projection matrix
        glm::mat4 getProjection() const {return projectionMatrix;}

        /// @brief sets Near clip
        /// @param value new value to set
        void setNC(const float value) {nearCP = value;}
        /// @brief sets far clip
        /// @param value new value to set
        void setFC(const float value) {farCP = value;}

        /// @brief updates aspect ratio
        /// @param width_height a vec2 containing new window width and height
        void updateAspectRatio(glm::vec2 width_height);

        /// @brief zoom's camera by changing fov
        /// @param amount amount to zoom
        void zoom(const float amount);


        /// @brief sets camera postion
        /// @param position new position
        void setPosition(const glm::vec3 position);
        /// @brief sets camera orientation
        /// @param orientation new orientation
        void setOrientation(const glm::quat orientation);

        /// @brief Moves camera in space
        /// @param translation translation amount
        void translate(const glm::vec3 translation);
        /// @brief changes camera scale
        /// @param scaleFactor scale factor 
        void changeScale(const glm::vec3 scaleFactor);

        /// @brief rotates camera
        /// @param rotation rotation amount
        virtual void rotate(const glm::quat rotation);
        /// @brief rotates cameras
        /// @param angle angle amount
        /// @param axis rotation axis
        void rotate(const float angle, const glm::vec3 axis);

        /// @brief changes where camera looks based on pitch and yaw
        /// @param pitch amount to change
        /// @param yaw amount to change
        void look(const float pitch, const float yaw);

        /// @brief changes where camera looks based on new target
        /// @param target target to look
        /// @param up up direction
        /// @param alternativeUp alternative up direction (required for quat)
        void lookAt(const glm::vec3 target, const glm::vec3 up = glm::vec3(0, 1, 0), const glm::vec3 alternativeUp = glm::vec3(0,0,1));
    
        /// @brief moves camera forward
        void moveForward();
        /// @brief moves camera backward
        void moveBackward();
        /// @brief moves camera right
        void moveRight();
        /// @brief moves camera left
        void moveLeft();
        /// @brief moves camera up
        void moveUp();
        /// @brief moves camera down
        void moveDown();

        /// @brief changes camera speed
        void sprint();
        void walk();
    
        //updates camera
        void update(const float deltaTime);

    };

#endif
