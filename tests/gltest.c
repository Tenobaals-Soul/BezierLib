#include <GL/glut.h>
#include <GL/gl.h>
#include <bezier_curves.h>

#define TMAX 32

bc_curve_t curve;

void draw_cross(double x, double y) {
    glBegin(GL_LINES);
    double width  = (double) glutGet(GLUT_WINDOW_WIDTH);
    double height = (double) glutGet(GLUT_WINDOW_HEIGHT);
    double size = 20;
    glVertex2d(x - size / width, y - size / height);
    glVertex2d(x + size / width, y + size / height);
    glVertex2d(x + size / width, y - size / height);
    glVertex2d(x - size / width, y + size / height);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(255, 255, 255);
    glBegin(GL_LINE_STRIP);
    for (unsigned long t = 0; t <= TMAX; t += 1) {
        double x, y;
        bc_interpolate2(curve, t / (double) TMAX, &x, &y);
        glVertex2d(x, y);
    }
    glEnd();
    for (unsigned long i = 0; i < 7; i++) {
        double x, y;
        bc_get_point2(curve, i, &x, &y);
        draw_cross(x, y);
    }
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("curves");

    glutDisplayFunc(display);

    bc_init_bezier_curve2(curve, 3, 3);
    double x[7] = {-0.8, -0.6, -0.4, 0.1, 0.6, 0.5, 0.7};
    double y[7] = {-0.8, 0.8, -0.6, 0.2, 0.8, 0.7, -0.7};
    for (int i = 0; i < 7; i++) {
        bc_set_point2(curve, i, x[i], y[i]);
    }
    glutMainLoop();
}