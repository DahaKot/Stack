#include <stdio.h>
#include <stdlib.h>

#define ASSERT_OK(ER, line)                                                         \
switch(ER) {                                                                        \
    case (NOT_ENOUGH_MEMORY): {                                                     \
        printf("An error in line: %d\n", __LINE__);                                     \
        printf("There is not enough memory. Sorry :(\n");                           \
        break;                                                                      \
    }                                                                               \
    case (BAD_PTR): {                                                               \
        printf("An error in line: %d\n", line);                                     \
        printf("WTF?! Bad pointer is used.\n");                                     \
        break;                                                                      \
    }                                                                               \
    case (NO_DATA): {                                                               \
        printf("An error in line: %d\n", line);                                     \
        printf("NULL pointer used as pointer on stack's data\n");                   \
        break;                                                                      \
    }                                                                               \
    case (DATA_CANARY_1): {                                                         \
        printf("An error in line: %d\n", line);                                     \
        printf("You pry in unavailable place (no rights to be there)\n");           \
        break;                                                                      \
    }                                                                               \
    case (CANARY_1): {                                                              \
        printf("An error in line: %d\n", line);                                     \
        printf("You pry in unavailable place (no rights to be there)\n");           \
        break;                                                                      \
    }                                                                               \
    case (CANARY_2): {                                                              \
        printf("An error in line: %d\n", line);                                     \
        printf("You pry in unavailable place (no rights to be there)\n");           \
        break;                                                                      \
    }                                                                               \
    case (EMPTY): {                                                                 \
        printf("An error in line: %d\n", line);                                     \
        printf("Trying to find elemenet in empty stack\n");                         \
        break;                                                                      \
    }                                                                               \
     case (WRONG_HASH): {                                                           \
        printf("An error in line: %d\n", line);                                     \
        printf("Wrong hash\n");                                                     \
        break;                                                                      \
    }                                                                               \
    case (ALL_RIGHT): {                                                             \
        break;                                                                      \
    }                                                                               \
    default: {                                                                      \
        printf("An error in line: %d\n", line);                                     \
        printf("Dunno what it is, dude");                                           \
    }                                                                               \
}

typedef double data_t;
const int one = 1;
const data_t sense_of_life = 42;
const data_t Ada = 5;
const data_t Alice = 7;

enum Err_code {
    ALL_RIGHT = 0,
    BAD_PTR = 1,
    NOT_ENOUGH_MEMORY = 2,
    INVALID_LEN = 3,
    EMPTY = 4,
    NO_DATA = 5,
    DATA_CANARY_1 = 6,
    UNDEFINED_ERR = 7,
    CANARY_1 = 8,
    CANARY_2 = 9,
    WRONG_HASH = 10
};

struct K_Stack {
    data_t canary_1;
    data_t *data;                                                       // pointer on the first element
    size_t len;                                                         // number of elements
    data_t canary_2;
    double hash;
};

double Hashing (const struct K_Stack *stack);                                 //counts Hash for stack
int Stack_Ok (const struct K_Stack *stack); //const?                          //checks if stack is correct
int Stack_Construct (struct K_Stack **new_one);                         //creates new stack and put pointer on it *new_one
void Stack_Destruct (struct K_Stack *old_one);                          //kills old_one
int Stack_Resize (struct K_Stack **stack, size_t Len);                  //change len of the stack
int Stack_Push (struct K_Stack **stack, data_t value);                  //pushes value to stack
data_t Stack_Pop (struct K_Stack **stack);                              //returns the last element from stack
data_t Stack_Peek (struct K_Stack *stack);                              //return the last element without poping it

int main() {
    struct K_Stack *test_stack = NULL;

    switch (Stack_Construct(&test_stack)) {
        case (ALL_RIGHT):           break;


        case (NOT_ENOUGH_MEMORY):   return NOT_ENOUGH_MEMORY;
        default:                    return UNDEFINED_ERR;
    }

    Stack_Push(&test_stack, 10);

    printf("Element: %lg\n", Stack_Peek(test_stack));
    data_t number = 0;
    printf("Enter the number for stack:\n");
    scanf("%lg", &number);
    Stack_Push(&test_stack, number);
    printf("%lg\n", Stack_Pop(&test_stack));
    printf("%lg\n", Stack_Pop(&test_stack));

    printf("From empty stack: %lg\n", Stack_Peek(test_stack));

    Stack_Destruct(test_stack);
    free(test_stack);

    return 0;
}

double Hashing (const struct K_Stack *stack) {//what about data_t instead of double?
    double result = stack->len*2 - Ada;
    int i = 0;
    for ( ; i < stack->len + one; i++) {
        result += (double) (stack->data[i]*2 - Alice*Alice);
    }

    return result;
}

int Stack_Ok (const struct K_Stack *stack) {
    if (stack != NULL) {
        if (stack->canary_1 == Ada) {
            if (stack->canary_2 == Alice) {
                if (stack->data != NULL) {
                    if (stack->len >= 0 && stack->len % 1 == 0) {           //if len is >= 0 and integer
                        if (stack->data[0] == sense_of_life) {              //we use data[0] as a canary
                            if (stack->hash == Hashing(stack)) {
                                return ALL_RIGHT;
                            }
                            else {
                                return WRONG_HASH;
                            }
                        }
                        else {
                            return DATA_CANARY_1;
                        }
                    }
                    else {
                        return INVALID_LEN;
                    }
                }
                else {
                    return NO_DATA;
                }
            }
            else {
                return CANARY_1;
            }
        }
        else {
            return CANARY_2;
        }
    }
    else {
        return BAD_PTR;
    }
}

