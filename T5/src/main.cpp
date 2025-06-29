#include <GL/glut.h>
#include <cstdlib>
#include <ctime>

#include "Camera.h"
#include "HUD.h"
#include "Scene.h"
#include "Vector3.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const float TARGET_FPS = 60.0f;
const float FRAME_TIME = 1000.0f / TARGET_FPS;

bool wireframeMode = false;
bool keys[256] = {false};
float lastTime = 0;
float deltaTime = 0;

Camera *camera = nullptr;
Scene *scene = nullptr;
HUD *hud = nullptr;

// Controle de FPS
float lastFrameTime = 0;
int frameCount = 0;
float fps = 0;

void calculateFPS()
{
    frameCount++;
    float currentTime = glutGet(GLUT_ELAPSED_TIME);

    if (currentTime - lastFrameTime >= 1000.0f) {
        fps = frameCount * 1000.0f / (currentTime - lastFrameTime);
        frameCount = 0;
        lastFrameTime = currentTime;

        // Atualizar FPS no HUD
        if (hud) {
            hud->setFPS(fps);
        }
    }
}

void setupLighting()
{
    // Habilita iluminação
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Configuração da luz ambiente
    GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    // Configuração da luz direcional (sol distante)
    GLfloat lightPos[] = {1.0f, 1.0f, 1.0f, 0.0f}; // Luz direcional
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

void init()
{
    // Configurações básicas do OpenGL
    glClearColor(0.05f, 0.05f, 0.15f, 1.0f); // Cor de fundo espacial
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Configuração da perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);

    setupLighting();

    camera = new Camera(Vector3(0, 0, 10), Vector3(0, 0, -1), Vector3(0, 1, 0));
    scene = new Scene();
    scene->generateAsteroids(100, 200.0f); // 50 asteroides em área de 200x200x200

    // Inicializar HUD
    hud = new HUD(SCREEN_WIDTH, SCREEN_HEIGHT);
    hud->setTextColor(0.0f, 1.0f, 0.0f);             // Verde para texto
    hud->setCrosshairColor(1.0f, 0.0f, 0.0f);        // Vermelho para mira
    hud->setBackgroundColor(0.0f, 0.0f, 0.0f, 0.6f); // Fundo semi-transparente

    // Configura modo de polígono inicial
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    srand(time(nullptr));
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Aplicar transformação da câmera
    camera->applyTransform();

    // Renderizar cena
    scene->render(camera->getPosition(), wireframeMode);

    // Renderizar HUD por último
    if (hud) {
        hud->render(camera->getPosition(), wireframeMode, scene->getAsteroidCount(), fps);
    }

    calculateFPS();

    glutSwapBuffers();
}

void update()
{
    float currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    // Limitar deltaTime para evitar saltos grandes
    if (deltaTime > 0.1f)
        deltaTime = 0.1f;

    // Processar input contínuo
    Vector3 movement(0, 0, 0);
    Vector3 rotation(0, 0, 0);

    float moveSpeed = 15.0f * deltaTime;
    float rotSpeed = 90.0f * deltaTime; // graus por segundo

    // Movimento da câmera
    if (keys['w'] || keys['W'])
        movement.z += moveSpeed; // Frente
    if (keys['s'] || keys['S'])
        movement.z -= moveSpeed; // Trás
    if (keys['a'] || keys['A'])
        movement.x -= moveSpeed; // Esquerda
    if (keys['d'] || keys['D'])
        movement.x += moveSpeed; // Direita
    if (keys['q'] || keys['Q'])
        movement.y += moveSpeed; // Cima
    if (keys['e'] || keys['E'])
        movement.y -= moveSpeed; // Baixo

    // Rotação da câmera
    if (keys['i'] || keys['I'])
        rotation.x += rotSpeed; // Pitch up
    if (keys['k'] || keys['K'])
        rotation.x -= rotSpeed; // Pitch down
    if (keys['j'] || keys['J'])
        rotation.y += rotSpeed; // Yaw left
    if (keys['l'] || keys['L'])
        rotation.y -= rotSpeed; // Yaw right
    if (keys['u'] || keys['U'])
        rotation.z -= rotSpeed; // Roll left
    if (keys['o'] || keys['O'])
        rotation.z += rotSpeed; // Roll right

    // Aplicar movimentos
    camera->move(movement);
    camera->rotate(rotation);

    // Atualizar asteroides (rotação)
    scene->update(deltaTime);

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    keys[key] = true;

    switch (key) {
    case 27: // ESC
        delete camera;
        delete scene;
        delete hud; // Limpar HUD
        exit(0);
        break;

    case 'f':
    case 'F':
        wireframeMode = !wireframeMode;
        if (wireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        break;

    case 'h':
    case 'H':
        // Toggle HUD
        if (hud) {
            hud->toggle();
        }
        break;

    case 'r':
    case 'R':
        // Reset da câmera
        camera->reset(Vector3(0, 0, 10), Vector3(0, 0, -1), Vector3(0, 1, 0));
        break;
    }
}

void keyboardUp(unsigned char key, int x, int y) { keys[key] = false; }

void specialKeys(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP:
        keys['w'] = true;
        break;
    case GLUT_KEY_DOWN:
        keys['s'] = true;
        break;
    case GLUT_KEY_LEFT:
        keys['a'] = true;
        break;
    case GLUT_KEY_RIGHT:
        keys['d'] = true;
        break;
    }
}

void specialKeysUp(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP:
        keys['w'] = false;
        break;
    case GLUT_KEY_DOWN:
        keys['s'] = false;
        break;
    case GLUT_KEY_LEFT:
        keys['a'] = false;
        break;
    case GLUT_KEY_RIGHT:
        keys['d'] = false;
        break;
    }
}

void idle()
{
    // Controle de FPS
    static float lastIdleTime = 0;
    float currentTime = glutGet(GLUT_ELAPSED_TIME);

    if (currentTime - lastIdleTime >= FRAME_TIME) {
        update();
        lastIdleTime = currentTime;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Guerra nas Estrelas - T5");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
