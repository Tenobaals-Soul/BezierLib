#include <GL/glut.h>
#include <GL/gl.h>
#include <bezier_curves.h>

#define TMAX 128

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
    for (unsigned long i = 0; i < 4; i++) {
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

    bc_init_bezier_curve2(curve, 3, 2);
    double x[4] = {-0.8, -0.6, 0.6, 0.8};
    double y[4] = {-0.8, 0.8, -0.6, 0.6};
    for (int i = 0; i < 4; i++) {
        bc_set_point2(curve, i, x[i], y[i]);
    }
    glutMainLoop();
}