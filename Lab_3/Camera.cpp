#include "Camera.h"

#include <iostream>

void Camera::initializeVariables(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float screenWidth,
                                 float screenHeight){
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    Yaw = yaw;
    Pitch = pitch;
    lastX = screenWidth / 2.0f;
    lastY = screenHeight / 2.0f;
    Position = position;
    WorldUp = up;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return lookAt(Position, Position + Front, Up);
}

void Camera::processKeyboard(GLFWwindow *window, float deltaTime) {
    float velocity = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        velocity *= 5;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += Front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= Front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position -= Right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += Right * velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        Position -= WorldUp * velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        Position += WorldUp * velocity;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        if (!oKeyPressed) {
            arePlanetsMoving = !arePlanetsMoving;
            oKeyPressed = true;
        }
    } else {
        oKeyPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!pKeyPressed) {
            arePlanetsRotating = !arePlanetsRotating;
            pKeyPressed = true;
        }
    } else {
        pKeyPressed = false;
    }
}

void Camera::processMouseMovement(GLFWwindow *window, double xpos, double ypos) {
    const float sensitivity = 0.1f;

    static bool isMousePressed = false;

    if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        isMousePressed = false;
        return;
    }

    if (!isMousePressed) {
        lastX = xpos;
        lastY = ypos;
        isMousePressed = true;
        return;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;


    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;


    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = normalize(front);

    Right = normalize(cross(Front, WorldUp));
    Up = normalize(cross(Right, Front));
}
