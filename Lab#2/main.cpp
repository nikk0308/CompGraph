#include <GL/glut.h>
#include <cmath>
#include <vector>

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const float MOVE_SPEED = 0.08f;

const float INNER_THORUS_R = 0.5;
const float OUTER_THORUS_R = 1;
const float CUBE_SIDE_SIZE = 2;
const float FIGURES_DISTANCE = 3;

const int GRID_SIZE = 50;
const float GRID_STEP = 0.2f;

float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraDistance = 15.0f;
bool isPerspective = true;

vector planeColor = {
    128,
    255,
    0,
};

vector thorusColor = {
    4,
    71,
    28,
};

vector cubeColor = {
    21,
    176,
    151,
};

float normRGB(const int RGBvalue) {
    return RGBvalue / 255.0f;
}

void drawPlane() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    vector<float> vertices;
    vector<float> colors;
    vector<int> indices;

    for (int x = 0; x <= GRID_SIZE; ++x) {
        for (int y = 0; y <= GRID_SIZE; ++y) {
            float posX = (x - GRID_SIZE / 2.0f) * GRID_STEP;
            float posY = (y - GRID_SIZE / 2.0f) * GRID_STEP;
            float posZ = sin(posX) * cos(posY);

            vertices.push_back(posX);
            vertices.push_back(posZ);
            vertices.push_back(posY);

            float t = (posZ + (GRID_SIZE / 2.0f) * GRID_STEP) / (GRID_SIZE * GRID_STEP);
            colors.push_back(normRGB(planeColor[0]) * t);
            colors.push_back(normRGB(planeColor[1]) * t);
            colors.push_back(normRGB(planeColor[2]) * t);
        }
    }

    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = 0; y < GRID_SIZE; ++y) {
            int topLeft = x * (GRID_SIZE + 1) + y;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + GRID_SIZE + 1;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);

            indices.push_back(topLeft);
            indices.push_back(bottomRight);
            indices.push_back(topRight);
        }
    }

    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glColorPointer(3, GL_FLOAT, 0, colors.data());
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}


void drawSolidCube() {
    glColor3f(normRGB(cubeColor[0]), normRGB(cubeColor[1]), normRGB(cubeColor[2]));
    glutSolidCube(CUBE_SIDE_SIZE);
}

void drawWireTorus() {
    glColor3f(normRGB(thorusColor[0]), normRGB(thorusColor[1]), normRGB(thorusColor[2]));
    glutWireTorus(INNER_THORUS_R, OUTER_THORUS_R, 20, 20);
}

void setCamera() {
    float cameraX = cameraDistance * sin(cameraAngleX) * cos(cameraAngleY);
    float cameraY = cameraDistance * sin(cameraAngleY);
    float cameraZ = cameraDistance * cos(cameraAngleX) * cos(cameraAngleY);

    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void changeProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float sceneSize = FIGURES_DISTANCE + max(CUBE_SIDE_SIZE, OUTER_THORUS_R * 2);
    if (isPerspective) {
        gluPerspective(45.0, 1.0, 0.1, sceneSize * 4);
    } else {
        glOrtho(-sceneSize, sceneSize, -sceneSize, sceneSize, -sceneSize * 2, sceneSize * 4);
    }

    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    setCamera();

    glPushMatrix();
    glTranslatef(0, -FIGURES_DISTANCE, 0);
    drawSolidCube();
    glPopMatrix();

    glPushMatrix();
    drawPlane();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, FIGURES_DISTANCE, 0);
    drawWireTorus();
    glPopMatrix();

    glutSwapBuffers();
}

void keyboard(const unsigned char key, int x, int y) {
    if (key == 'p' || key == 'P') {
        isPerspective = true;
        changeProjection();
    } else if (key == 'o' || key == 'O') {
        isPerspective = false;
        changeProjection();
    }

    if (key == 'a' || key == 'A')
        cameraAngleX -= MOVE_SPEED;
    if (key == 'w' || key == 'W')
        cameraAngleY = min(cameraAngleY + MOVE_SPEED, 1.5f);
    if (key == 'd' || key == 'D')
        cameraAngleX += MOVE_SPEED;
    if (key == 's' || key == 'S')
        cameraAngleY = max(cameraAngleY - MOVE_SPEED, -1.5f);
    glutPostRedisplay();
}

void specialKeys(const int key, int x, int y) {
    if (key == GLUT_KEY_LEFT)
        cameraAngleX -= MOVE_SPEED;
    if (key == GLUT_KEY_RIGHT)
        cameraAngleX += MOVE_SPEED;
    if (key == GLUT_KEY_UP)
        cameraAngleY = min(cameraAngleY + MOVE_SPEED, 1.5f);
    if (key == GLUT_KEY_DOWN)
        cameraAngleY = max(cameraAngleY - MOVE_SPEED, -1.5f);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Have a nice day!");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    changeProjection();
    glutMainLoop();

    return 0;
}