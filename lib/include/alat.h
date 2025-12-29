/* ALAT (Advanced Linear Algebra Toolkit) */

#ifndef ALAT_H
#define ALAT_H

/* Standard C libraries */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <assert.h>
 
/* User-defined constants */

#define MAX_ROW					64
#define MAX_COL					64
#define MAX_DIM					MAX_ROW * MAX_COL
#define MAX_LEN					MAX_DIM
 
#define DEG(rad)              (rad * 180.0 / M_PI)
#define RAD(deg)              (deg * M_PI / 180.0)
 
#define assert_shape(shape) do {                                    \
	assert (shape.row > 0 && shape.row <= MAX_ROW &&                 \
		shape.col > 0 && shape.col <= MAX_COL);                       \
} while (0)
  
#define assert_matrix(matrix) assert_shape(matrix.shape)
 
#define assert_place(matrix, place) do {                            \
	assert (place.crow >= 0 && place.crow < matrix.shape.col &&      \
			  place.ccol >= 0 && place.ccol < matrix.shape.col);       \
} while (0)
 
#define assert_vector(vector) do {                                  \
	assert (vector.dim > 0 && vector.dim <= MAX_DIM);                \
	assert (vector.coor == cartesian ||                              \
		vector.coor == cylindrical || vector.coor == spherical);      \
	if (vector.coor == cylindrical ||                                \
		vector.coor == spherical) assert(vector.dim == 3);            \
} while (0)

#define assert_complex(complex) do {                                \
	assert (complex.format == algebraic || complex.format == polar); \
} while (0)
 
#define assert_points(points) do {                                  \
	assert (points.length > 0 && points.length <= MAX_LEN);          \
} while (0)

/* User-defined data types */
 
typedef unsigned int row_t; 
typedef unsigned int col_t; 
typedef unsigned int dim_t;
typedef unsigned int len_t;
typedef double mat_t;
typedef double vec_t;
typedef double com_t;
typedef double point_t;
typedef char * msg_t;
 
/* User-defined enumerations */

typedef enum {
	false, 
	true, 
} bool_t;
 
typedef enum {
	horizontal, 
	vertical, 
} axis_t;

typedef enum {
	cartesian, 
	cylindrical, 
	spherical
} coor_t;

typedef enum {
	decimal, 
	radians, 
	degrees
} form_t;

typedef enum {
	algebraic, 
	polar 
} format_t;

/* User-defined stuctures */

typedef struct {
	row_t row;               
	col_t col;                
} shape_t;
 
typedef struct {
	row_t crow;
	col_t ccol;
} place_t;
  
typedef struct {
	shape_t shape;           
	mat_t matrix[MAX_ROW][MAX_COL];   
} matrix_t;

typedef struct {
	dim_t dim; 
	coor_t coor;
	vec_t vector[MAX_DIM];
} vector_t;

typedef struct {
	format_t format;           
	com_t complex[2];        
} complex_t;
 
typedef struct {
	len_t length;
	point_t points[MAX_LEN];
} points_t;

/* Matrix Operations */

