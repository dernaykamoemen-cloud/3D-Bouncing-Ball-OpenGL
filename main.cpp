
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Camera state
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 15.0f;
float lookAtX = 0.0f, lookAtY = 0.0f, lookAtZ = 0.0f;
float cameraYaw = 0.0f;
float cameraPitch = 0.0f;
float camRotateSpeed = 2.0f;
float camMoveSpeed = 0.5f;

// Ball state
float ballRadius = 0.5f;
float ballX = 0.0f, ballY = 8.0f, ballZ = 0.0f;
float ballVX = 0.0f, ballVY = 0.0f, ballVZ = 0.0f;
float ballAngle = 0.0f;

// Ground obstacle (cube)
float obstacle1X = 4.0f, obstacle1Y = -9.5f, obstacle1Z = 3.0f;
float obstacle1Width = 2.0f, obstacle1Height = 4.0f, obstacle1Depth = 5.0f;

// Wall obstacle (cube at -9.5, 0, 0)
float obstacle2X = -9.5f, obstacle2Y = 0.0f, obstacle2Z = 0.0f;
float obstacle2Width = 1.0f, obstacle2Height = 10.0f, obstacle2Depth = 10.0f;

// Physics variables
float gravity = -0.002f;
float elasticity = 0.8f;
float friction = 0.999f;
float floorLevel = -10.0f + ballRadius;
float ceilingLevel = 10.0f - ballRadius;
float wallLevel = 10.0f - ballRadius;

// Bounce tracking
int bounceCount = 0;
float lastBounceTime = 0.0f;
const float bounceTimeThreshold = 0.5f;
const int maxBounceCount = 3;

// Time tracking
float currentTime = 0.0f;
float lastFrameTime = 0.0f;

// Mouse control
int lastMouseX, lastMouseY;
bool mouseLeftDown = false;

// Light positions
GLfloat light_position0[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat light_position1[] = { -1.0, 1.0, -1.0, 0.0 };

// Material properties
GLfloat mat_shininess[] = { 50.0 };
bool light0_enabled = true;
bool light1_enabled = true;

void updateCameraView() {
    float yawRad = cameraYaw * 3.14159265f / 180.0f;
    float pitchRad = cameraPitch * 3.14159265f / 180.0f;

    float dirX = cos(pitchRad) * sin(yawRad);
    float dirY = sin(pitchRad);
    float dirZ = -cos(pitchRad) * cos(yawRad);

    lookAtX = cameraX + dirX;
    lookAtY = cameraY + dirY;
    lookAtZ = cameraZ + dirZ;

    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ,
        lookAtX, lookAtY, lookAtZ,
        0.0f, 1.0f, 0.0f);
}

bool checkSphereCubeCollision(float bx, float by, float bz, float br,
    float cx, float cy, float cz,
    float cw, float ch, float cd) {
    float closestX = fmax(cx - cw / 2, fmin(bx, cx + cw / 2));
    float closestY = fmax(cy - ch / 2, fmin(by, cy + ch / 2));
    float closestZ = fmax(cz - cd / 2, fmin(bz, cz + cd / 2));

    float distance = sqrt(pow(bx - closestX, 2) +
        pow(by - closestY, 2) +
        pow(bz - closestZ, 2));

    return distance < br;
}

