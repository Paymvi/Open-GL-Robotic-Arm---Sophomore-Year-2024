#include <math.h>       // Include file for math
#include <cmath> // I added this
#include "glut.h"      // Include file for glut


/*
Chelsea Alysson Ongjoco
ID: 2552139
roboticArm.cpp
This assignment is to make a robotic arm by making and transforming rectagular prisms that the user can interact with using the keyboard.
*/

// Set the dimensions of the window
int windowWidth = 1024;                 // Width
int windowHeight = 768;                 // Height

float angleX = 0.0f;      // Rotation around the X-axis for crystal ball effect
float angleY = 0.0f;      // Rotation around the Y-axis for crystal ball effect

float jointAngle = 0.0f;  // Rotation for the elbow
float jointAngle2 = 0.0f;  // Rotation for the finger
float jointAngle3 = 0.0f;  // Rotation for the finger joint


// Aspect ratio
float aspect = float(windowWidth) / float(windowHeight);

// Initialize the variables for the camera (spin and rotating)
float spin = 0.0;           // Spin for camera
bool rotating = false;      // Rotating for camera

// Camera properties
double eye[] = { 0.0, 0.0, 5.0 };        // Position
double center[] = { 0.0, 0.0, 0.0 };     // Look at
double up[] = { 0.0, 1.0, 0.0 };         // Up vector

// Default camera values
double eyeDefault[] = { 0.0f, 0.0f, 5.0f };
double centerDefault[] = { 0.0f, 0.0f, 0.0f };
double upDefault[] = { 0.0f, 1.0f, 0.0f };
double angleXDefault = 0.0f;
double angleYDefault = 0.0f;

// Computes the cross product of a and b and stores it in c
void crossProduct(double a[], double b[], double c[]) {
    c[0] = a[1] * b[2] - a[2] * b[1]; // x component
    c[1] = a[2] * b[0] - a[0] * b[2]; // y component
    c[2] = a[0] * b[1] - a[1] * b[0]; // z component
}

// Normalizes the given vector to make the length 1 (unit vector)
void normalize(double a[]) {
    double norm = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] /= norm; // x component
    a[1] /= norm; // y component
    a[2] /= norm; // z component
}

// Rotates the point p around the axis a by an angle theta
void rotatePoint(double a[], double theta, double p[]) {
    double temp[3];
    temp[0] = p[0] * cos(theta) + (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]) - a[2] * p[1] * sin(theta) + a[1] * p[2] * sin(theta);
    temp[1] = p[1] * cos(theta) + (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]) + a[2] * p[0] * sin(theta) - a[0] * p[2] * sin(theta);
    temp[2] = p[2] * cos(theta) + (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]) - a[1] * p[0] * sin(theta) + a[0] * p[1] * sin(theta);

    p[0] = temp[0]; // x component
    p[1] = temp[1]; // y component
    p[2] = temp[2]; // z component
}

// Rotates the camera to the left (around the up vector)
void Left() {
    double speed = 3.1415 / 8;
    rotatePoint(up, speed, eye);
}

// Rotates the camera to the right (around the up vector)
void Right() {
    double speed = 3.1415 / 8;
    rotatePoint(up, -speed, eye);
}

// Rotates the camera upwards (by tilting)
void Up() {
    double speed = 3.1415 / 8;
    double rot_axis[3], look[3];
    look[0] = -eye[0]; look[1] = -eye[1]; look[2] = -eye[2];
    crossProduct(look, up, rot_axis);
    normalize(rot_axis);
    rotatePoint(rot_axis, speed, eye);
    rotatePoint(rot_axis, speed, up);
}

// Tilts the view so that the camera rotates downward
void Down() {
    double speed = 3.1415 / 8;
    double rot_axis[3], look[3];
    look[0] = -eye[0]; look[1] = -eye[1]; look[2] = -eye[2];
    crossProduct(look, up, rot_axis);
    normalize(rot_axis);
    rotatePoint(rot_axis, -speed, eye);
    rotatePoint(rot_axis, -speed, up);
}

// Handles the inputs of the special keys (the keyboard keys) to move the camera
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT: Left(); break;
    case GLUT_KEY_RIGHT: Right(); break;
    case GLUT_KEY_UP: Up(); break;
    case GLUT_KEY_DOWN: Down(); break;
    }
    glutPostRedisplay();
}

