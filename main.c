#include "MATRIXES.h"
#include <stdio.h>
#include "matrix_operations.h"
#include "matrix_manipulations.h"

int main() {
    // Тестирование матричной экспоненты
    printf("Testing matrix exponential:\n");
    // Создание диагональной матрицы 3x3
    matrix* m = matrix_alloc(3, 3);
    *matrix_ptr(m, 0, 0) = 1.0;  // Элемент (0,0)
    *matrix_ptr(m, 1, 1) = 2.0;  // Элемент (1,1)
    *matrix_ptr(m, 2, 2) = -1.0; // Элемент (2,2)

    printf("Input matrix:\n");
    matrix_print(m);  // Печать исходной матрицы

    // Вычисление матричной экспоненты с точностью 1e-10
    matrix* exp_m = matrix_exp(m, 1e-10);
    printf("Matrix exponential:\n");
    matrix_print(exp_m);  // Печать результата

    // Освобождение памяти
    matrix_free(m);
    matrix_free(exp_m);

    // Тестирование метода Гаусса
    printf("\nTesting Gauss method:\n");
    // Создание матрицы коэффициентов A
    matrix* A = matrix_alloc(3, 3);
    *matrix_ptr(A, 0, 0) = 2.0; *matrix_ptr(A, 0, 1) = 1.0; *matrix_ptr(A, 0, 2) = -1.0;
    *matrix_ptr(A, 1, 0) = -3.0; *matrix_ptr(A, 1, 1) = -1.0; *matrix_ptr(A, 1, 2) = 2.0;
    *matrix_ptr(A, 2, 0) = -2.0; *matrix_ptr(A, 2, 1) = 1.0; *matrix_ptr(A, 2, 2) = 2.0;

    // Создание вектора правых частей B
    matrix* B = matrix_alloc(1, 3);  // Вектор-столбец
    *matrix_ptr(B, 0, 0) = 8.0;     // Первый элемент
    *matrix_ptr(B, 1, 0) = -11.0;   // Второй элемент
    *matrix_ptr(B, 2, 0) = -3.0;    // Третий элемент

    printf("Matrix A:\n");
    matrix_print(A);  // Печать матрицы A
    printf("\nRight-hand side vector B:\n");
    matrix_print(B);  // Печать вектора B

    // Решение системы уравнений AX = B
    matrix* X = matrix_solve_gauss(A, B);
    printf("\nSolution X:\n");
    matrix_print(X);  // Печать решения

    // Проверка решения: вычисление невязки A*X - B
    if (X) {
        matrix* AX = matrix_alloc(1, 3);  // Вектор для результата A*X
        matrix_mul2(AX, A, X);           // Умножение A на X
        matrix_sub(AX, B);                // Вычитание B (AX - B)
        printf("Residual A*X - B (should be near zero):\n");
        matrix_print(AX);                 // Печать невязки
        printf("Residual norm: %f\n", matrix_norm(AX));  // Вычисление нормы невязки
        matrix_free(AX);                  // Освобождение памяти
    }

    // Освобождение памяти
    matrix_free(A);
    matrix_free(B);
    matrix_free(X);

    return 0;
}
