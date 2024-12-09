#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "Skybox.h"
#include "Camera.h"
#include "Planet.h"
#include "Variables.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Skybox skybox;
Camera camera;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

float normRGB(int RGBvalue);

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "UltraSuperMega Solar System 3000", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
        camera.processMouseMovement(window, xpos, ypos);
    });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    unsigned int sphereShader = Loader::loadShader("shaders/planet/planet.vert", "shaders/planet/planet.frag");
    unsigned int orbitShader = Loader::loadShader("shaders/orbit/orbit.vert", "shaders/orbit/orbit.frag");
    glLinkProgram(sphereShader);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    skybox.initializeVars();
    camera.initializeVariables(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -80.0f, mode->width,
                               mode->height);

    Planet sun(4.0f, 0.0f, 0.0f, 0.1f,
               "resources/textures/sun/8k_sun.jpg");


    Planet mercury(0.6f, 7.0f, 0.3f, 0.4f,
                   "resources/textures/mercury/surface-diff.jpg",
                   "resources/textures/mercury/surface-norm.jpg",
                   "",
                   "resources/textures/mercury/surface-bump.jpg",
                   "",
                   glm::vec4(normRGB(255), normRGB(204), normRGB(204), 1),
                   5);

    Planet venus(0.8f, 10.0f, 0.4f, 0.3f,
                 "resources/textures/venus/surface-diff.jpg",
                 "resources/textures/venus/surface-norm.jpg",
                 "",
                 "resources/textures/venus/surface-bump.jpg",
                 "resources/textures/venus/clouds0-diff.jpg",
                 glm::vec4(normRGB(255), normRGB(229), normRGB(204), 1),
                 5);

    Planet earth(1.0f, 14.0f, 0.5f, 0.4f,
                 "resources/textures/earth/surface-diff.jpg",
                 "resources/textures/earth/surface-norm.jpg",
                 "resources/textures/earth/surface-spec.jpg",
                 "resources/textures/earth/surface-bump.jpg",
                 "resources/textures/earth/clouds0-diff.jpg",
                 glm::vec4(normRGB(255), normRGB(255), normRGB(204), 1),
                 5);

    Planet moon(0.27f, 2.0f, 0.2f, 0.2f,
                "resources/textures/moon/surface-diff.jpg",
                "resources/textures/moon/surface-norm.jpg",
                "resources/textures/moon/surface-spec.jpg",
                "resources/textures/moon/surface-bump.jpg",
                "",
                glm::vec4(normRGB(255), normRGB(255), normRGB(153), 1),
                3);

    Planet mars(0.8f, 18.0f, 0.4f, 0.3f,
                "resources/textures/mars/surface-diff.jpg",
                "resources/textures/mars/surface-norm.jpg",
                "",
                "resources/textures/mars/surface-bump.jpg",
                "resources/textures/mars/clouds0-diff.jpg",
                glm::vec4(normRGB(229), normRGB(255), normRGB(204), 1),
                5);

    Planet jupiter(3.5f, 25.0f, 0.9f, 0.2f,
                   "resources/textures/jupiter/surface-diff.jpg",
                   "resources/textures/jupiter/surface-norm.jpg",
                   "",
                   "resources/textures/jupiter/surface-bump.jpg",
                   "",
                   glm::vec4(normRGB(204), normRGB(255), normRGB(204), 1),
                   5);

    Planet saturn(1.5f, 32.0f, 0.7f, 0.3f,
                  "resources/textures/saturn/surface-diff.jpg",
                  "resources/textures/saturn/surface-norm.jpg",
                  "",
                  "resources/textures/saturn/surface-bump.jpg",
                  "",
                  glm::vec4(normRGB(204), normRGB(255), normRGB(229), 1),
                  5);

    Planet titan(0.4f, 2.5f, 0.1f, 0.3f,
                 "resources/textures/titan/surface-diff.jpg",
                 "resources/textures/titan/surface-norm.jpg",
                 "resources/textures/titan/surface-spec.jpg",
                 "resources/textures/titan/surface-bump.jpg",
                 "resources/textures/titan/clouds0-diff.jpg",
                 glm::vec4(normRGB(153), normRGB(255), normRGB(204), 1),
                 3);

    Planet uranus(1.8f, 38.0f, 0.8f, 0.5f,
                  "resources/textures/uranus/surface-diff.jpg",
                  "resources/textures/uranus/surface-norm.jpg",
                  "",
                  "resources/textures/uranus/surface-bump.jpg",
                  "",
                  glm::vec4(normRGB(204), normRGB(255), normRGB(255), 1),
                  3);

    Planet neptune(1.4f, 44.0f, 0.4f, 0.2f,
                   "resources/textures/neptune/surface-diff.jpg",
                   "resources/textures/neptune/surface-norm.jpg",
                   "",
                   "resources/textures/neptune/surface-bump.jpg",
                   "",
                   glm::vec4(normRGB(204), normRGB(229), normRGB(255), 1),
                   3);

    moon.setParent(&earth);
    titan.setParent(&saturn);

    glm::vec3 sunPosition(0.0f, 0.0f, 0.0f);
    glm::vec3 lightColor(normRGB(255), normRGB(255), normRGB(230));
    glm::vec3 ambientLightColor(normRGB(255), normRGB(255), normRGB(255));

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processKeyboard(window, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) mode->width / (float) mode->height, 0.1f,
                                                500.0f);

        glUseProgram(sphereShader);
        glUniform3fv(glGetUniformLocation(sphereShader, "lightPos"), 1, glm::value_ptr(sunPosition));
        glUniform3fv(glGetUniformLocation(sphereShader, "viewPos"), 1, glm::value_ptr(camera.Position));
        glUniform3fv(glGetUniformLocation(sphereShader, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(sphereShader, "ambientLightColor"), 1, glm::value_ptr(ambientLightColor));
        glUniform1f(glGetUniformLocation(sphereShader, "lightIntensity"), lightIntensity);
        glUniform1f(glGetUniformLocation(sphereShader, "time"), currentFrame);

        sun.update(deltaTime);
        mercury.update(deltaTime);
        venus.update(deltaTime);
        earth.update(deltaTime);
        moon.update(deltaTime);
        mars.update(deltaTime);
        jupiter.update(deltaTime);
        saturn.update(deltaTime);
        titan.update(deltaTime);
        uranus.update(deltaTime);
        neptune.update(deltaTime);

        sun.draw(sphereShader, view, projection);
        mercury.draw(sphereShader, view, projection);
        venus.draw(sphereShader, view, projection);
        earth.draw(sphereShader, view, projection);
        moon.draw(sphereShader, view, projection);
        mars.draw(sphereShader, view, projection);
        jupiter.draw(sphereShader, view, projection);
        saturn.draw(sphereShader, view, projection);
        titan.draw(sphereShader, view, projection);
        uranus.draw(sphereShader, view, projection);
        neptune.draw(sphereShader, view, projection);

        mercury.drawOrbit(orbitShader, view, projection);
        venus.drawOrbit(orbitShader, view, projection);
        earth.drawOrbit(orbitShader, view, projection);
        moon.drawOrbit(orbitShader, view, projection);
        mars.drawOrbit(orbitShader, view, projection);
        jupiter.drawOrbit(orbitShader, view, projection);
        saturn.drawOrbit(orbitShader, view, projection);
        titan.drawOrbit(orbitShader, view, projection);
        uranus.drawOrbit(orbitShader, view, projection);
        neptune.drawOrbit(orbitShader, view, projection);

        skybox.Draw(camera.getViewMatrix(), projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(sphereShader);

    glfwTerminate();
    return 0;
}

float normRGB(const int RGBvalue) {
    return RGBvalue / 255.0f;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}