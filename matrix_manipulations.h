#ifndef MATRIX_MANIPULATIONS_H_INCLUDED
#define MATRIX_MANIPULATIONS_H_INCLUDED

#include "MATRIXES.h"



// Специальные функции
matrix* matrix_exp(const matrix* m, double eps);
matrix* matrix_solve_gauss(const matrix* A, const matrix* B);


#endif // MATRIX_MANIPULATIONS_H_INCLUDED
