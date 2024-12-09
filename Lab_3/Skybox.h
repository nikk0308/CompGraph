#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Loader.h"

class Skybox {
public:
    void initializeVars();

    void Draw(const glm::mat4 &view, const glm::mat4 &projection);

private:
    unsigned int cubemapTexture;
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int shaderProgram;

    void setupSkybox();
};

#endif
