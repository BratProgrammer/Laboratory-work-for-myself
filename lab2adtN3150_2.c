#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Проверка на то что символы являются числом типа double
bool сheck_double(char *str[]) {
    unsigned int dot_number = 0;
    for (int i = 0; i < (int)strlen((*str)); i++) {

        // Игнорирование \n последнего числа в строке матрицы
        if ((*str)[i] == '\n' && i == (int)strlen(*str) - 1) continue;

        // Проверка на то, что минус может стоять только на 1-ой позиции и при этом он не может быть единственным символом в строке
        if ((*str)[i] == '-' && i == 0) {
            if (strlen((*str)) > 1) continue;
            else return false;
        } else if ((*str)[i] == '-') return false;

        // Проверка на наличие других обрабатываемых исключений (см коментарии из блока в if { ... })
        if (!(('0' <= (*str)[i]) && ((*str)[i] <= '9' || (*str)[i] == '.'))) {

            // проверка на то, что точка может встречаться только 1 раз
            if ((*str)[i] == '.') {
                dot_number += 1;
                if (dot_number > 1) {
                    return false;
                }
            // Проверка что все остальные символы это числа
            } else if (!('0' <= (*str)[i] && (*str)[i] <= '9')) {
                return false;
            }
        }
    }
    return true;
}

// Проверка на то, что набор символов является целым положительным числом (Просто проверяем что в строке только числа)
bool check_uint(char *str[]) {
    for (int i = 0; i < (int)strlen(*str); i++) {
        if (!('0' <= (*str)[i] && (*str)[i] <= '9')) {
            return false;
        }
    }
    return true;
}

// Функция преобразовывающая строку в unsigned int
unsigned int get_uint(char *str[]) {
    int power_of_ten = 0;
    int index = strlen(*str);
    unsigned int value = 0;
    // Пройдём по всем элементам полученной строки и переведём их в соответствующие им числовые значения
    while (index) {
        // Прибавляем к value значение каждого символа домноженое на его степень 10
        value += (pow(10, power_of_ten)) * (int)((*str)[index - 1] - '0'); 
        power_of_ten += 1;
        index -= 1;
    }
    return value;
}

// Функция которая считает характеристику для кольца порядка K в матрице, левый верхний угол которого находится по координатам start_string_index start_column_index
// аргумент height нужен для вычисления индекса элемента в одномерном массиве
long double get_characteristic(double *matrix, int start_column_index, int start_string_index, int height, int K) {
    // Задаём характеритику 1, так как потом будем считать произведение
    long double characteristic = 1;
    // Выщитывам количество элементов в конце (Это просто площадь квадрата без середины, то есть сначала считаем K во 2 степени, вычитаем из него (К-1) во 2 степени)
    int number_of_elemets_in_ring = (K * K) - ((K - 2) * (K - 2));
    // Статус считывания кольца из матрицы
    int state = 0;

    // Координаты считываемого элемента
    int column_index = start_column_index;
    int string_index = start_string_index;

    // Считываем элементы кольца
    for (int i = 0; i < number_of_elemets_in_ring; i++) {
        // Считаем характеристику
        characteristic *= matrix[string_index + column_index * height];
        // В зависимости от state сдвигаемся на элемент вправо, вниз, влево или вверх по матрице
        switch (state)
        {
        // При статусе 0 двигаемся вправо
        case 0:
            column_index++;
            if (column_index == start_column_index + K - 1) {
                // Когда достигаем правого верхнего угла кольца, переключаем статус
                state = 1;
            }
            break;
        // При статусе 1 двигаемся вниз
        case 1:
            string_index++;
            if (string_index == start_string_index + K - 1) {
                // Когда достигаем правого нижнего угла кольца, переключаем статус
                state = 2;
            }
            break;
        // При статусе 2 двигаемся влево
        case 2:
            column_index--;
            if (column_index == start_column_index) {
                // Когда достигаем левого нижнего угла кольца, переключаем статус
                state = 3;
            }
            break;
        // При статусе 3 двигаемся вверх
        case 3:
            string_index--;
            if (string_index == start_string_index - 1) {
                // Когда достигаем левого верхнего угла кольца, не доходя до него на 1 шаг, переключаем статус
                state = 4;
            }
        default:
            break;
        }
    }

    return characteristic;
}