extern bool_t alat_matrix_issquare(matrix_t matrix); 
extern bool_t alat_matrix_ishomogen(matrix_t matrix); 
extern bool_t alat_matrix_iszeros(matrix_t matrix); 
extern bool_t alat_matrix_isones(matrix_t matrix); 
extern bool_t alat_matrix_isarbitrary(matrix_t matrix, double value); 
extern bool_t alat_matrix_isidentity(matrix_t matrix); 
extern bool_t alat_matrix_islowertri(matrix_t matrix); 
extern bool_t alat_matrix_isuppertri(matrix_t matrix);
extern bool_t alat_matrix_isdiagonal(matrix_t matrix);   
extern bool_t alat_matrix_issymmetric(matrix_t matrix);
extern bool_t alat_matrix_isequal(matrix_t fmatrix, matrix_t smatrix); 
extern bool_t alat_matrix_istriangle(matrix_t matrix); 
extern bool_t alat_matrix_isinvertible(matrix_t matrix); 
extern shape_t alat_matrix_shape(matrix_t matrix); 
extern matrix_t alat_matrix_zeros(shape_t shape); 
extern matrix_t alat_matrix_ones(shape_t shape); 
extern matrix_t alat_matrix_identity(shape_t shape); 
extern matrix_t alat_matrix_arbitrary(double value, shape_t shape); 
extern matrix_t alat_matrix_sequential(double start, double end, shape_t shape); 
extern matrix_t alat_matrix_random(shape_t shape); 
extern matrix_t alat_matrix_uniform(int start, int end, shape_t shape); 
extern matrix_t alat_matrix_randint(int start, int end, shape_t shape); 
extern matrix_t alat_matrix_diagonal(matrix_t matrix); 
extern double alat_matrix_max(matrix_t matrix); 
extern double alat_matrix_min(matrix_t matrix); 
extern double alat_matrix_value(matrix_t matrix, place_t place); 
extern place_t alat_matrix_argmax(matrix_t matrix, bool_t reverse); 
extern place_t alat_matrix_argmin(matrix_t matrix, bool_t reverse); 
extern double alat_matrix_mean(matrix_t matrix); 
extern double alat_matrix_stddev(matrix_t matrix); 
extern double alat_matrix_median(matrix_t matrix); 
extern double alat_matrix_norm_one(matrix_t matrix); 
extern double alat_matrix_norm_infinity(matrix_t matrix); 
extern double alat_matrix_norm_frobenius(matrix_t matrix); 
extern int alat_matrix_rank(matrix_t matrix); 
extern matrix_t alat_matrix_abs(matrix_t matrix); 
extern matrix_t alat_matrix_pow(matrix_t matrix, double n); 
extern matrix_t alat_matrix_sqrt(matrix_t matrix); 
extern matrix_t alat_matrix_root(matrix_t matrix, double n); 
extern matrix_t alat_matrix_degrees(matrix_t matrix); 
extern matrix_t alat_matrix_radians(matrix_t matrix); 
extern matrix_t alat_matrix_subset(matrix_t matrix, place_t fplace, place_t splace); 
extern matrix_t alat_matrix_insert_row(matrix_t matrix, double value, row_t crow); 
extern matrix_t alat_matrix_insert_col(matrix_t matrix, double value, col_t ccol); 
extern matrix_t alat_matrix_append_row(matrix_t matrix, double value); 
extern matrix_t alat_matrix_append_col(matrix_t matrix, double value); 
extern matrix_t alat_matrix_remove_row(matrix_t matrix, row_t crow); 
extern matrix_t alat_matrix_remove_col(matrix_t matrix, col_t ccol); 
extern matrix_t alat_matrix_pop_row(matrix_t matrix); 
extern matrix_t alat_matrix_pop_col(matrix_t matrix); 
extern matrix_t alat_matrix_sort(matrix_t matrix_t, bool_t reverse); 
extern matrix_t alat_matrix_sum(matrix_t matrix, axis_t axis); 
extern matrix_t alat_matrix_shuffle(matrix_t matrix); 
extern matrix_t alat_matrix_reshape(matrix_t matrix, shape_t shape); 
extern matrix_t alat_matrix_transpose(matrix_t matrix); 
extern matrix_t alat_matrix_concat(matrix_t fmatrix, matrix_t smatrix, axis_t axis); 
extern matrix_t alat_matrix_add(matrix_t fmatrix, matrix_t smatrix); 
extern matrix_t alat_matrix_sub(matrix_t fmatrix, matrix_t smatrix); 
extern matrix_t alat_matrix_scalar_mul(matrix_t matrix, double scalar); 
extern matrix_t alat_matrix_dot_mul(matrix_t fmatrix, matrix_t smatrix); 
extern matrix_t alat_matrix_cross_mul(matrix_t fmatrix, matrix_t smatrix); 
extern matrix_t alat_matrix_scalar_div(matrix_t matrix, double scalar); 
extern matrix_t alat_matrix_swap(matrix_t matrix); 
extern matrix_t alat_matrix_dot_div(matrix_t fmatrix, matrix_t smatrix); 
extern double alat_matrix_det(matrix_t matrix); 
extern matrix_t alat_matrix_minors(matrix_t matrix); 
extern matrix_t alat_matrix_cofactors(matrix_t matrix); 
extern matrix_t alat_matrix_adjoint(matrix_t matrix); 
extern matrix_t alat_matrix_inverse(matrix_t matrix); 
extern matrix_t alat_matrix_uppertri(matrix_t matrix);  
extern matrix_t alat_matrix_lowertri(matrix_t matrix); 
extern matrix_t alat_matrix_solve(matrix_t matrix); 

