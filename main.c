#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include "stack.h"

int main() {
    struct K_Stack *test_stack = NULL;
    test_stack = Stack_Construct();

    test_stack->len = 1;
    Stack_Push(&test_stack, 10);
    printf("Stack: %d", test_stack->len);

    Stack_Destruct(&test_stack);
    free(test_stack);

    return 0;
}

/*Прога, которая читает с клавиатуры команды и выполняет их со стеком
 * push 5      1 0 5
 * push 5(ах)      7 1
 * mul - извлекаем два последних числа и перемножаем
 * push 4
 * push 6
 * push -50
 * mul
 * mul
 * sub
 * sqrt
 * end
 *
 * считать файл в массив scanf
 * написать atoi
 * предварительная прога-компилятор, которая переводит слова  в цифры         ассемблер
 * из кодов в названия                                                        дизассемблер
 * struct cpu {
 *   stack;
 * }
 * load()
 * run()
 * у каждой команды своя длина
 * chip8 - несуществующий процессор проще интела, можно написать с готоым эмулятором, погуглить
 * out - вывод последнего числа в стэке
 * *setpixel(x, y, color)
 * in - читает с клавы число и кладет в стэк
 * sin
 * cos
 * log
 * pop ax- выкидывает последнее число и закидывает в регистр (заводим в проессоре переменные: ах bх сх dx - регистры)
 * регистры тоже занумеровать или в массив запихнуть
 * модифицируем команду push вместе с регистрами
 * move
 * каждая ячейка в оперативке - double или все по байтам, но так муторнее
 *
 *
 *
 * в ассемблере завести массив с комадами тоже
 * */