// Функция совершает поворот кольца
void turn_the_ring(double **matrix, int start_column_index, int start_string_index, int K, int L, int height) {

    // Выщитывам количество элементов в конце (Это просто площадь квадрата без середины, то есть сначала считаем K во 2 степени, вычитаем из него (К-1) во 2 степени)
    int number_of_elemets_in_ring = (K * K) - ((K - 2) * (K - 2));
    // Создаём массив под значения из кольца
    double ring_values[number_of_elemets_in_ring];
    // Статус чтения значений кольца
    int state_of_read_ring = 0;

    int column_index = start_column_index;
    int string_index = start_string_index;
    for (int i = 0; i < number_of_elemets_in_ring; i++) {
        // Считываем значение из кольцы в матрице
        ring_values[i] = (*matrix)[string_index + column_index * height];
        // В зависимости от state_of_read_ring сдвигаемся на элемент вправо, вниз, влево или вверх по матрице
        switch (state_of_read_ring)
        {
        // При статусе 0 двигаемся вправо
        case 0:
            column_index++;
            if (column_index == start_column_index + K - 1) {
                // Когда достигаем правого верхнего угла кольца, переключаем статус
                state_of_read_ring = 1;
            }
            break;
        // При статусе 1 двигаемся вниз
        case 1:
            string_index++;
            if (string_index == start_string_index + K - 1) {
                // Когда достигаем правого нижнего угла кольца, переключаем статус
                state_of_read_ring = 2;
            }
            break;
        // При статусе 2 двигаемся влево
        case 2:
            column_index--;
            if (column_index == start_column_index) {
                // Когда достигаем левого нижнего угла кольца, переключаем статус
                state_of_read_ring = 3;
            }
            break;
        // При статусе 3 двигаемся вверх
        case 3:
            string_index--;
            if (string_index == start_string_index - 1) {
                // Когда достигаем левого верхнего угла кольца, не доходя до него на 1 шаг, переключаем статус
                state_of_read_ring = 4;
            }
        default:
            break;
        }
    }

    // Совершаем поворот кольца
    // L раз сдвигаем все элементы в лево, а первый отправляем в конец массива
    for (int i = 0; i < L; i++) {
        double buf_value = ring_values[0];
        for (int value_index = 0; value_index < number_of_elemets_in_ring - 1; value_index++) {
            ring_values[value_index] = ring_values[value_index + 1];
            if (value_index == number_of_elemets_in_ring - 2) {
                ring_values[number_of_elemets_in_ring - 1] = buf_value;
            }
        }
    }

    // Устанавливаем координаты в стартовую позицию для записи новых значений в матрицу
    column_index = start_column_index;
    string_index = start_string_index;
    int turned_index_of_ring_element = 0;
    int state_of_write_ring = 0;

    for (int i = 0; i < number_of_elemets_in_ring; i++) {
        //Записываем сдвинутые значения на старые позиции
        //То есть на индексыв, в которые мы считали значения, теперь лежат новые значения
        (*matrix)[string_index + column_index * height] = ring_values[turned_index_of_ring_element];
        turned_index_of_ring_element++;
        //В зависимости от state_of_write_ring сдвигаемся на элемент вправо, вниз, влево или вверх по матрице
        switch (state_of_write_ring)
        {
        // При статусе 0 двигаемся вправо
        case 0:
            column_index++;
            if (column_index == start_column_index + K - 1) {
                //Когда достигаем правого верхнего угла кольца, переключаем статус
                state_of_write_ring = 1;
            }
            break;
        // При статусе 1 двигаемся вниз
        case 1:
            string_index++;
            if (string_index == start_string_index + K - 1) {
                //Когда достигаем правого нижнего угла кольца, переключаем статус
                state_of_write_ring = 2;
            }
            break;
        // При статусе 2 двигаемся влево
        case 2:
            column_index--;
            if (column_index == start_column_index) {
                //Когда достигаем левого нижнего угла кольца, переключаем статус
                state_of_write_ring = 3;
            }
            break;
        // При статусе 3 двигаемся вверх
        case 3:
            string_index--;
            if (string_index == start_string_index - 1) {
                //Когда достигаем левого верхнего угла кольца, не доходя до него на 1 шаг, переключаем статус
                state_of_write_ring = 4;
            }
        default:
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    // Задаём переменные
    int height = 0;
    int width = 0;
    int K = 0;
    int L = 0;
    
    // Проверка количесвта введённых аргументов
    if (argc < 5 || (argc == 5 && (strcmp(argv[1], "-m") == 0))) {
        printf("Ошибка: отсутствует обязательный аргумент.\n");
        exit(1);
    } 
    // Проверяем на поддерживающуюся опцию в начале ввода
    if (argv[1][0] == '-' && (strcmp(argv[1], "-m") != 0)) {
        printf("Ошибка: опция '%s' не поддерживается.\n", argv[1]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        if (i == 1 && strcmp(argv[1], "-m") == 0) continue;
        // Проверка на поддерживающуюся опцию не в первом аргументе (Пример: ./programm -m -x 2 2 3 4)  эта строка найдёт -x
        else if (argv[i][0] == '-') {
            printf("Ошибка: опция '%s' не поддерживается.\n", argv[i]);
            exit(1);
        }
        // Проверка на корректность введённых чисел (они должныбыть положительными целыми)
        else if (check_uint(&argv[i]) == false) {
            printf("Ошибка: '%s' не является числом.\n", argv[i]);
            exit(1);
        }
    }
    
    // Считывание размера матрицы и параметров K и L
    if (strcmp(argv[1], "-m") == 0) {
        height = get_uint(&argv[2]);
        width = get_uint(&argv[3]);
        K = get_uint(&argv[4]);
        L = get_uint(&argv[5]);
    } else {
        height = get_uint(&argv[1]);
        width = get_uint(&argv[2]);
        K = get_uint(&argv[3]);
        L = get_uint(&argv[4]);
    }

    // Считаем количество элементов в матрице
    int number_of_elemets = height * width;
    // Выделяем память под нашу матрицу в куче и заполняем её нулями
    double *matrix = calloc(number_of_elemets, sizeof(double));

    // Эти значения показывают максимально возможные координаты левого верхнего угла кольца со стороной K в матрице
    // Если одно из значений координат будет больше этих, то кольцо вылезет за пределы матрицы
    int max_string_index = height - K;
    int max_column_index = width - K;

    // Если эти значения отрицательны, то кольцо со стороной K просто не может влезть в матрицу (Пример: матрица 3x4  кольцо 4x4)
    if (max_column_index < 0 || max_string_index < 0) {
        printf("Ошибка: в матрице нет колец порядка %d", K);
        exit(1);
    }
    
    // Создание строк
    // если был передан -m то начинаем считывать строки
    if (strcmp(argv[1], "-m") == 0) {
        printf("Введите строки матрицы:\n");
        int index_of_string = 0;
        for (int i = 0; i < height; i++) {
            // Создаём массив, в который считаем введённую строку
            char input_str[240];

            // Получаем строку
            fgets(input_str, 240, stdin);
            // Чистим буфер для корректного считывания следующих строк
            fflush(stdin);

            // Разбиваем введённую строку по пробелам и кладём каждую такую подстроку в value
            char *value = strtok(input_str, " ");
            // Номер числа в строке
            int value_index = 0;

            // Пока в value есть значение проверяем его на то что оно число нужного формата и записываем в матрицу
            while (value != NULL) {
                if (сheck_double(&value)) {
                    // Координаты элемента в одномерном массиве содержащем столбцы матрицы рассчитываются по формуле:  (индекс столбца * высота матрицы) + позиция элемента в столбце(индекс строки)
                    // В данном месте value_index означает позицию элемента в считанной строке, что эквивалентно индексу столбца
                    matrix[value_index * height + index_of_string] = strtof(value, NULL);
                    value_index++;
                    value = strtok(NULL, " ");
                }
                // Выводи ошибку если value не число
                else {
                    printf("Ошибка: '%s' не является числом.\n", value);
                    exit(1);
                }
            }
            if (value_index < width) {
                printf("Ошибка: введено слишком мало значений(заполнены не все ячейки матрицы в строке).\n");
                exit(1);
            } else if (value_index > width) {
                printf("Ошибка: введено слишком много значений(строка вышла за пределы матрицы).\n");
                exit(1);
            }
            index_of_string++;
        }   

    // Иначе заполняем строки рандомными значениями
    } else {
        // Задание опорного элемента для функции rand()
        srand(time(NULL));
        for (int i = 0; i < number_of_elemets; i++) {
            // Получение рандомного double в диапазоне от 0 до 10
            double random_value = (double)rand() / RAND_MAX + (float)(rand() % 10);

            // Запись полученного значения в матрице и вывод его в консоль для демонстрации исходной матрицы
            matrix[i] = random_value;
        }
    }

    // Вывод полученной исходной матрицы
    printf("\n");
    printf("Исходная матрица:\n");
    for (int string_ind = 0; string_ind < height; string_ind++) {
        for (int column_ind = 0; column_ind < width; column_ind++) {
            printf("%f ", matrix[column_ind * height + string_ind]);
        }
        printf("\n");
    }
    printf("\n");

    // Координаты проверяемого элемента
    int string_index = 0;
    int column_index = 0;

    // Координаты для записи найденного левого верхнего угла кольца с минимальной характеристикой
    int result_string_index = -1;
    int result_column_index = -1;

    long double min_characteristic = 0;
    // Флаг для установления первого значения в min_characteristic
    bool characteristic_not_set = true;

    // Пока в столбце есть элементы, которые могут стать левым верзним углом кольца
    while (column_index <= max_column_index) {
        // Перебираем эти элементы в столбце
        while (string_index <= max_string_index)
        {
            // Считаем для каждого характеристику
            double charavteristic = get_characteristic(matrix, column_index, string_index, height, K);
            // Если она меньше чем раньше найденная минимальная или это первая посчитаная характеристика, то меняем найденный результат
            if ((min_characteristic > charavteristic) || characteristic_not_set) {
            	characteristic_not_set = false;
                min_characteristic = charavteristic;
                result_column_index = column_index;
                result_string_index = string_index;
            }
            // Переходи к следующему элементу в столбце
            string_index++;
        }
        // Переходи к следующему столбцу
        string_index = 0;
        column_index++;
    }

    // Поворачиваем найденное кольцо с минимальной характеристикой
    turn_the_ring(&matrix, result_column_index, result_string_index, K, L, height);

    // Вывод результата в консоль
    printf("Результат:\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%f ", matrix[j * height + i]);
        }
        printf("\n");
    }
    
    // Очистка памяти
    free(matrix);
    return 0;
}
