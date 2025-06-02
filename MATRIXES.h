#ifndef MATRIXES_H_INCLUDED
#define MATRIXES_H_INCLUDED

#include <stddef.h>

// �������� ��� ������� (������������)
struct matrix;
typedef struct matrix matrix;

// �������� � ������������ ������
matrix* matrix_alloc(size_t w, size_t h); // ��������� ������ ��� ������� w x h
matrix* matrix_copy(const matrix* m);     // �������� ����� �������
void matrix_free(matrix* m);              // ������������ ������ �������

// ������� ������ � ���������
double* matrix_ptr(matrix* m, size_t i, size_t j);         // ��������� �� ������� (i,j)
const double* matrix_cptr(const matrix* m, size_t i, size_t j); // ����������� ��������� �� �������

// ������������� ������
void matrix_set_zero(matrix* m);          // ���������� ������� ������
void matrix_set_id(matrix* m);            // �������������� � ��������� �������
matrix* matrix_alloc_zero(size_t w, size_t h); // �������� ������� �������
matrix* matrix_alloc_id(size_t w, size_t h);   // �������� ��������� �������

// ������������ ������
int matrix_assign(matrix* m1, const matrix* m2); // ����������� ������ �� m2 � m1

// �������� � ���������
void matrix_transpose(matrix* m);         // ���������������� �������
void matrix_swap_rows(matrix* m, size_t i1, size_t i2); // ������������ �����
void matrix_swap_cols(matrix* m, size_t j1, size_t j2); // ������������ ��������
void matrix_mul_row(matrix* m, size_t i, double d); // ��������� ������ �� �����
void matrix_add_rows(matrix* m, size_t i1, size_t i2); // �������� ���� �����
double matrix_norm(const matrix* m);      // ���������� ����� �������

// ����/����� ������
void matrix_print(const matrix* m);       // ������ ������� �� ������
matrix* matrix_input(size_t w, size_t h); // ���� ������� � ����������

#endif // MATRIXES_H_INCLUDED
