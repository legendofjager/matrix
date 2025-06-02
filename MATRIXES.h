#ifndef MATRIXES_H_INCLUDED
#define MATRIXES_H_INCLUDED

#include <stddef.h>

// Неполный тип матрицы (инкапсуляция)
struct matrix;
typedef struct matrix matrix;

// Создание и освобождение матриц
matrix* matrix_alloc(size_t w, size_t h); // Выделение памяти под матрицу w x h
matrix* matrix_copy(const matrix* m);     // Создание копии матрицы
void matrix_free(matrix* m);              // Освобождение памяти матрицы

// Быстрый доступ к элементам
double* matrix_ptr(matrix* m, size_t i, size_t j);         // Указатель на элемент (i,j)
const double* matrix_cptr(const matrix* m, size_t i, size_t j); // Константный указатель на элемент

// Инициализация матриц
void matrix_set_zero(matrix* m);          // Заполнение матрицы нулями
void matrix_set_id(matrix* m);            // Преобразование в единичную матрицу
matrix* matrix_alloc_zero(size_t w, size_t h); // Создание нулевой матрицы
matrix* matrix_alloc_id(size_t w, size_t h);   // Создание единичной матрицы

// Присваивание матриц
int matrix_assign(matrix* m1, const matrix* m2); // Копирование данных из m2 в m1

// Операции с матрицами
void matrix_transpose(matrix* m);         // Транспонирование матрицы
void matrix_swap_rows(matrix* m, size_t i1, size_t i2); // Перестановка строк
void matrix_swap_cols(matrix* m, size_t j1, size_t j2); // Перестановка столбцов
void matrix_mul_row(matrix* m, size_t i, double d); // Умножение строки на число
void matrix_add_rows(matrix* m, size_t i1, size_t i2); // Сложение двух строк
double matrix_norm(const matrix* m);      // Вычисление нормы матрицы

// Ввод/вывод матриц
void matrix_print(const matrix* m);       // Печать матрицы на экране
matrix* matrix_input(size_t w, size_t h); // Ввод матрицы с клавиатуры

#endif // MATRIXES_H_INCLUDED
