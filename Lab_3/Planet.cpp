#include "Planet.h"

Planet::Planet(float size, float orbitRadius, float orbitSpeed, float rotationSpeed,
               const std::string &diffuseMapPath,
               const std::string &normalMapPath,
               const std::string &specularMapPath,
               const std::string &bumpMapPath,
               const std::string &cloudMapPath,
               const glm::vec4 &orbitColor, float orbitThickness)
    : size(size), orbitRadius(orbitRadius), orbitSpeed(orbitSpeed), rotationSpeed(rotationSpeed),
      orbitColor(orbitColor), orbitThickness(orbitThickness) {
    if (orbitRadius > 0.0f) {
        generateOrbit();
    }

    generateSphere();

    Loader::loadTexture(diffuseMapPath.c_str(), diffuseMap);
    if (!normalMapPath.empty()) Loader::loadTexture(normalMapPath.c_str(), normalMap);
    if (!specularMapPath.empty()) Loader::loadTexture(specularMapPath.c_str(), specularMap);
    if (!bumpMapPath.empty()) Loader::loadTexture(bumpMapPath.c_str(), bumpMap);
    if (!cloudMapPath.empty()) Loader::loadTexture(cloudMapPath.c_str(), cloudMap);
}

void Planet::drawOrbit(unsigned int shaderProgram, const glm::mat4 &view, const glm::mat4 &projection) {
    if (orbitRadius <= 0.0f) {
        return;
    }

    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 center = parentPlanet ? parentPlanet->getPosition() : glm::vec3(0.0f);
    model = glm::translate(model, center);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3fv(glGetUniformLocation(shaderProgram, "orbitColor"), 1, glm::value_ptr(orbitColor));

    glBindVertexArray(orbitVAO);
    glLineWidth(orbitThickness);
    glDrawArrays(GL_LINE_LOOP, 0, orbitSegments);
    glBindVertexArray(0);
}

void Planet::setParent(Planet *parentPlanet) {
    this->parentPlanet = parentPlanet;
}

void Planet::update(float deltaTime) {
    if (arePlanetsMoving) {
        orbitAngle += orbitSpeed * deltaTime;
    }

    if (arePlanetsRotating) {
        rotationAngle += rotationSpeed * deltaTime;
    }

    if (parentPlanet) {
        glm::vec3 parentPosition = parentPlanet->getPosition();
        position = parentPosition + glm::vec3(
                       orbitRadius * cos(orbitAngle),
                       0.0f,
                       orbitRadius * sin(orbitAngle)
                   );
    } else {
        position = glm::vec3(
            orbitRadius * cos(orbitAngle),
            0.0f,
            orbitRadius * sin(orbitAngle)
        );
    }
}

void Planet::draw(unsigned int shaderProgram, const glm::mat4 &view, const glm::mat4 &projection) {
    glUseProgram(shaderProgram);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(size));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glUniform1i(glGetUniformLocation(shaderProgram, "diffuseMap"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMap);
    glUniform1i(glGetUniformLocation(shaderProgram, "normalMap"), 1);


    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glUniform1i(glGetUniformLocation(shaderProgram, "specularMap"), 2);


    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, bumpMap);
    glUniform1i(glGetUniformLocation(shaderProgram, "bumpMap"), 3);


    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, cloudMap);
    glUniform1i(glGetUniformLocation(shaderProgram, "cloudMap"), 4);


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Planet::generateOrbit() {
    std::vector<float> orbitVertices;
    for (int i = 0; i <= orbitSegments; ++i) {
        float angle = (float) i / orbitSegments * 2.0f * glm::pi<float>();
        float x = orbitRadius * cos(angle);
        float z = orbitRadius * sin(angle);
        orbitVertices.push_back(x);
        orbitVertices.push_back(0.0f);
        orbitVertices.push_back(z);
    }

    glGenVertexArrays(1, &orbitVAO);
    glGenBuffers(1, &orbitVBO);
    glBindVertexArray(orbitVAO);

    glBindBuffer(GL_ARRAY_BUFFER, orbitVBO);
    glBufferData(GL_ARRAY_BUFFER, orbitVertices.size() * sizeof(float), orbitVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Planet::generateSphere() {
    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359f;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
            float xSegment = (float) x / X_SEGMENTS;
            float ySegment = (float) y / Y_SEGMENTS;

            float xPos = cos(xSegment * 2.0f * PI) * sin(ySegment * PI);
            float yPos = cos(ySegment * PI);
            float zPos = sin(xSegment * 2.0f * PI) * sin(ySegment * PI);

            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);

            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);

            vertices.push_back(xSegment);
            vertices.push_back(ySegment);
        }
    }

    for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
        for (unsigned int x = 0; x < X_SEGMENTS; ++x) {
            unsigned int i1 = y * (X_SEGMENTS + 1) + x;
            unsigned int i2 = (y + 1) * (X_SEGMENTS + 1) + x;
            unsigned int i3 = (y + 1) * (X_SEGMENTS + 1) + (x + 1);
            unsigned int i4 = y * (X_SEGMENTS + 1) + (x + 1);

            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);

            indices.push_back(i1);
            indices.push_back(i3);
            indices.push_back(i4);
        }
    }

    indexCount = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
