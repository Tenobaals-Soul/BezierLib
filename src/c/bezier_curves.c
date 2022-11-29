#include <bezier_curves.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <memory.h>

#define LIST_ALLOCATION_SIZE ((4096 / sizeof(double)) * sizeof(double))

__attribute__((__const__))
static size_t vert_byte_size(bc_curve_t curve) {
    return sizeof(double) * curve->_dimension *
        (curve->_vertice_count - 1) * (curve->_grade + 1);
}

void init_binomial_coefficients(unsigned long* out, unsigned long level) {
    out[0] = 1;
    for (unsigned int i = 1; i <= level; i++)
        out[i] = 0;
    for (unsigned int i = 1; i <= level; i++) {
        for (int j = i; j > 0; j--)
            out[j] = out[j] + out[j - 1];
    }
}

void bc_init_bezier_curve(bc_curve_t curve,
                          unsigned int dimension,
                          unsigned int grade,
                          unsigned long vertices) {
    curve->_dimension = dimension;
    curve->_grade = grade;
    curve->_vertice_count = vertices;
    size_t required = sizeof(double) * dimension * vertices * (grade + 1);
    curve->_vertices = malloc(
        ((LIST_ALLOCATION_SIZE + required - 1) / required) * required
    );
    curve->_bin_cof_buffer = malloc(
        sizeof(unsigned long) * curve->_grade
    );
    init_binomial_coefficients(curve->_bin_cof_buffer, curve->_grade);
}

void bc_init_bezier_curve2(bc_curve_t curve,
                           unsigned int grade,
                           unsigned long vertices) {
    bc_init_bezier_curve(curve, 2, grade, vertices);
}

void bc_init_bezier_curve3(bc_curve_t curve,
                           unsigned int grade,
                           unsigned long vertices) {
    bc_init_bezier_curve(curve, 3, grade, vertices);
}

void bc_init_standart_bezier(bc_curve_t curve) {
        bc_init_bezier_curve(curve, 2, 3, 0);
}

static inline void bc_set_point_internal(bc_curve_t curve,
                                         unsigned long n, va_list v) {
    unsigned long off = curve->_dimension * n;
    for (unsigned int i = 0; i < (curve->_grade + 1); i++) {
        double val = va_arg(v, double);
        curve->_vertices[off + i] = val;
    }
}

void bc_set_point(bc_curve_t curve, unsigned long n, ...) {
    va_list v;
    va_start(v, n);
    bc_set_point_internal(curve, n, v);
    va_end(v);
}

void bc_set_pointv(bc_curve_t curve, unsigned long n,
                   const double* points) {
    unsigned long off = curve->_dimension * n;
    for (unsigned int i = 0; i < (curve->_grade + 1); i++) {
        curve->_vertices[off + i] = points[i];
    }
}

void bc_set_point2(bc_curve_t curve, unsigned long n,
                   double x, double y) {
    unsigned long off = curve->_dimension * n;
    curve->_vertices[off++] = x;
    curve->_vertices[off++] = y;
}

void bc_set_point3(bc_curve_t curve, unsigned long n,
                   double x, double y, double z) {
    unsigned long off = curve->_dimension * n;
    curve->_vertices[off++] = x;
    curve->_vertices[off++] = y;
    curve->_vertices[off++] = z;
}

static unsigned long append_point_space(bc_curve_t curve) {
    if (vert_byte_size(curve) % LIST_ALLOCATION_SIZE == 0) {
        curve->_vertices = realloc(curve->_vertices,
            vert_byte_size(curve) + LIST_ALLOCATION_SIZE);
    }
    return (curve->_vertice_count - 1) * (curve->_grade + 1);
}

void bc_append_point(bc_curve_t curve, ...) {
    unsigned long n = append_point_space(curve);
    va_list v;
    va_start(v, curve);
    bc_set_point_internal(curve, n, v);
    va_end(v);
}

void bc_append_pointv(bc_curve_t curve, const double* points) {
    unsigned long n = append_point_space(curve);
    bc_set_pointv(curve, n, points);
}

void bc_append_point2(bc_curve_t curve,
                      double x, double y) {
    unsigned long n = append_point_space(curve);
    bc_set_point2(curve, n, x, y);
}

void bc_append_point3(bc_curve_t curve,
                      double x, double y, double z) {
    unsigned long n = append_point_space(curve);
    bc_set_point3(curve, n, x, y, z);
}

static inline void bc_get_point_internal(bc_curve_t curve,
                                         unsigned long n, va_list v) {
    unsigned long off = curve->_dimension * n;
    for (unsigned int i = 0; i < (curve->_grade + 1); i++) {
        double* valptr = va_arg(v, double*);
        *valptr = curve->_vertices[off + i];
    }
}

void bc_get_point(bc_curve_t curve, unsigned long n, ...) {
    va_list v;
    va_start(v, n);
    bc_get_point_internal(curve, n, v);
    va_end(v);
}

void bc_get_pointv(bc_curve_t curve, unsigned long n, double* points) {
    unsigned long off = curve->_dimension * n;
    for (unsigned int i = 0; i < (curve->_grade + 1); i++) {
        points[i] = curve->_vertices[off + i];
    }
}

void bc_get_point2(bc_curve_t curve, unsigned long n,
                   double* x,
                   double* y) {
    unsigned long off = curve->_dimension * n;
    *x = curve->_vertices[off++];
    *y = curve->_vertices[off++];
}

void bc_get_point3(bc_curve_t curve, unsigned long n,
                   double* x,
                   double* y,
                   double* z) {
    unsigned long off = curve->_dimension * n;
    *x = curve->_vertices[off++];
    *y = curve->_vertices[off++];
    *z = curve->_vertices[off++];
}

static inline double lerp(double a, double b, double t) {
    return (1 - t) * a + (t) * b;
}

static double interpolate_internal(bc_curve_t curve,
                                   unsigned int dimension,
                                   double t) {
    unsigned long vertices_in_curve = curve->_grade + 1;
    unsigned long off = t * (curve->_vertice_count - 1);
    if (off == curve->_vertice_count - 1) // t == 1 would result in an overrun
        off--;
    off *= curve->_dimension * vertices_in_curve;
    off += dimension;
    double out_buffer[vertices_in_curve];
    for (unsigned long i = 0; i < vertices_in_curve; i++) {
        out_buffer[i] = curve->_vertices[off + curve->_dimension * i];
    }
    for (unsigned long i = 1; i < vertices_in_curve; i++) {
        for (unsigned long j = 0; j < vertices_in_curve - i; j++) {
            out_buffer[j] = lerp(out_buffer[j], out_buffer[j + 1], t);
        }
    }
    return out_buffer[0];
}

void bc_interpolate(bc_curve_t curve, double t, ...) {
    va_list v;
    va_start(v, t);
    for (unsigned int i = 0; i < curve->_dimension; i++) {
        double* out = va_arg(v, double*);
        *out = interpolate_internal(curve, i, t);
    }
    va_end(v);
}

void bc_interpolatev(bc_curve_t curve, double t, double* points) {
    for (unsigned int i = 0; i < curve->_dimension; i++) {
        points[i] = interpolate_internal(curve, i, t);
    }
}

void bc_interpolate2(bc_curve_t curve, double t,
                     double* x,
                     double* y) {
    *x = interpolate_internal(curve, 0, t);
    *y = interpolate_internal(curve, 1, t);
}

void bc_interpolate3(bc_curve_t curve, double t,
                     double* x,
                     double* y,
                     double* z) {
    *x = interpolate_internal(curve, 0, t);
    *y = interpolate_internal(curve, 1, t);
    *z = interpolate_internal(curve, 2, t);
}