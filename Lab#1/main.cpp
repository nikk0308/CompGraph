#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const float MOVE_SPEED = 2.0f;
const float CELL_SIZE = 0.03f;

float lastFrameTime = 0.0f;
float figureX = 0.0f;
float figureY = 0.0f;

vector<vector<vector<int> > > figure = {
    {
        {-9, 13},
        {-3, 13},
        {-3, 7},
        {-9, 7},
    },
    {
        {1, 17},
        {9, 17},
        {5, 13},
        {-3, 13},
    },
    {
        {-3, 7},
        {9, -5},
        {-3, -5},
    },
    {
        {9, -5},
        {-3, -5},
        {9, -17},
    },
    {
        {-3, -1},
        {-3, -9},
        {-7, -5},
    },
    {
        {3, -11},
        {9, -17},
        {-3, -17},
    },
    {
        {-3, -11},
        {3, -11},
        {-3, -17},
    },
};

vector figureBounds = {
    -9,
    17,
    9,
    -17,
};

vector outlineColor = {
    0,
    0,
    0,
};

vector figureColor = {
    0,
    153,
    251,
};

vector backColor = {
    255,
    242,
    60,
};

float calculateDeltaTime() {
    float currentFrameTime = glfwGetTime();
    float deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
    return deltaTime;
}

float normRGB(const int RGBvalue) {
    return RGBvalue / 255.0f;
}

void drawPolygon(const vector<vector<int> > &pointsArray, const float xOffset, const float yOffset) {
    glColor3f(normRGB(figureColor[0]), normRGB(figureColor[1]), normRGB(figureColor[2]));
    glBegin(GL_POLYGON);
    for (int i = 0; i < pointsArray.size(); i++) {
        glVertex2f(xOffset + pointsArray[i][0] * CELL_SIZE,
                   yOffset + pointsArray[i][1] * CELL_SIZE);
    }
    glEnd();

    glColor3f(normRGB(outlineColor[0]), normRGB(outlineColor[1]), normRGB(outlineColor[2]));
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < pointsArray.size(); i++) {
        glVertex2f(xOffset + pointsArray[i][0] * CELL_SIZE,
                   yOffset + pointsArray[i][1] * CELL_SIZE);
    }
    glEnd();
}

void processInput(GLFWwindow *window) {
    float deltaTime = calculateDeltaTime();
    float moveSpeed = deltaTime * MOVE_SPEED;

    if (((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) ||
         (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS))
        && figureX + figureBounds[0] * CELL_SIZE > -1.0f) {
        figureX -= moveSpeed;
    }
    if (((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) ||
         (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
        && figureY + figureBounds[1] * CELL_SIZE < 1.0f) {
        figureY += moveSpeed;
    }
    if (((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) ||
         (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS))
        && figureX + figureBounds[2] * CELL_SIZE < 1.0f) {
        figureX += moveSpeed;
    }
    if (((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) ||
         (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
        && figureY + figureBounds[3] * CELL_SIZE > -1.0f) {
        figureY -= moveSpeed;
    }
}

void drawFigure(const float offsetX, const float offsetY) {
    for (const auto &fig: figure) {
        drawPolygon(fig, offsetX, offsetY);
    }
}

int main() {
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Have a nice day!", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glClearColor(normRGB(backColor[0]), normRGB(backColor[1]), normRGB(backColor[2]), 1.0f);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        drawFigure(figureX, figureY);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
