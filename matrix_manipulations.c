#include "matrix_manipulations.h"
#include "matrix_operations.h"
#include <math.h>

// вариант c
struct matrix {
    double* data;   // Данные матрицы
    size_t w;       // Ширина (количество столбцов)
    size_t h;       // Высота (количество строк)
};

// Вычисление матричной экспоненты exp(m)
matrix* matrix_exp(const matrix* m, double eps) {
    // Проверка входных параметров: матрица должна быть квадратной
    if (!m || m->w != m->h) return NULL;

    size_t n = m->w;
    // Создание единичной матрицы (начальное приближение)
    matrix* result = matrix_alloc_id(n, n);
    if (!result) return NULL;

    // Первый член ряда Тейлора
    matrix* term = matrix_copy(m);
    if (!term) {
        matrix_free(result);
        return NULL;
    }

    int k = 1;  // Счетчик итераций/факториал
    // Итерации пока норма текущего члена больше epsilon
    while (matrix_norm(term) >= eps) {
        // Копирование текущего результата
        matrix* temp = matrix_copy(result);
        if (!temp) {
            matrix_free(result);
            matrix_free(term);
            return NULL;
        }

        // Добавление текущего члена ряда к результату
        if (matrix_add(temp, term) != 0) {
            matrix_free(temp);
            matrix_free(result);
            matrix_free(term);
            return NULL;
        }

        // Обновление результата
        matrix_free(result);
        result = temp;

        // Вычисление следующего члена ряда: term = term * m / (k+1)
        matrix* next_term = matrix_alloc(n, n);
        if (!next_term) {
            matrix_free(result);
            matrix_free(term);
            return NULL;
        }

        // Умножение текущего члена на исходную матрицу
        if (matrix_mul2(next_term, term, m) != 0) {
            matrix_free(next_term);
            matrix_free(result);
            matrix_free(term);
            return NULL;
        }

        // Деление на следующий факториал
        matrix_sdiv(next_term, ++k);

        // Переход к следующему члену ряда
        matrix_free(term);
        term = next_term;
    }

    // Освобождение памяти последнего члена ряда
    matrix_free(term);
    return result;  // Возврат результата
}

// Решение СЛАУ методом Гаусса с выбором ведущего элемента
matrix* matrix_solve_gauss(const matrix* A, const matrix* B) {
    // Проверка входных параметров:
    // - A должна быть квадратной
    // - B должна быть вектором-столбцом
    // - Размеры A и B должны быть согласованы
    if (!A || !B || A->w != A->h || A->h != B->h || B->w != 1)
        return NULL;

    const size_t n = A->h;  // Размер системы

    // Создание копий матриц, чтобы не изменять оригиналы
    matrix* ACopy = matrix_copy(A);
    matrix* BCopy = matrix_copy(B);
    if (!ACopy || !BCopy) {
        matrix_free(ACopy);
        matrix_free(BCopy);
        return NULL;
    }

    // Прямой ход метода Гаусса
    for (size_t k = 0; k < n; ++k) {
        // Поиск строки с максимальным элементом в текущем столбце
        size_t max_row = k;
        double max_val = fabs(*matrix_ptr(ACopy, k, k));

        // Поиск максимума в столбце ниже текущей строки
        for (size_t i = k + 1; i < n; ++i) {
            double val = fabs(*matrix_ptr(ACopy, i, k));
            if (val > max_val) {
                max_val = val;
                max_row = i;
            }
        }

        // Перестановка строк, если найден больший элемент
        if (max_row != k) {
            matrix_swap_rows(ACopy, k, max_row);  // Перестановка в матрице A

            // Перестановка соответствующих элементов в векторе B
            double tmp = *matrix_ptr(BCopy, k, 0);
            *matrix_ptr(BCopy, k, 0) = *matrix_ptr(BCopy, max_row, 0);
            *matrix_ptr(BCopy, max_row, 0) = tmp;
        }

        // Проверка на вырожденность (нулевой диагональный элемент)
        if (fabs(*matrix_ptr(ACopy, k, k)) < 1e-12) {
            matrix_free(ACopy);
            matrix_free(BCopy);
            return NULL;  // Система вырождена
        }

        // Исключение элементов ниже диагонали
        for (size_t i = k + 1; i < n; ++i) {
            // Вычисление множителя для исключения
            double factor = *matrix_ptr(ACopy, i, k) / *matrix_ptr(ACopy, k, k);

            // Вычитание строки k из строки i в векторе B
            *matrix_ptr(BCopy, i, 0) -= factor * *matrix_ptr(BCopy, k, 0);

            // Вычитание строки k из строки i в матрице A
            for (size_t j = k; j < n; ++j) {
                *matrix_ptr(ACopy, i, j) -= factor * *matrix_ptr(ACopy, k, j);
            }
        }
    }

    // Обратный ход метода Гаусса
    matrix* X = matrix_alloc(1, n);  // Вектор решений
    if (!X) {
        matrix_free(ACopy);
        matrix_free(BCopy);
        return NULL;
    }

    // Начиная с последней строки и двигаясь вверх
    for (size_t i = n; i-- > 0; ) {
        double sum = 0.0;
        // Суммирование известных членов
        for (size_t j = i + 1; j < n; ++j) {
            sum += *matrix_ptr(ACopy, i, j) * *matrix_ptr(X, j, 0);
        }
        // Вычисление неизвестного
        *matrix_ptr(X, i, 0) = (*matrix_ptr(BCopy, i, 0) - sum) / *matrix_ptr(ACopy, i, i);
    }

    // Освобождение временных матриц
    matrix_free(ACopy);
    matrix_free(BCopy);
    return X;  // Возврат вектора решений
}
