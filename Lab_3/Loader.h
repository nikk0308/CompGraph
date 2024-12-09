#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <SOIL2/stb_image.h>
#include <SOIL2/SOIL2.h>

class Loader {
public:
    static unsigned int loadCubemap(const std::vector<std::string> &faces);

    static unsigned int loadShader(const char *vertexPath, const char *fragmentPath);

    static void loadTexture(const char *path, unsigned int &texture);
};

#endif
