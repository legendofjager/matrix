#include "matrix_operations.h"
#include "MATRIXES.h"

// вариант c
struct matrix {
    double* data;   // Данные матрицы
    size_t w;       // Ширина (количество столбцов)
    size_t h;       // Высота (количество строк)
};


// Сложение матриц (m1 += m2)
int matrix_add(matrix* m1, const matrix* m2) {
    // Проверка совместимости размеров матриц
    if (!m1 || !m2 || m1->w != m2->w || m1->h != m2->h)
        return -1;

    // Поэлементное сложение матриц
    for (size_t i = 0; i < m1->h; ++i) {
        for (size_t j = 0; j < m1->w; ++j) {
            *matrix_ptr(m1, i, j) += *matrix_cptr(m2, i, j);
        }
    }
    return 0;
}

// Вычитание матриц (m1 -= m2)
int matrix_sub(matrix* m1, const matrix* m2) {
    // Проверка совместимости размеров матриц
    if (!m1 || !m2 || m1->w != m2->w || m1->h != m2->h)
        return -1;

    // Поэлементное вычитание матриц
    for (size_t i = 0; i < m1->h; ++i) {
        for (size_t j = 0; j < m1->w; ++j) {
            *matrix_ptr(m1, i, j) -= *matrix_cptr(m2, i, j);
        }
    }
    return 0;
}

// Умножение матрицы на скаляр (m *= d)
void matrix_smul(matrix* m, double d) {
    if (!m) return;  // Проверка указателя

    // Поэлементное умножение матрицы на скаляр
    for (size_t i = 0; i < m->h; ++i) {
        for (size_t j = 0; j < m->w; ++j) {
            *matrix_ptr(m, i, j) *= d;
        }
    }
}

// Деление матрицы на скаляр (m /= d)
void matrix_sdiv(matrix* m, double d) {
    // Проверка деления на ноль
    if (!m || d == 0.0) return;
    matrix_smul(m, 1.0 / d);  // Через умножение на обратное значение
}

// Сложение матриц с сохранением результата (m = m1 + m2)
int matrix_add2(matrix* m, const matrix* m1, const matrix* m2) {
    // Проверка совместимости размеров матриц
    if (!m || !m1 || !m2 || m1->w != m2->w || m1->h != m2->h ||
        m->w != m1->w || m->h != m1->h)
        return -1;

    // Поэлементное сложение с сохранением результата
    for (size_t i = 0; i < m->h; ++i) {
        for (size_t j = 0; j < m->w; ++j) {
            *matrix_ptr(m, i, j) = *matrix_cptr(m1, i, j) + *matrix_cptr(m2, i, j);
        }
    }
    return 0;
}

// Вычитание матриц с сохранением результата (m = m1 - m2)
int matrix_sub2(matrix* m, const matrix* m1, const matrix* m2) {
    // Проверка совместимости размеров матриц
    if (!m || !m1 || !m2 || m1->w != m2->w || m1->h != m2->h ||
        m->w != m1->w || m->h != m1->h)
        return -1;

    // Поэлементное вычитание с сохранением результата
    for (size_t i = 0; i < m->h; ++i) {
        for (size_t j = 0; j < m->w; ++j) {
            *matrix_ptr(m, i, j) = *matrix_cptr(m1, i, j) - *matrix_cptr(m2, i, j);
        }
    }
    return 0;
}

// Умножение матрицы на скаляр с сохранением результата (m = m1 * d)
int matrix_smul2(matrix* m, const matrix* m1, double d) {
    // Проверка совместимости размеров матриц
    if (!m || !m1 || m->w != m1->w || m->h != m1->h)
        return -1;

    // Поэлементное умножение с сохранением результата
    for (size_t i = 0; i < m->h; ++i) {
        for (size_t j = 0; j < m->w; ++j) {
            *matrix_ptr(m, i, j) = *matrix_cptr(m1, i, j) * d;
        }
    }
    return 0;
}

// Деление матрицы на скаляр с сохранением результата (m = m1 / d)
int matrix_sdiv2(matrix* m, const matrix* m1, double d) {
    // Проверка деления на ноль
    if (d == 0.0) return -1;
    return matrix_smul2(m, m1, 1.0 / d);  // Через умножение на обратное значение
}

// Умножение матриц с сохранением результата в m1 (m1 *= m2)
int matrix_mul(matrix* m1, const matrix* m2) {
    // Проверка совместимости размеров матриц
    if (!m1 || !m2 || m1->w != m2->h)
        return -1;

    // Создание временной матрицы для результата
    matrix* temp = matrix_alloc(m2->w, m1->h);
    if (!temp) return -1;

    // Вычисление произведения матриц
    for (size_t i = 0; i < m1->h; ++i) {
        for (size_t j = 0; j < m2->w; ++j) {
            double sum = 0.0;
            // Скалярное произведение строки и столбца
            for (size_t k = 0; k < m1->w; ++k) {
                sum += *matrix_cptr(m1, i, k) * *matrix_cptr(m2, k, j);
            }
            *matrix_ptr(temp, i, j) = sum;
        }
    }

    // Копирование результата в исходную матрицу
    int result = matrix_assign(m1, temp);
    matrix_free(temp);  // Освобождение временной матрицы
    return result;
}

// Умножение матриц с сохранением результата (m = m1 * m2)
int matrix_mul2(matrix* m, const matrix* m1, const matrix* m2) {
    // Проверка совместимости размеров матриц
    if (!m || !m1 || !m2 || m1->w != m2->h || m->w != m2->w || m->h != m1->h)
        return -1;

    // Обработка случая, когда m совпадает с m1 или m2
    if (m == m1 || m == m2) {
        // Использование временной матрицы
        matrix* temp = matrix_alloc(m2->w, m1->h);
        if (!temp) return -1;

        // Рекурсивный вызов с временной матрицей
        int result = matrix_mul2(temp, m1, m2);
        if (result == 0) {
            result = matrix_assign(m, temp);  // Копирование результата
        }
        matrix_free(temp);  // Освобождение временной матрицы
        return result;
    }

    // Непосредственное вычисление произведения
    for (size_t i = 0; i < m1->h; ++i) {
        for (size_t j = 0; j < m2->w; ++j) {
            double sum = 0.0;
            // Скалярное произведение строки и столбца
            for (size_t k = 0; k < m1->w; ++k) {
                sum += *matrix_cptr(m1, i, k) * *matrix_cptr(m2, k, j);
            }
            *matrix_ptr(m, i, j) = sum;  // Сохранение результата
        }
    }
    return 0;
}
