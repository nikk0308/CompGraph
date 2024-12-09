#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Variables.h"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float lastX;
    float lastY;

    void initializeVariables(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float screenWidth, float screenHeight);

    glm::mat4 getViewMatrix();

    void processKeyboard(GLFWwindow* window, float deltaTime);
    void processMouseMovement(GLFWwindow* window, double xpos, double ypos);

private:
    bool oKeyPressed = false;
    bool pKeyPressed = false;

    void updateCameraVectors();
};

#endif
