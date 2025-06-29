/**
 * Asteroid.cpp
 *
 * Implementação da classe Asteroid
 * Inclui LOD, iluminação e diferentes tipos de asteroides
 */

#include "Asteroid.h"
#include <cstdlib>

Asteroid::Asteroid()
    : position(0, 0, 0), rotation(0, 0, 0), rotationSpeed(0, 0, 0), scale(1.0f), type(SPHERE), detailLevel(2)
{
    // Configurar distâncias LOD (próximo, médio, longe)
    lodDistances[0] = 50.0f;  // Alta qualidade
    lodDistances[1] = 100.0f; // Média qualidade
    lodDistances[2] = 200.0f; // Baixa qualidade

    // Cor padrão cinza rochoso
    color[0] = 0.5f;
    color[1] = 0.5f;
    color[2] = 0.5f;

    randomizeProperties();
}

Asteroid::Asteroid(const Vector3 &pos, float size, AsteroidType asteroidType)
    : position(pos), rotation(0, 0, 0), scale(size), type(asteroidType), detailLevel(2)
{

    lodDistances[0] = 50.0f;
    lodDistances[1] = 100.0f;
    lodDistances[2] = 200.0f;

    color[0] = 0.5f;
    color[1] = 0.5f;
    color[2] = 0.5f;

    randomizeProperties();
}

void Asteroid::setColor(float r, float g, float b)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

void Asteroid::update(float deltaTime)
{
    rotation.x += rotationSpeed.x * deltaTime;
    rotation.y += rotationSpeed.y * deltaTime;
    rotation.z += rotationSpeed.z * deltaTime;

    // Manter ângulos no range [0, 360)
    if (rotation.x >= 360.0f)
        rotation.x -= 360.0f;
    if (rotation.y >= 360.0f)
        rotation.y -= 360.0f;
    if (rotation.z >= 360.0f)
        rotation.z -= 360.0f;
}

void Asteroid::render(const Vector3 &cameraPos, bool wireframe)
{
    if (!isVisible(cameraPos))
        return;
    updateLOD(cameraPos);

    glPushMatrix();

    // Aplicar transformações
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, scale);

    // Aplicar cor
    glColor3f(color[0], color[1], color[2]);

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);
    }

    switch (type) {
    case SPHERE:
        renderSphere(detailLevel);
        break;
    case CUBE:
        renderCube();
        break;
    case TORUS:
        renderTorus(detailLevel);
        break;
    case TEAPOT:
        renderTeapot(!wireframe);
        break;
    case ICOSAHEDRON:
        renderIcosahedron();
        break;
    }

    glPopMatrix();
}

void Asteroid::renderSphere(int detail)
{
    GLUquadric *quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    int slices, stacks;
    switch (detail) {
    case 0:
        slices = 8;
        stacks = 6;
        break; // Baixo
    case 1:
        slices = 16;
        stacks = 12;
        break; // Médio
    case 2:
        slices = 32;
        stacks = 24;
        break; // Alto
    default:
        slices = 16;
        stacks = 12;
        break;
    }

    gluSphere(quad, 1.0, slices, stacks);
    gluDeleteQuadric(quad);
}

void Asteroid::renderCube()
{
    // Cubo com normais para iluminação
    glBegin(GL_QUADS);

    // Face frontal
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Face traseira
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Face superior
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Face inferior
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Face direita
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Face esquerda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glEnd();
}

void Asteroid::renderTorus(int detail)
{
    int sides, rings;
    switch (detail) {
    case 0:
        sides = 8;
        rings = 12;
        break; // Baixo
    case 1:
        sides = 16;
        rings = 24;
        break; // Médio
    case 2:
        sides = 32;
        rings = 48;
        break; // Alto
    default:
        sides = 16;
        rings = 24;
        break;
    }

    glutSolidTorus(0.3, 1.0, sides, rings);
}

void Asteroid::renderTeapot(bool solid)
{
    if (solid) {
        glutSolidTeapot(1.0);
    } else {
        glutWireTeapot(1.0);
    }
}

void Asteroid::renderIcosahedron() { glutSolidIcosahedron(); }

void Asteroid::updateLOD(const Vector3 &cameraPos)
{
    float distance = distanceToCamera(cameraPos);
    detailLevel = calculateDetailLevel(distance);
}

int Asteroid::calculateDetailLevel(float distance) const
{
    if (distance < lodDistances[0]) {
        return 2; // Alto detalhe
    } else if (distance < lodDistances[1]) {
        return 1; // Médio detalhe
    } else {
        return 0; // Baixo detalhe
    }
}

float Asteroid::distanceToCamera(const Vector3 &cameraPos) const
{
    Vector3 diff = position - cameraPos;
    return diff.length();
}

bool Asteroid::isVisible(const Vector3 &cameraPos, float maxDistance) const
{
    return distanceToCamera(cameraPos) <= maxDistance;
}

AsteroidType Asteroid::getRandomType()
{
    int typeCount = 5; // Número de tipos disponíveis
    return static_cast<AsteroidType>(rand() % typeCount);
}

void Asteroid::randomizeProperties()
{
    // Randomizar velocidade de rotação
    rotationSpeed.x = ((float)rand() / RAND_MAX) * 60.0f - 30.0f; // -30 a +30 graus/seg
    rotationSpeed.y = ((float)rand() / RAND_MAX) * 60.0f - 30.0f;
    rotationSpeed.z = ((float)rand() / RAND_MAX) * 60.0f - 30.0f;

    // Randomizar cor (tons rochosos)
    float baseColor = 0.3f + ((float)rand() / RAND_MAX) * 0.4f; // 0.3 a 0.7
    float variation = 0.1f;

    color[0] = baseColor + ((float)rand() / RAND_MAX) * variation - variation / 2;
    color[1] = baseColor + ((float)rand() / RAND_MAX) * variation - variation / 2;
    color[2] = baseColor + ((float)rand() / RAND_MAX) * variation - variation / 2;

    // Garantir que cores fiquem no range [0,1]
    for (int i = 0; i < 3; i++) {
        if (color[i] < 0.0f)
            color[i] = 0.0f;
        if (color[i] > 1.0f)
            color[i] = 1.0f;
    }
}
