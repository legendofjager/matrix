#include "MATRIXES.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

// вариант c
struct matrix {
    double* data;   // Данные матрицы
    size_t w;       // Ширина (количество столбцов)
    size_t h;       // Высота (количество строк)
};

// Выделение памяти под матрицу указанного размера
matrix* matrix_alloc(size_t w, size_t h) {
    matrix* m = malloc(sizeof(matrix));
    if (!m) return NULL;  // Проверка успешности выделения памяти

    // Выделение памяти под данные с инициализацией нулями
    m->data = calloc(w * h, sizeof(double));
    if (!m->data) {
        free(m);  // Освобождение памяти при ошибке
        return NULL;
    }

    m->w = w;  // Установка ширины
    m->h = h;  // Установка высоты
    return m;
}

// Освобождение памяти, занятой матрицей
void matrix_free(matrix* m) {
    if (m) {
        free(m->data);  // Освобождение данных
        free(m);        // Освобождение структуры
    }
}

// Создание копии матрицы
matrix* matrix_copy(const matrix* m) {
    if (!m) return NULL;  // Проверка входного параметра

    matrix* copy = matrix_alloc(m->w, m->h);
    if (!copy) return NULL;  // Проверка успешности выделения

    // Копирование данных
    memcpy(copy->data, m->data, m->w * m->h * sizeof(double));
    return copy;
}

// Получение указателя на элемент матрицы
double* matrix_ptr(matrix* m, size_t i, size_t j) {
    return &m->data[i * m->w + j];  // Вычисление позиции элемента
}

// Получение константного указателя на элемент матрицы
const double* matrix_cptr(const matrix* m, size_t i, size_t j) {
    return &m->data[i * m->w + j];  // Вычисление позиции элемента
}

// Заполнение матрицы нулями
void matrix_set_zero(matrix* m) {
    if (!m) return;
    memset(m->data, 0, m->w * m->h * sizeof(double));  // Обнуление памяти
}

// Преобразование матрицы в единичную
void matrix_set_id(matrix* m) {
    if (!m) return;
    matrix_set_zero(m);  // Сначала заполняем нулями

    // Определяем минимальную размерность (для неквадратных матриц)
    size_t min_dim = m->w < m->h ? m->w : m->h;

    // Заполняем диагональ единицами
    for (size_t i = 0; i < min_dim; ++i) {
        *matrix_ptr(m, i, i) = 1.0;
    }
}

// Создание нулевой матрицы
matrix* matrix_alloc_zero(size_t w, size_t h) {
    matrix* m = matrix_alloc(w, h);
    if (m) matrix_set_zero(m);  // Инициализация нулями
    return m;
}

// Создание единичной матрицы
matrix* matrix_alloc_id(size_t w, size_t h) {
    matrix* m = matrix_alloc(w, h);
    if (m) matrix_set_id(m);  // Инициализация как единичной
    return m;
}

// Присваивание матриц (копирование содержимого)
int matrix_assign(matrix* m1, const matrix* m2) {
    // Проверка совместимости размеров
    if (!m1 || !m2 || m1->w != m2->w || m1->h != m2->h)
        return -1;  // Ошибка: несовместимые размеры

    // Копирование данных
    memcpy(m1->data, m2->data, m1->w * m1->h * sizeof(double));
    return 0;  // Успешное выполнение
}

// Транспонирование матрицы
void matrix_transpose(matrix* m) {
    if (!m) return;  // Проверка входного параметра

    // Транспонирование на месте для квадратных матриц
    if (m->w == m->h) {
        for (size_t i = 0; i < m->h; ++i) {
            for (size_t j = i + 1; j < m->w; ++j) {
                // Обмен элементов
                double tmp = *matrix_ptr(m, i, j);
                *matrix_ptr(m, i, j) = *matrix_ptr(m, j, i);
                *matrix_ptr(m, j, i) = tmp;
            }
        }
    } else {
        // Для неквадратных матриц создаем временную копию
        matrix* temp = matrix_copy(m);
        if (!temp) return;  // Проверка успешности копирования

        // Освобождаем старые данные и выделяем новую память
        free(m->data);
        m->data = malloc(m->w * m->h * sizeof(double));
        if (!m->data) {
            // Восстановление при ошибке
            m->data = temp->data;
            temp->data = NULL;
            matrix_free(temp);
            return;
        }

        // Сохраняем старые размеры
        size_t old_w = m->w;
        size_t old_h = m->h;

        // Меняем размеры местами
        m->w = old_h;
        m->h = old_w;

        // Копируем данные с транспонированием
        for (size_t i = 0; i < m->h; ++i) {
            for (size_t j = 0; j < m->w; ++j) {
                *matrix_ptr(m, i, j) = *matrix_cptr(temp, j, i);
            }
        }

        matrix_free(temp);  // Освобождаем временную матрицу
    }
}