/* Vector Operations */

extern vector_t alat_vector_transform(vector_t vector, coor_t coor); 
extern bool_t alat_vector_iscartesian(vector_t vector); 
extern bool_t alat_vector_iscylindrical(vector_t vector); 
extern bool_t alat_vector_isspherical(vector_t vector); 
extern bool_t alat_vector_iszeros(vector_t vector); 
extern bool_t alat_vector_isones(vector_t vector); 
extern bool_t alat_vector_isequal(vector_t fvector, vector_t svector); 
extern bool_t alat_vector_isarbitrary(vector_t vector, double value); 
extern bool_t alat_vector_iscasc(vector_t fvector, vector_t svector); 
extern bool_t alat_vector_istriangle(vector_t fvector, vector_t svector); 
extern bool_t alat_vector_ispythagorean(vector_t fvector, vector_t svector); 
extern bool_t alat_vector_issteep(vector_t fvector, vector_t svector); 
extern bool_t alat_vector_isparallel(vector_t fvector, vector_t svector); 
extern double alat_vector_car_n(vector_t vector, int n); 
extern double alat_vector_car_x(vector_t vector); 
extern double alat_vector_car_y(vector_t vector); 
extern double alat_vector_car_z(vector_t vector); 
extern double alat_vector_cyl_radial(vector_t vector); 
extern double alat_vector_cyl_angular(vector_t vector); 
extern double alat_vector_cyl_z(vector_t vector); 
extern double alat_vector_sph_radial(vector_t vector); 
extern double alat_vector_sph_polar(vector_t vector); 
extern double alat_vector_sph_azimuthal(vector_t vector); 
extern vector_t alat_vector_zeros(dim_t dim, coor_t coor); 
extern vector_t alat_vector_ones(dim_t dim, coor_t coor); 
extern vector_t alat_vector_arbitrary(double value, dim_t dim, coor_t coor); 
extern vector_t alat_vector_sequential(double start, double end, dim_t dim, coor_t coor); 
extern vector_t alat_vector_random(dim_t dim, coor_t coor); 
extern vector_t alat_vector_uniform(int start, int end, dim_t dim, coor_t coor); 
extern vector_t alat_vector_randint(int start, int end, dim_t dim, coor_t coor); 
extern dim_t alat_vector_dim(vector_t vector); 
extern coor_t alat_vector_coor(vector_t vector); 
extern double alat_vector_lenght(vector_t vector); 
extern double alat_vector_magnitude(vector_t vector); 
extern vector_t alat_vector_abs(vector_t vector, coor_t coor); 
extern vector_t alat_vector_pow(vector_t vector, double n, coor_t coor); 
extern vector_t alat_vector_root(vector_t vector, double n, coor_t coor);  
extern vector_t alat_vector_sqrt(vector_t vector, coor_t coor); 
extern vector_t alat_vector_unit(vector_t vector, coor_t coor); 
extern vector_t alat_vector_add(vector_t fvector, vector_t svector, coor_t coor); 
extern vector_t alat_vector_sub(vector_t fvector, vector_t svector, coor_t coor); 
extern vector_t alat_vector_scalar_mul(vector_t vector, double scalar, coor_t coor); 
extern double alat_vector_distance(vector_t fvector, vector_t svector); 
extern double alat_vector_dot_mul(vector_t fvector, vector_t svector); 
extern vector_t alat_vector_cross_mul(vector_t fvector, vector_t svector, coor_t coor); 
extern vector_t alat_vector_project(vector_t fvector, vector_t svector, coor_t coor); 
extern vector_t alat_vector_reflect(vector_t fvector, vector_t svector, coor_t coor); 
extern vector_t alat_vector_interpolate(vector_t fvector, vector_t svector, double factor, coor_t coor); 
extern double alat_vector_angle(vector_t fvector, vector_t svector, form_t form); 

