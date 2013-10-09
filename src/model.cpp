#include "glm.h"
#include <iostream>

//Writing a comment to check if i can committ


// Define some constants we shall need later
#define GLUT_KEY_ESCAPE 27  // The keycode for the escape key.
#define GLUT_KEY_ENTER 13   // The keycode for "enter" key.

// --------------- Global variables --------------------- //


// --------------- Forward declarations ------------- //
int main(int argc, char* argv[]);

void display();
void keyboard(unsigned char key, int x, int y);

/**
 * Keyboard handler
 */
void keyboard(unsigned char key, int x, int y) {
    if (key == GLUT_KEY_ESCAPE || key == ' ') glutLeaveMainLoop();
}

void controls(int key, int x, int y) {

}

void idle() {

    glutPostRedisplay();
}

// ----------------------------------------------- //
/**
 * Program entry point
 */
int main(int argc, char* argv[]) {

    // Initialize GLUT and create a window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(30,30);
    glutCreateWindow("GraphicsProject2013");

    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);

    // Set up perspective
    glMatrixMode(GL_PROJECTION);
        gluPerspective(45, 1, 0.5, 100);
    glMatrixMode(GL_MODELVIEW);

    // Enable basic lighting
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    // Register handlers
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard); // Every time the key is pressed, invoke this
    glutSpecialFunc(controls);
    glutIdleFunc(idle);

    // Run main application loop
    glutMainLoop();
    return 0;
}

/**
 * This function is invoked every time the screen is being repainted.
 */
void display() {
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up view
    glLoadIdentity();
    gluLookAt(6, 6, 6, 0, 0, 0, 0, 1, 0);

    // Draw the y = 0 plane
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_QUADS);
        glVertex3f(-3, 0, 3);
        glVertex3f(-3, 0, -3);
        glVertex3f(3, 0, -3);
        glVertex3f(3, 0, 3);
    glEnd();

    // Flush picture
    glutSwapBuffers();
}