// Draws the cube
void gluWireCube(void) {
    glBegin(GL_QUADS);

    // Top face (y = 1.0f)
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f); // Orange
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glEnd();
}
// The official display function that displays the figure
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear color and depth buffers
    glEnable(GL_DEPTH_TEST);                                // Enable depth testing

    glMatrixMode(GL_MODELVIEW);  // Set model view matrix
    glLoadIdentity();

    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]); // Adjust the camera

    glPushMatrix();  // Save the current transformation state

    // Apply crystal ball-style rotation (rotation around the center of the scene)
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);  // Rotate around the X-axis
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);  // Rotate around the Y-axis


    // -------------------------------------- DRAW THE THUMB FINGER --------------------------------------

    // -------- 1st Joint --------

    // for degrees and radians conversions
    float temp3;
    temp3 = (jointAngle2) / 57.29578;
    float temp2;
    temp2 = (jointAngle2) / 57.29578;
    float temp;


    glPushMatrix();

        temp = jointAngle / 57.29578;

        glTranslatef(2 * cos(temp), (2 * sin(temp)), 0.f);      // Move to the area of the palm
        glTranslatef(1.0f, -0.3f, 0.0f);
        glRotatef(-jointAngle2, 0.0f, 0.0f, 1.0f);              // Make it slightly open & apply rotation for the joint 
        glTranslatef(0.3f, 0.1f, 0.0f);                         // Move the prism edge that you want to keep fixed on the origin b4 rotating
        glScalef(0.3f, -0.1f, 0.1f);                            // Scale
        gluWireCube();                                        // Draw the finger part

    glPopMatrix();

    // -------- 2nd Joint --------
    glPushMatrix();

        glTranslatef(0.6 * cos(temp2), -0.6 * sin(temp2), 0.f); // Move to the area of the finger joint
        glTranslatef(2 * cos(temp), (2 * sin(temp)), 0.f);      // Move to the area of the palm
        glTranslatef(1.0f, -0.3f, 0.0f);
        glRotatef(-jointAngle3, 0.0f, 0.0f, 1.0f);              // Make it slightly open & apply rotation for the joint 
        glTranslatef(0.3f, 0.1f, 0.0f);                         // Move the prism edge that you want to keep fixed on the origin b4 rotating
        glScalef(0.3f, -0.1f, 0.1f);                            // Scale
        gluWireCube();                                        // Draw the finger part

    glPopMatrix();



    // -------------------------------------- DRAW THE BACK FINGER --------------------------------------

    // -------- 1st Joint --------
    glPushMatrix();

        glTranslatef(-0.05f, -0.1f, 0.0f);
        glTranslatef(2 * cos(temp), 2 * sin(temp), 0.f);        // Move to the area of the palm
        glTranslatef(1.0f, 0.3f, -0.205f);                      // Move to the end of the prism (translation) & move z to seperate the fingers
        glRotatef(jointAngle2, 0.0f, 0.0f, 1.0f);               // Apply rotation for the joint 
        glTranslatef(0.3f, 0.1f, 0.0f);                         // Move the prism edge that you want to keep fixed on the origin b4 rotating
        glScalef(0.3f, 0.1f, 0.1f);                             // Scale
        gluWireCube();                                        // Draw the finger part

    glPopMatrix();

    // -------- 2nd Joint --------

    glPushMatrix();

        //glTranslatef(-0.05f, 0.5f, 0.0f);
        glTranslatef(0.6 * cos(temp2), 0.6 * sin(temp2), 0.f);  // Move to the area of the finger joint
        glTranslatef(2 * cos(temp), 2 * sin(temp), 0.f);        // Move to the area of the palm
        glTranslatef(-0.05f, -0.1f, 0.0f);
        glTranslatef(1.0f, 0.3f, -0.205f);                      // Move to the end of the prism (translation) & move z to seperate the fingers
        glRotatef(jointAngle3, 0.0f, 0.0f, 1.0f);               // Apply rotation for the joint 
        glTranslatef(0.3f, 0.1f, 0.0f);                         // Move the prism edge that you want to keep fixed on the origin b4 rotating
        glScalef(0.3f, 0.1f, 0.1f);                             // Scale
        gluWireCube();                                        // Draw the finger part

    glPopMatrix();


    // -------------------------------------- DRAW THE MIDDLE FINGER --------------------------------------

    // -------- 1st Joint --------
    glPushMatrix();

        glTranslatef(-0.05f, -0.1f, 0.0f);
        glTranslatef(2 * cos(temp), 2 * sin(temp), 0.f);        // Move to the area of the palm
        glTranslatef(1.0f, 0.3f, 0.0f);                         // Move to the end of the prism (translation) (keep z the same because it is the middle finger)
        glRotatef(jointAngle2, 0.0f, 0.0f, 1.0f);               // Apply rotation for the joint 
        glTranslatef(0.3f, 0.1f, 0.0f);                         // Move the prism edge that you want to keep fixed on the origin b4 rotating
        glScalef(0.3f, 0.1f, 0.1f);                             // Scale
        gluWireCube();                                        // Draw the finger part

    glPopMatrix();

    // -------- 2nd Joint --------

    glPushMatrix();

        //glTranslatef(-0.05f, 0.5f, 0.0f);
        glTranslatef(0.6 * cos(temp2), 0.6 * sin(temp2), 0.f);  // Move to the area of the finger joint
        glTranslatef(2 * cos(temp), 2 * sin(temp), 0.f);        // Move to the area of the palm
        glTranslatef(-0.05f, -0.1f, 0.0f);
        glTranslatef(1.0f, 0.3f, 0.0f);                         // Move to the end of the prism (translation) (keep z the same because it is the middle finger)
        glRotatef(jointAngle3, 0.0f, 0.0f, 1.0f);               // Apply rotation for the joint 
        glTranslatef(0.3f, 0.1f, 0.0f);                         // Move the prism edge that you want to keep fixed on the origin b4 rotating
        glScalef(0.3f, 0.1f, 0.1f);                             // Scale
        gluWireCube();                                        // Draw the finger part

    glPopMatrix();


    // -------------------------------------- DRAW THE FRONT FINGER --------------------------------------

    // -------- 1st Joint --------
    glPushMatrix();

        glTranslatef(-0.05f, -0.1f, 0.0f);
        glTranslatef(2 * cos(temp), 2 * sin(temp), 0.f);            // Move to the area of the palm
        glTranslatef(1.0f, 0.3f, 0.205f);                       // Move to the end of the prism (translation) & move z to seperate the fingers
        glRotatef(jointAngle2, 0.0f, 0.0f, 1.0f);               // Apply rotation for the joint 
        glTranslatef(0.3f, 0.1f, 0.0f);                         // Move the prism edge that you want to keep fixed on the origin b4 rotating
        glScalef(0.3f, 0.1f, 0.1f);                             // Scale
        gluWireCube();                                        // Draw the finger part

    glPopMatrix();

    // -------- 2nd Joint --------

    glPushMatrix();

        //glTranslatef(-0.05f, 0.5f, 0.0f);
        glTranslatef(0.6 * cos(temp2), 0.6 * sin(temp2), 0.f);  // Move to the area of the finger joint
        glTranslatef(2 * cos(temp), 2 * sin(temp), 0.f);        // Move to the area of the palm
        glTranslatef(-0.05f, -0.1f, 0.0f);
        glTranslatef(1.0f, 0.3f, 0.205f);                       // Move to the end of the prism (translation) & move z to seperate the fingers
        glRotatef(jointAngle3, 0.0f, 0.0f, 1.0f);               // Apply rotation for the joint 
        glTranslatef(0.3f, 0.1f, 0.0f);                         // Move the prism edge that you want to keep fixed on the origin b4 rotating
        glScalef(0.3f, 0.1f, 0.1f);                             // Scale
        gluWireCube();                                        // Draw the finger part

    glPopMatrix();

    // -------------------------------------- DRAW A RECT PRISM --------------------------------------

    glPushMatrix();

        glScalef(1.0f, 0.2f, 0.2f);                 // Scale to a rectangular prism
        gluWireCube();                              // Draw the arm part

    glPopMatrix();  // Restore the OG state b4 translation for the 2nd prism

    // -------------------------------------- DRAW A RECT PRISM --------------------------------------
    glPushMatrix();

        glTranslatef(1.0f, 0.2f, 0.0f);             // Move to the end of the first prism (translation)
        glRotatef(jointAngle, 0.0f, 0.0f, 1.0f);    // Apply rotation for the joint (arm bending)... Rotate around the Z-axis
        glTranslatef(1.0f, -0.2f, 0.0f);            // Move the prism edge that you want to keep fixed on the origin b4 rotating
        glScalef(1.0f, 0.2f, 0.2f);
        gluWireCube();                              // Draw the arm part

    glPopMatrix();


    glutSwapBuffers();  // Swap front and back buffers (double buffering)
}