void handleCollisionWithObstacles() {
    // Check collision with first obstacle (ground cube)
    if (checkSphereCubeCollision(ballX, ballY, ballZ, ballRadius,
        obstacle1X, obstacle1Y, obstacle1Z,
        obstacle1Width, obstacle1Height, obstacle1Depth)) {
        // Find closest face and set normal accordingly
        float nx = 0, ny = 0, nz = 0;
        float minDist = fabs(ballX - (obstacle1X - obstacle1Width / 2));
        nx = -1;

        float dist = fabs(ballX - (obstacle1X + obstacle1Width / 2));
        if (dist < minDist) { minDist = dist; nx = 1; ny = 0; nz = 0; }

        dist = fabs(ballY - (obstacle1Y - obstacle1Height / 2));
        if (dist < minDist) { minDist = dist; nx = 0; ny = -1; nz = 0; }

        dist = fabs(ballY - (obstacle1Y + obstacle1Height / 2));
        if (dist < minDist) { minDist = dist; nx = 0; ny = 1; nz = 0; }

        dist = fabs(ballZ - (obstacle1Z - obstacle1Depth / 2));
        if (dist < minDist) { minDist = dist; nx = 0; ny = 0; nz = -1; }

        dist = fabs(ballZ - (obstacle1Z + obstacle1Depth / 2));
        if (dist < minDist) { minDist = dist; nx = 0; ny = 0; nz = 1; }

        // Calculate reflection vector
        float dot = ballVX * nx + ballVY * ny + ballVZ * nz;
        ballVX = elasticity * (ballVX - 2 * dot * nx);
        ballVY = elasticity * (ballVY - 2 * dot * ny);
        ballVZ = elasticity * (ballVZ - 2 * dot * nz);

        // Move ball outside obstacle
        ballX += nx * 0.1f;
        ballY += ny * 0.1f;
        ballZ += nz * 0.1f;
        return;
    }

    // Check collision with second obstacle (wall cube at -9.5, 0, 0)
    if (checkSphereCubeCollision(ballX, ballY, ballZ, ballRadius,
        obstacle2X, obstacle2Y, obstacle2Z,
        obstacle2Width, obstacle2Height, obstacle2Depth)) {
        // Find closest face and set normal accordingly
        float nx = 0, ny = 0, nz = 0;
        float minDist = fabs(ballX - (obstacle2X - obstacle2Width / 2));
        nx = -1;

        float dist = fabs(ballX - (obstacle2X + obstacle2Width / 2));
        if (dist < minDist) { minDist = dist; nx = 1; ny = 0; nz = 0; }

        dist = fabs(ballY - (obstacle2Y - obstacle2Height / 2));
        if (dist < minDist) { minDist = dist; nx = 0; ny = -1; nz = 0; }

        dist = fabs(ballY - (obstacle2Y + obstacle2Height / 2));
        if (dist < minDist) { minDist = dist; nx = 0; ny = 1; nz = 0; }

        dist = fabs(ballZ - (obstacle2Z - obstacle2Depth / 2));
        if (dist < minDist) { minDist = dist; nx = 0; ny = 0; nz = -1; }

        dist = fabs(ballZ - (obstacle2Z + obstacle2Depth / 2));
        if (dist < minDist) { minDist = dist; nx = 0; ny = 0; nz = 1; }

        // Calculate reflection vector
        float dot = ballVX * nx + ballVY * ny + ballVZ * nz;
        ballVX = elasticity * (ballVX - 2 * dot * nx);
        ballVY = elasticity * (ballVY - 2 * dot * ny);
        ballVZ = elasticity * (ballVZ - 2 * dot * nz);

        // Move ball outside obstacle
        ballX += nx * 0.1f;
        ballY += ny * 0.1f;
        ballZ += nz * 0.1f;
    }
}

void drawObstacles() {
    // Draw first obstacle (ground cube)
    glPushMatrix();
    glTranslatef(obstacle1X, obstacle1Y, obstacle1Z);
    glColor3f(0.4, 0.4, 0.8);  // Blue color
    glScalef(obstacle1Width, obstacle1Height, obstacle1Depth);
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw second obstacle (wall cube at -9.5, 0, 0)
    glPushMatrix();
    glTranslatef(obstacle2X, obstacle2Y, obstacle2Z);
    glColor3f(0.4, 0.8, 0.4);  // Green color
    glScalef(obstacle2Width, obstacle2Height, obstacle2Depth);
    glutSolidCube(1.0);
    glPopMatrix();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '0': light0_enabled = !light0_enabled; break;
    case '1': light1_enabled = !light1_enabled; break;
    case '2': mat_shininess[0] += 5.0f; break;
    case ' ': // Space to stop the ball
        ballVX = ballVY = ballVZ = 0.0f;
        bounceCount = 0;
        break;
    case 'r': // Reset ball to center
        ballX = ballZ = 0.0f;
        ballY = 8.0f;
        ballVX = ballVY = ballVZ = 0.0f;
        bounceCount = 0;
        break;

        // Camera movement controls
    case 'a': // Move left
        cameraX -= cos(cameraYaw * 3.14159265f / 180.0f) * camMoveSpeed;
        cameraZ += sin(cameraYaw * 3.14159265f / 180.0f) * camMoveSpeed;
        break;
    case 'd': // Move right
        cameraX += cos(cameraYaw * 3.14159265f / 180.0f) * camMoveSpeed;
        cameraZ -= sin(cameraYaw * 3.14159265f / 180.0f) * camMoveSpeed;
        break;
    case 'q': // Move up
        cameraY += camMoveSpeed;
        break;
    case 'e': // Move down
        cameraY -= camMoveSpeed;
        break;
    case 'w': // Move forward
        cameraX += sin(cameraYaw * 3.14159265f / 180.0f) * camMoveSpeed;
        cameraZ += cos(cameraYaw * 3.14159265f / 180.0f) * camMoveSpeed;
        break;
    case 's': // Move backward
        cameraX -= sin(cameraYaw * 3.14159265f / 180.0f) * camMoveSpeed;
        cameraZ -= cos(cameraYaw * 3.14159265f / 180.0f) * camMoveSpeed;
        break;
    }

    updateCameraView();
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        cameraPitch += camRotateSpeed;
        if (cameraPitch > 89.0f) cameraPitch = 89.0f;
        break;
    case GLUT_KEY_DOWN:
        cameraPitch -= camRotateSpeed;
        if (cameraPitch < -89.0f) cameraPitch = -89.0f;
        break;
    case GLUT_KEY_LEFT:
        cameraYaw -= camRotateSpeed;
        break;
    case GLUT_KEY_RIGHT:
        cameraYaw += camRotateSpeed;
        break;
    }

    updateCameraView();
    glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        mouseLeftDown = (state == GLUT_DOWN);
        lastMouseX = x;
        lastMouseY = y;
    }
}