/* Complex Number Operations */

extern complex_t alat_complex_transform(complex_t complex, format_t format); 
extern bool_t alat_complex_isalgebraic(complex_t complex); 
extern bool_t alat_complex_ispolar(complex_t complex); 
extern bool_t alat_complex_iszero(complex_t complex); 
extern bool_t alat_complex_istriangle(complex_t fcomplex, complex_t scomplex); 
extern bool_t alat_complex_isequal(complex_t fcomplex, complex_t scomplex); 
extern complex_t alat_complex_zero(format_t format); 
extern double alat_complex_real(complex_t complex); 
extern double alat_complex_imaginary(complex_t complex); 
extern double alat_complex_magnitude(complex_t complex); 
extern double alat_complex_argument(complex_t complex); 
extern complex_t alat_complex_sin(complex_t complex, format_t format); 
extern complex_t alat_complex_cos(complex_t complex, format_t format); 
extern complex_t alat_complex_tan(complex_t complex, format_t format); 
extern complex_t alat_complex_sinh(complex_t complex, format_t format); 
extern complex_t alat_complex_cosh(complex_t complex, format_t format); 
extern complex_t alat_complex_tanh(complex_t complex, format_t format); 
extern complex_t alat_complex_asin(complex_t complex, format_t format); 
extern complex_t alat_complex_acos(complex_t complex, format_t format); 
extern complex_t alat_complex_asinh(complex_t complex, format_t format); 
extern complex_t alat_complex_acosh(complex_t complex, format_t format); 
extern complex_t alat_complex_log(complex_t complex, format_t format); 
extern complex_t alat_complex_log2(complex_t complex, format_t format); 
extern complex_t alat_complex_log10(complex_t complex, format_t format); 
extern complex_t alat_complex_logx(complex_t complex, double x, format_t format); 
extern complex_t alat_complex_add(complex_t fcomplex, complex_t scomplex, format_t format); 
extern complex_t alat_complex_scalar_mul(complex_t complex, double scalar, format_t format); 
extern complex_t alat_complex_scalar_div(complex_t complex, double scalar, format_t format); 
extern complex_t alat_complex_sub(complex_t fcomplex, complex_t scomplex, format_t format); 
extern complex_t alat_complex_mul(complex_t fcomplex, complex_t scomplex, format_t format); 
extern complex_t alat_complex_div(complex_t fcomplex, complex_t scomplex, format_t format); 
extern complex_t alat_complex_pow(complex_t complex, double n, format_t format); 
extern complex_t alat_complex_root(complex_t complex, double n, format_t format); 
extern complex_t alat_complex_sqrt(complex_t complex, format_t format); 
extern complex_t alat_complex_conjugate(complex_t complex, format_t format); 
extern complex_t alat_complex_reciprocol(complex_t complex, format_t format); 
 
/* Cryptography Operations */

extern matrix_t alat_crypt_to_matrix(msg_t message, shape_t shape); 
extern matrix_t alat_crypt_encode(msg_t message, matrix_t encoder); 
extern matrix_t alat_crypt_decode(matrix_t encoded, matrix_t encoder); 
extern msg_t alat_crypt_to_message(matrix_t encoded, matrix_t encoder); 

/* Application Operations */

extern points_t alat_app_poly_curve_fitting(points_t xpoints, points_t ypoints); 
extern points_t alat_app_least_sqaures_reg(points_t xpoints, points_t ypoints); 
extern double alat_app_area(points_t xpoints, points_t ypoints); 
extern double alat_app_volume(points_t xpoints, points_t ypoints, points_t zpoints); 

#endif /* ALAT_H */
