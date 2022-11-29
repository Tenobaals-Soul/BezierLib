#ifndef INCLUDE_BEZIER_CURVES_H
#define INCLUDE_BEZIER_CURVES_H
#include <stddef.h>

/**
 * @brief the type holding a Beziér curve
 * 
 * For example with the dimension 2, the grade 3 and vertices 3, the curve
 * will go through point 0, 3 and 6 and point 1 and 2 are the handles between
 * points 0 and 3. Point 4 and 5 are the handles between point 3 and 6.
 * For a correct Beziér curve, the handles around a point should be
 * rotationally symmetrical arount that point: `p_n - p_{n-1} = p_{n+1} - p_n`
 * meaning: `p3 - p2 = p4 - p3`. For more information look at README.md (todo)
 */
typedef struct bc_curve_t {
    // The grade of the Beziér curve. 2 means a direct line between 2 vertices
    // 3 means a quadratic Beziér curve between 3 verticies and so on.
    unsigned int _grade;
    // The dimension of the coordinate space the curve is in. `_grade >= 1`
    unsigned int _dimension;
    // This is also the lenght of the `_vertices` array.
    unsigned long _vertice_count;
    // Array of the vertices with the size
    // `_dimension * _vertice_count * (_grade + 1)`
    // All values for one dimesion are stored in consecutive positions.
    // First all values of the x dimension are stored than all y ...
    // with `_vertice_count` subarrays of size each describing a point.
    double* _vertices;
} bc_curve_t[1];

/**
 * @brief initialize a Beziér curve
 * 
 * Initializes a Beziér curve in a `dimension` dimensional coordinate space
 * with `vertices` number of vertices each having `grade` points per vertice.
 * Setting grade to 0 equals setting it to 3.
 * 
 * @param curve the curve object
 * @param dimension the dimension of the coordinate system
 * @param grade the grade of the curve (2 = quadratic, 3 = cubic)
 */
void bc_init_bezier_curve(bc_curve_t curve,
                          unsigned int dimension,
                          unsigned int grade,
                          unsigned long vertices);

/**
 * @brief initialize a Beziér curve with two dimensions
 * 
 * Initializes a Beziér curve in a two dimensional coordinate space
 * with `vertices` number of vertices each having `grade` points per vertice.
 * Setting grade to 0 equals setting it to 3.
 * 
 * @param curve the curve object
 * @param dimension the dimension of the coordinate system
 * @param grade the grade of the curve (2 = quadratic, 3 = cubic)
 */
void bc_init_bezier_curve2(bc_curve_t curve,
                           unsigned int grade,
                           unsigned long vertices);

/**
 * @brief initialize a Beziér curve with three dimensions
 * 
 * Initializes a Beziér curve in a three dimensional coordinate space
 * with `vertices` number of vertices each having `grade` points per vertice.
 * Setting grade to 0 equals setting it to 3.
 * 
 * @param curve the curve object
 * @param dimension the dimension of the coordinate system
 * @param grade the grade of the curve (2 = quadratic, 3 = cubic)
 */
void bc_init_bezier_curve3(bc_curve_t curve,
                           unsigned int grade,
                           unsigned long vertices);

/**
 * @brief initialize a standart Beziér curve
 * 
 * Initializes a Beziér curve in a two dimensional coordinate space
 * with 0 vertices each having 3 points per vertice.
 * 
 * @param curve the curve object
 * @param dimension the dimension of the coordinate system
 * @param grade the grade of the curve (2 = quadratic, 3 = cubic)
 */
void bc_init_standart_bezier(bc_curve_t curve);

/**
 * @brief set the point `n` of a Beziér curve using vaargs
 * 
 * Sets the point number `n` of a Beziér curve.
 * The arguments must be dimension number of points, each of type `double`.
 * 
 * @param curve the curve object
 * @param n the index to set
 * @param ... dimension doubles, the coordinate
 */
void bc_set_point(bc_curve_t curve, unsigned long n, ...);

/**
 * @brief set the point `n` of a Beziér curve using a vector
 * 
 * Sets the point number `n` of a Beziér curve.
 * The argument bust must be an array of doubles with a lenght equal
 * (or greater) to the dimension.
 * 
 * @param curve the curve object
 * @param n the index to set
 * @param points an array of doubles with the lenght equal to the dimension
 */
void bc_set_pointv(bc_curve_t curve, unsigned long n,
                   const double* points);

/**
 * @brief set the point `n` of a Beziér curve using two numbers
 * 
 * Sets the point number `n` of a Beziér curve.
 * 
 * @param curve the curve object
 * @param n the index to set
 * @param x the x coordinate
 * @param y the y coordinate
 */
void bc_set_point2(bc_curve_t curve, unsigned long n,
                   double x, double y);

