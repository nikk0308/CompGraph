#ifndef PLANET_H
#define PLANET_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <vector>
#include "Loader.h"
#include "Variables.h"

class Planet {
public:
    Planet(float size, float orbitRadius, float orbitSpeed, float rotationSpeed,
           const std::string &diffuseMapPath,
           const std::string &normalMapPath = "",
           const std::string &specularMapPath = "",
           const std::string &bumpMapPath = "",
           const std::string &cloudMapPath = "",
           const glm::vec4 &orbitColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), float orbitThickness = 10.0f);

    void drawOrbit(unsigned int shaderProgram, const glm::mat4 &view, const glm::mat4 &projection);
    void setParent(Planet *parentPlanet);
    void update(float deltaTime);
    void draw(unsigned int shaderProgram, const glm::mat4 &view, const glm::mat4 &projection);

    glm::vec3 getPosition() const {
        return position;
    }

private:
    float size;
    float orbitRadius;
    float orbitSpeed;
    float rotationSpeed;

    glm::vec3 position = glm::vec3(0.0f);
    float orbitAngle = 0.0f;
    float rotationAngle = 0.0f;

    unsigned int diffuseMap = 0, normalMap = 0, specularMap = 0, bumpMap = 0, cloudMap = 0;
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    unsigned int indexCount = 0;

    Planet *parentPlanet = nullptr;

    glm::vec4 orbitColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float orbitThickness = 0.01f;
    unsigned int orbitVAO = 0, orbitVBO = 0;
    int orbitSegments = 100;

    void generateOrbit();
    void generateSphere();
};

#endif