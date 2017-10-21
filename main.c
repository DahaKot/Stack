#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

//set LOG equal to "//" if you don't need it
#define LOG

typedef double data_t;
const int one = 1;
const size_t LEN = 10;
const data_t sense_of_life = 42;
const data_t Ada = 5;
const data_t Alice = 7;

enum Err_code {
    ALL_RIGHT = 0,
    CREATING_A_STACK = 1,
    NOT_ENOUGH_MEMORY = 2,
    STACK_CREATED = 3,
    EMPTY = 4,
    ERR = 5,
    KILLING_STACK = 6,
    STACK_KILLED = 7,
    RESIZING_STACK = 8,
    STACK_RESIZED = 9,
    PUSHING_STACK = 10,
    STACK_PUSHED = 11,
    POPING_STACK = 12,
    STACK_POPED = 13,
    PEEKING_STACK = 14,
    BAD_PTR = 15,
    INVALID_LEN = 16
};

struct K_Stack {
    data_t canary_1;
    data_t *data;                                                       // pointer on the first element
    size_t len;                                                         // number of elements
    size_t current_size;
    data_t canary_2;
    double hash;
};

double Hashing (const struct K_Stack *stack);                       //counts Hash for the stack
int Stack_Ok (const struct K_Stack *stack);                         //checks if the stack is correct
void Stack_Dump (const struct K_Stack *stack, int line);            //prints current condition of the stack
struct K_Stack *Stack_Construct ();                                 //creates new stack and put pointer on it *new_one
void Stack_Destruct (struct K_Stack **old_one);                     //kills old_one
int Stack_Resize (struct K_Stack **stack, size_t Len);              //change len of the stack to Len
int Stack_Push (struct K_Stack **stack, data_t value);              //pushes value to stack
data_t Stack_Pop (struct K_Stack **stack);                          //returns the last element from stack
data_t Stack_Peek (struct K_Stack *stack);                          //return the last element without poping it
void Log_write (int err_code, int line);                            //print in log file

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

double Hashing (const struct K_Stack *stack) {
    double result = stack->len*2 - Ada;
    int i = 0;
    for ( ; i < stack->len + one; i++) {
        result += (double) (stack->data[i]*2 - Alice*Alice);
    }

    return result;
}

int Stack_Ok (const struct K_Stack *stack) {
    if (stack != NULL && stack->canary_1 == Ada && stack->canary_2 == Alice &&
        stack->data != NULL && stack->len >= 0 && stack->len % 1 == 0   &&
        stack->data[0] == sense_of_life && stack->hash == Hashing(stack)) {
        return ALL_RIGHT;
    }
    else {
        return ERR;
    }
}

void Stack_Dump (const struct K_Stack *stack, int line) {
    printf("Stack on %p has a problem in %d line\n", stack, line);
    if (stack == NULL) {
        printf(" //NULL pointer?)\n");
        LOG Log_write(BAD_PTR, __LINE__);
        return ;
    }

    printf("Number of elements: %d\n", stack->len);
    if (stack->len < 0 || stack->len % 1 != 0) {
        printf(" //Weird length!\n");
        LOG Log_write(INVALID_LEN, __LINE__);
        return ;
    }

    printf("Data is on: %p\n", stack->data);
    if (stack->data == NULL) {
        printf(" //NULL pointer?)\n");
        LOG Log_write(BAD_PTR, __LINE__);
        return ;
    }
    printf("Main numbers: %lg, %lg, %lg\n", stack->canary_1, stack->canary_2, stack->data[0]);
    if (stack->canary_1 != Ada || stack->canary_2 != Alice || stack->data[0] != sense_of_life) {
        printf(" //Hm... seems smth is wrong here\n");
        LOG Log_write(ERR, __LINE__);
        return ;
    }

    printf("Size of data's array: %d\n", stack->current_size);
    printf("Elements:\n");
    int i = 0;
    while (i < stack->len) {
        printf("%d. %lg\n", i, stack->data[i]);
        i++;
    }
    printf("===================================================================\n");
}

struct K_Stack *Stack_Construct () {
    LOG Log_write(CREATING_A_STACK, __LINE__);

    struct K_Stack *new_one = calloc(one, sizeof(struct K_Stack));
    if (new_one == NULL) {
        LOG Log_write(NOT_ENOUGH_MEMORY, __LINE__);
        return (void *) NOT_ENOUGH_MEMORY;
    }

    (new_one)->len = 0;
    (new_one)->data = calloc(LEN, sizeof(data_t));
    if ((new_one)->data == NULL) {
        LOG Log_write(NOT_ENOUGH_MEMORY, __LINE__);
        return (void *) NOT_ENOUGH_MEMORY;
    }
    (new_one)->data[0] = sense_of_life;                    //use data[0] element as a canary
    (new_one)->canary_1 = Ada;                             //initializing canaries
    (new_one)->canary_2 = Alice;
    (new_one)->current_size = LEN;

    double Hash = Hashing((new_one));

    (new_one)->hash = Hash;

    LOG Log_write(STACK_CREATED, __LINE__);
    return new_one;
}

void Stack_Destruct (struct K_Stack **old_one) {
    LOG Log_write(KILLING_STACK, __LINE__);

    if (*old_one != NULL) {
        free((*old_one)->data);
    }
    free(*old_one);
    *old_one = NULL;

    LOG Log_write(STACK_KILLED, __LINE__);
}