int Stack_Construct (struct K_Stack **new_one) {
    *new_one = calloc(one, sizeof(struct K_Stack));
    if (*new_one == NULL) {
        ASSERT_OK(NOT_ENOUGH_MEMORY, __LINE__);
        return NOT_ENOUGH_MEMORY;
    }

    (*new_one)->len = 0;
    (*new_one)->data = calloc(1, sizeof(data_t));
    if ((*new_one)->data == NULL) {
        ASSERT_OK(NOT_ENOUGH_MEMORY, __LINE__);
        return NOT_ENOUGH_MEMORY;
    }
    (*new_one)->data[0] = sense_of_life;                    //use data[0] element as a canary
    (*new_one)->canary_1 = Ada;                             //initializing canaries
    (*new_one)->canary_2 = Alice;

    double Hash = Hashing((*new_one));

    (*new_one)->hash = Hash;

    return ALL_RIGHT;
}

void Stack_Destruct (struct K_Stack **old_one) {
    if (old_one != NULL) {
        free(old_one->data);
    }
    free(old_one);
    *old_one = NULL;
}

int Stack_Resize (struct K_Stack **stack, size_t Len) {
    int err_code = Stack_Ok(*stack);
    if (err_code != ALL_RIGHT) {
        ASSERT_OK(err_code, __LINE__);
        Stack_Destruct(*stack);
        return err_code;
    }

    struct K_Stack *new_s = NULL;
    Stack_Construct(&new_s);

    if (Len == 0) {                                                     //if asked about empty stack - returns it
        Stack_Destruct(*stack);
        *stack = new_s;

        return ALL_RIGHT;
    }

    new_s->len = Len;
    free(new_s->data);
    new_s->data = calloc(Len + one, sizeof(data_t));                    //Len - amount of elements in new_s and one for data[0]
    if (new_s->data == NULL) {
        ASSERT_OK(NOT_ENOUGH_MEMORY, __LINE__);
        return NOT_ENOUGH_MEMORY;
    }
    new_s->hash = Hashing(new_s);
    realloc();instead
    int i = 0;                                                          //copies elements from stack(old) to new_s
    new_s->data[i] = (*stack)->data[i];                                 //firstly and always data[0]
    for (++i; i < (*stack)->len; i++) {                                 //then elements from data[1] to data[len-1]
        new_s->data[i] = (*stack)->data[i];                             //(resizing is always on 1 element: + or -)
    }                                                                   //so if Len == len - 1 - we've done
    if (Len > (*stack)->len) {                                          //else copy the last element and then
        new_s->data[i] = (*stack)->data[i];                             //in Push() fill the very last element with value
    }
    new_s->hash = Hashing(new_s);

    err_code = Stack_Ok(new_s);
    if (err_code != ALL_RIGHT) {
        ASSERT_OK(err_code, __LINE__);
        Stack_Destruct(*stack);
        return err_code;
    }

    Stack_Destruct(*stack);
    *stack = new_s;

    return ALL_RIGHT;
}

int Stack_Push (struct K_Stack **stack, data_t value) {
    int err_code = Stack_Ok(*stack);
    if (err_code != ALL_RIGHT) {
        ASSERT_OK(err_code, __LINE__);
        Stack_Destruct(*stack);
        return err_code;
    }

    Stack_Resize(stack, (*stack)->len + one);                           //stack increases here
    (*stack)->data[(*stack)->len] = value;                              //the last element = value
    (*stack)->hash = Hashing(*stack);

    err_code = Stack_Ok(*stack);
    if (err_code != ALL_RIGHT) {
        ASSERT_OK(err_code, __LINE__);
        Stack_Destruct(*stack);
        return err_code;
    }

    return ALL_RIGHT;
}

data_t Stack_Pop (struct K_Stack **stack) {
    int err_code = Stack_Ok(*stack);
    if (err_code != ALL_RIGHT) {
        ASSERT_OK(err_code, __LINE__);
        Stack_Destruct(*stack);
        return (data_t) err_code;
    }

    if ((*stack)->len == 0) {
        ASSERT_OK(EMPTY, __LINE__);
        Stack_Destruct(*stack);
        return (data_t) EMPTY;
    }

    data_t last_elem = (*stack)->data[(*stack)->len];                       //take the last element before we
    Stack_Resize(stack, (*stack)->len - one);                               //change the stack
    (*stack)->hash = Hashing(*stack);

    err_code = Stack_Ok(*stack);
    if (err_code != ALL_RIGHT) {
        ASSERT_OK(err_code, __LINE__);
        Stack_Destruct(*stack);
        return (data_t) err_code;
    }

    return last_elem;
}

data_t Stack_Peek(struct K_Stack *stack) {
    int err_code = Stack_Ok(stack);
    if (err_code != ALL_RIGHT) {
        ASSERT_OK(err_code, __LINE__);
        Stack_Destruct(stack);
        return (data_t) err_code;
    }

    if (stack->len == 0) {
        ASSERT_OK(EMPTY, __LINE__);
        Stack_Destruct(stack);
        return (data_t) EMPTY;
    }

    return (stack->data[stack->len]);
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