// Перестановка строк матрицы
void matrix_swap_rows(matrix* m, size_t i1, size_t i2) {
    // Проверка корректности индексов
    if (!m || i1 >= m->h || i2 >= m->h) return;

    // Поэлементный обмен строк
    for (size_t j = 0; j < m->w; ++j) {
        double tmp = *matrix_ptr(m, i1, j);
        *matrix_ptr(m, i1, j) = *matrix_ptr(m, i2, j);
        *matrix_ptr(m, i2, j) = tmp;
    }
}

// Перестановка столбцов матрицы
void matrix_swap_cols(matrix* m, size_t j1, size_t j2) {
    // Проверка корректности индексов
    if (!m || j1 >= m->w || j2 >= m->w) return;

    // Поэлементный обмен столбцов
    for (size_t i = 0; i < m->h; ++i) {
        double tmp = *matrix_ptr(m, i, j1);
        *matrix_ptr(m, i, j1) = *matrix_ptr(m, i, j2);
        *matrix_ptr(m, i, j2) = tmp;
    }
}

// Умножение строки на скаляр
void matrix_mul_row(matrix* m, size_t i, double d) {
    // Проверка корректности индекса
    if (!m || i >= m->h) return;

    // Умножение каждого элемента строки
    for (size_t j = 0; j < m->w; ++j) {
        *matrix_ptr(m, i, j) *= d;
    }
}

// Сложение двух строк матрицы
void matrix_add_rows(matrix* m, size_t i1, size_t i2) {
    // Проверка корректности индексов
    if (!m || i1 >= m->h || i2 >= m->h) return;

    // Почленное сложение строк
    for (size_t j = 0; j < m->w; ++j) {
        *matrix_ptr(m, i1, j) += *matrix_ptr(m, i2, j);
    }
}

// Вычисление нормы матрицы (максимальная сумма модулей элементов строки)
double matrix_norm(const matrix* m) {
    // Проверка на нулевой размер
    if (!m || m->w == 0 || m->h == 0) return 0.0;

    double max_sum = 0.0;  // Максимальная сумма
    for (size_t i = 0; i < m->h; ++i) {
        double row_sum = 0.0;  // Сумма элементов строки
        for (size_t j = 0; j < m->w; ++j) {
            row_sum += fabs(*matrix_cptr(m, i, j));  // Сумма модулей
        }
        // Обновление максимума
        if (row_sum > max_sum) {
            max_sum = row_sum;
        }
    }
    return max_sum;
}

// Печать матрицы
void matrix_print(const matrix* m) {
    if (!m) {
        printf("NULL matrix\n");
        return;
    }

    // Построчный вывод элементов
    for (size_t i = 0; i < m->h; ++i) {
        for (size_t j = 0; j < m->w; ++j) {
            printf("%8.4f ", *matrix_cptr(m, i, j));  // Форматированный вывод
        }
        printf("\n");  // Переход на новую строку
    }
}

// Ввод матрицы с клавиатуры
matrix* matrix_input(size_t w, size_t h) {
    matrix* m = matrix_alloc(w, h);
    if (!m) return NULL;  // Проверка успешности выделения

    printf("Enter matrix %zux%zu:\n", h, w);  // Приглашение к вводу
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            scanf("%lf", matrix_ptr(m, i, j));  // Чтение элемента
        }
    }

    return m;
}

