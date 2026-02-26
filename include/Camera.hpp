#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../thirdparty/glm-master/glm/glm.hpp"
#include "../thirdparty/glm-master/glm/gtc/matrix_transform.hpp"
class Camera
{
    public:

        // default contructor
        Camera()
        {
            Eye_ = glm::vec3(0.0f, 0.0f, 0.0f);
            // assume we are looking out into the world space, RECALL: looking outwards is along the negative Z
            ViewDirection_ = glm::vec3(0.0f, 0.0f,-1.0f);
            // assume we start on a perfect plane
            UpVector_ = glm::vec3(0.0f, 1.0f, 0.0f);
        }

        // view matrix we produce and return
        glm::mat4 GetViewMatrix() const;
        void MouseLook(int mouseX, int mouseY);
        // move forward and backward
        void MoveForward(float speed);
        void MoveBackward(float speed);
        void MoveLeft(float speed);
        void MoveRight(float speed);
    private:

        glm::vec3 Eye_;
        glm::vec3 ViewDirection_;
        glm::vec3 UpVector_;
        
        

};


#endif