void keyboard(unsigned char key, int x, int y) {


    switch (key) {
    case '1':
        jointAngle -= 5.0f;  // Bend the arm downward (open)
        break;
    case '!':
        jointAngle += 5.0f;  // Bend the arm upward (close)
        break;
    case '2':
        jointAngle2 += 5.0f; // Open fingers
        break;
    case '@':
        jointAngle2 -= 5.0f; // Close fingers
        break;
    case '3':
        jointAngle3 += 5.0f; // Open fingers 2
        break;
    case '#':
        jointAngle3 -= 5.0f; // Close fingers 2
        break;
    case 'r':
        // reset to default (no rotation)
        jointAngle = 0;
        jointAngle = 0;
        jointAngle2 = 0;
        jointAngle2 = 0;
        jointAngle3 = 0;
        jointAngle3 = 0;
        
        // reset the camera
        for (int i = 0; i < 3; i++) {
            eye[i] = eyeDefault[i];
            center[i] = centerDefault[i];
            up[i] = upDefault[i];
        }
        angleX = angleXDefault;
        angleY = angleYDefault;

        break;
    default:
        break;
    }
    glutPostRedisplay();  // Redraw the window
}

// Initializes the perspective projection matrix
void init(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 1.0, 20.0);                        // Set perspective
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                                          // Initialize GLUT
    glutInitWindowSize(windowWidth, windowHeight);                  // Set window size
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);       // Enable double buffering, RGB color mode, and depth buffer
    glutCreateWindow("Robotic Arm");                                // Create window with title

    init();  // Initialize the perspective and camera settings

    glutDisplayFunc(display);                                       // Display function
    glutSpecialFunc(specialKeys);                                   // Special key function for camera rotation
    glutKeyboardFunc(keyboard);                                     // So that the robot moves according to the keyboard

    glutMainLoop();  // Enter the GLUT main loop
    return 0;
}