/**
 * @brief set the point `n` of a Beziér curve using three numbers
 * 
 * Sets the point number `n` of a Beziér curve.
 * 
 * @param curve the curve object
 * @param n the index to set
 * @param x the x coordinate
 * @param y the y coordinate
 * @param y the z coordinate
 */
void bc_set_point3(bc_curve_t curve, unsigned long n,
                   double x, double y, double z);

/**
 * @brief appends a point to a Beziér curve using vaargs
 * 
 * Appends a point to the end of the current curve.
 * Neccessary handles needed are added automatically.
 * The arguments must be dimension number of points, each of type `double`.
 * 
 * @param curve the curve object
 * @param ... dimension doubles, the coordinate
 */
void bc_append_point(bc_curve_t curve, ...);

/**
 * @brief appends a point to a Beziér curve using a vector
 * 
 * Appends a point to the end of the current curve.
 * The argument bust must be an array of doubles with a lenght equal
 * (or greater) to the dimension.
 * 
 * @param curve the curve object
 * @param points an array of doubles with the lenght equal to the dimension
 */
void bc_append_pointv(bc_curve_t curve, const double* points);

/**
 * @brief appends a point to a Beziér curve using two numbers
 * 
 * Appends a point to the end of the current curve.
 * 
 * @param curve the curve object
 * @param x the x coordinate
 * @param y the y coordinate
 */
void bc_append_point2(bc_curve_t curve,
                      double x, double y);

/**
 * @brief appends a point to a Beziér curve using three numbers
 * 
 * Appends a point to the end of the current curve.
 * 
 * @param curve the curve object
 * @param x the x coordinate
 * @param y the y coordinate
 * @param y the z coordinate
 */
void bc_append_point3(bc_curve_t curve,
                      double x, double y, double z);

/**
 * @brief get the point on the curve with index `n` using vaargs
 * 
 * Interpolates on the curve and writes the output to the argument.
 * The arguments must be dimension number of points, each of type `double*`.
 * 
 * @param curve the curve object
 * @param n the index of the desired point
 * @param ... must be dimension double*
 */
void bc_get_point(bc_curve_t curve, unsigned long n, ...);

/**
 * @brief get the point on the curve with index `n` using a vector
 * 
 * Interpolates on the curve and writes the output to the argument.
 * The argument bust must be an array of `double*` with a lenght equal
 * (or greater) to the dimension.
 * 
 * @param curve the curve object
 * @param n the index of the desired point
 * @param points an array of doubles with the lenght equal to the dimension 
 */
void bc_get_pointv(bc_curve_t curve, unsigned long n, double* points);

/**
 * @brief get the point on the curve with index `n` using two number pointer
 * 
 * @param curve the curve object
 * @param n the index of the desired point
 * @param x the x coordinate
 * @param y the y coordinate
 */
void bc_get_point2(bc_curve_t curve, unsigned long n,
                   double* x,
                   double* y);

/**
 * @brief get the point on the curve with index `n` using three number pointer.
 * 
 * @param curve the curve object
 * @param n the index of the desired point
 * @param x the x coordinate
 * @param y the y coordinate
 * @param z the z coordinate
 */
void bc_get_point3(bc_curve_t curve, unsigned long n,
                   double* x,
                   double* y,
                   double* z);

/**
 * @brief get the point on the curve for a given t using vaargs
 * 
 * Interpolates on the curve and writes the output to the argument.
 * The arguments must be dimension number of points, each of type `double*`.
 * 
 * @param curve the curve object
 * @param ... must be dimension double*
 */
void bc_interpolate(bc_curve_t curve, double t, ...);

/**
 * @brief get the point on the curve for a given t using a vector
 * 
 * Interpolates on the curve and writes the output to the argument.
 * The argument bust must be an array of `double*` with a lenght equal
 * (or greater) to the dimension.
 * 
 * @param curve the curve object
 * @param points an array of doubles with the lenght equal to the dimension 
 */
void bc_interpolatev(bc_curve_t curve, double t, double* points);

/**
 * @brief get the point on the curve for a given t using two number pointer
 * 
 * Interpolates on the curve and writes the output to the argument.
 * 
 * @param curve the curve object
 * @param x the x coordinate
 * @param y the y coordinate
 */
void bc_interpolate2(bc_curve_t curve, double t,
                     double* x,
                     double* y);

/**
 * @brief get the point on the curve for a given t using three number pointer
 * 
 * Interpolates on the curve and writes the output to the argument.
 * 
 * @param curve the curve object
 * @param x the x coordinate
 * @param y the y coordinate
 * @param z the z coordinate
 */
void bc_interpolate3(bc_curve_t curve, double t,
                     double* x,
                     double* y,
                     double* z);

#endif