int Stack_Resize (struct K_Stack **stack, size_t Len) {
    LOG Log_write(RESIZING_STACK, __LINE__);

    if (Stack_Ok(*stack) != ALL_RIGHT) {
        Stack_Dump(*stack, __LINE__);
        return ERR;
    }

    data_t *new_data = NULL;
    if (Len >= (*stack)->current_size - one) {
        new_data = calloc((*stack)->current_size * 2, sizeof(data_t));
        (*stack)->current_size *= 2;
    }
    else {
        new_data = calloc((*stack)->current_size, sizeof(data_t));
    }

    if (new_data == NULL) {
        LOG Log_write(NOT_ENOUGH_MEMORY, __LINE__);
        return NOT_ENOUGH_MEMORY;
    }

    int i = 0;
    while (i < (*stack)->len + one) {
        new_data[i] = (*stack)->data[i];
        i++;
    }
    (*stack)->data = new_data;
    (*stack)->len = Len;
    (*stack)->hash = Hashing(*stack);

    /*if (Len >= (*stack)->current_size - 1) {
        if (realloc((*stack)->data, (*stack)->current_size*2) == NULL) {
            ASSERT_OK(NOT_ENOUGH_MEMORY, __LINE__);
            return NOT_ENOUGH_MEMORY;
        }
        (*stack)->current_size *= 2;
        (*stack)->len = Len;
        (*stack)->hash = Hashing(*stack);
    }
    else {
        if (realloc((*stack)->data, Len) == NULL) {
            ASSERT_OK(NOT_ENOUGH_MEMORY, __LINE__);
            return NOT_ENOUGH_MEMORY;
        }
        (*stack)->len = Len;
        (*stack)->hash = Hashing(*stack);
    }*/

    if (Stack_Ok(*stack) != ALL_RIGHT) {
        Stack_Dump(*stack, __LINE__);
        return ERR;
    }

    LOG Log_write(STACK_RESIZED, __LINE__);
    return ALL_RIGHT;
}

int Stack_Push (struct K_Stack **stack, data_t value) {
    LOG Log_write(PUSHING_STACK, __LINE__);

    if (Stack_Ok(*stack) != ALL_RIGHT) {
        Stack_Dump(*stack, __LINE__);
        return ERR;
    }

    if (Stack_Resize(stack, (*stack)->len + one) != ALL_RIGHT) {
        return ERR;
    }
    (*stack)->data[(*stack)->len] = value;                              //the last element = value
    (*stack)->hash = Hashing(*stack);

    if (Stack_Ok(*stack) != ALL_RIGHT) {
        Stack_Dump(*stack, __LINE__);
        return ERR;
    }

    LOG Log_write(STACK_PUSHED, __LINE__);
    return ALL_RIGHT;
}

data_t Stack_Pop (struct K_Stack **stack) {
    LOG Log_write(POPING_STACK, __LINE__);

    if (Stack_Ok(*stack) != ALL_RIGHT) {
        Stack_Dump(*stack, __LINE__);
        return ERR;
    }

    if ((*stack)->len == 0) {
        LOG Log_write(EMPTY, __LINE__);
        return (data_t) EMPTY;
    }

    data_t last_elem = (*stack)->data[(*stack)->len];                       //take the last element before we
    if (Stack_Resize(stack, (*stack)->len - one) != ALL_RIGHT) {
        return ERR;
    }
    (*stack)->hash = Hashing(*stack);

    if (Stack_Ok(*stack) != ALL_RIGHT) {
        Stack_Dump(*stack, __LINE__);
        return ERR;
    }

    LOG Log_write(STACK_POPED, __LINE__);
    return last_elem;
}

data_t Stack_Peek(struct K_Stack *stack) {
    LOG Log_write(PEEKING_STACK, __LINE__);
    if (Stack_Ok(stack) != ALL_RIGHT) {
        Stack_Dump(stack, __LINE__);
        return ERR;
    }

    if ((stack)->len == 0) {
        LOG Log_write(EMPTY, __LINE__);
        return (data_t) EMPTY;
    }

    return (stack->data[stack->len]);
}

void Log_write (int err_code, int line) {
    FILE *logs;
    logs = fopen("Log.txt", "a+");

    static int start = 0;
    if (start == 0) {
        time_t current_time;
        time(&current_time);

        fprintf(logs, "Program started: %s", ctime(&current_time));
        start++;
    }

    switch(err_code) {
        case (CREATING_A_STACK):
            fprintf(logs, "Creating a stack\n");
            break;
        case (NOT_ENOUGH_MEMORY):
            fprintf(logs, "Can't alloc memory :C\n");
            break;
        case (STACK_CREATED):
            fprintf(logs, "New stack is created\n");
            break;
        case (STACK_KILLED):
            fprintf(logs, "You've killed him! He had two pretty children: Sasha and Abdurahman ! T_T\n");
            break;
        case (STACK_POPED):
            fprintf(logs, "Stack is poped\n");
            break;
        case (STACK_PUSHED):
            fprintf(logs, "Stack is pushed\n");
            break;
        case (EMPTY):
            fprintf(logs, "Stack is empty like your mind, dude\n");
            break;
        case (ERR):
            fprintf(logs, "Canaries are dead\n");
            break;
        case (KILLING_STACK):
            fprintf(logs, "Killer is called\n");
            break;
        case (STACK_RESIZED):
            fprintf(logs, "Stack had been resized\n");
            break;
        case (PUSHING_STACK):
            fprintf(logs, "Pushing in stack\n");
            break;
        case (POPING_STACK):
            fprintf(logs, "Poping stack\n");
            break;
        case (PEEKING_STACK):
            fprintf(logs, "Taking last element\n");
            break;
        case (BAD_PTR):
            fprintf(logs, "You use invalid pointer\n");
            break;
        case (INVALID_LEN):
            fprintf(logs, "Pi-len\n");
            break;
        default: fprintf(logs, "Who knows, what's happening right now?)\n");
    }

    fclose(logs);
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