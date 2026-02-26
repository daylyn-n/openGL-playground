#include "../include/Camera.hpp"

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Eye_, ViewDirection_, UpVector_);
}
void Camera::MouseLook(int mouseX, int mouseY)
{
    
}
// move forward and backward
void Camera::MoveForward(float speed)
{
    // simple but not correct
    Eye_.z -= speed;

}
void Camera::MoveBackward(float speed)
{
    Eye_.z += speed;
}
void Camera::MoveLeft(float speed)
{
    Eye_.x -=speed;
}
void Camera::MoveRight(float speed)
{
    Eye_.x += speed;
}
    