void mouseMotion(int x, int y) {
    if (mouseLeftDown) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        ballVX = deltaX * 0.02f;
        ballVZ = -deltaY * 0.02f;

        if (fabs(ballVX) < 0.5f) ballVX = (ballVX < 0) ? -0.5f : 0.5f;
        if (fabs(ballVZ) < 0.5f) ballVZ = (ballVZ < 0) ? -0.5f : 0.5f;

        ballVY = 0.8f;
        bounceCount = 0;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

void init(void) {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.4, 0.4, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat light1_diffuse[] = { 0.8f, 0.8f, 0.2f, 1.0f };
    GLfloat light1_specular[] = { 0.1f, 0.3f, 0.4f, 1.0f };
    GLfloat light1_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

    glEnable(GL_DEPTH_TEST);
    srand(time(0));
    lastFrameTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    updateCameraView();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up lights
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

    if (light0_enabled) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
    if (light1_enabled) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);

    // Draw wireframe cube
    glPushMatrix();
    glColor3f(0.1, 0.1, 0.4);
    glutWireCube(20.0);
    glPopMatrix();

    // Draw obstacles
    drawObstacles();

    // Draw ball
    glPushMatrix();
    glTranslatef(ballX, ballY, ballZ);
    glRotatef(ballAngle, 1.0f, 1.0f, 1.0f);
    glColor3f(0.8, 0.2, 0.2);
    glutSolidSphere(ballRadius, 32, 32);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    // Update ball position
    ballX += ballVX * deltaTime * 60.0f;
    ballY += ballVY * deltaTime * 60.0f;
    ballZ += ballVZ * deltaTime * 60.0f;

    // Apply gravity to ball
    ballVY += gravity * deltaTime * 60.0f;

    // Check floor collision for ball
    if (ballY < floorLevel) {
        ballY = floorLevel;

        if (currentTime - lastBounceTime < bounceTimeThreshold) {
            bounceCount++;
        }
        else {
            bounceCount = 1;
        }

        lastBounceTime = currentTime;

        if (bounceCount >= maxBounceCount) {
            ballVY = 0.0f;
            ballVX *= 0.5f;
            ballVZ *= 0.5f;
        }
        else {
            ballVY = -ballVY * elasticity;
        }
    }
    else if (ballY > ceilingLevel) {
        ballY = ceilingLevel;
        ballVY = -ballVY * elasticity;
    }

    // Check wall collisions for ball
    if (ballX < -wallLevel) {
        ballX = -wallLevel;
        ballVX = -ballVX * elasticity;
    }
    else if (ballX > wallLevel) {
        ballX = wallLevel;
        ballVX = -ballVX * elasticity;
    }

    if (ballZ < -wallLevel) {
        ballZ = -wallLevel;
        ballVZ = -ballVZ * elasticity;
    }
    else if (ballZ > wallLevel) {
        ballZ = wallLevel;
        ballVZ = -ballVZ * elasticity;
    }

    // Check collision with obstacles
    handleCollisionWithObstacles();

    // Apply friction
    ballVX *= pow(friction, deltaTime * 60.0f);
    ballVZ *= pow(friction, deltaTime * 60.0f);

    // Rotation
    ballAngle += 0.5f * deltaTime * 60.0f;
    if (ballAngle > 360) ballAngle -= 360;

    glutPostRedisplay();
    glutTimerFunc(7, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Projet");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutMainLoop();
    return 0;
}