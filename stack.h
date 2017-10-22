//
// Created by dasch on 22.10.2017.
//

#ifndef NEW_STACK_STACK_H
#define NEW_STACK_STACK_H

//set LOG equal to "//" if you don't need it
#define LOG

typedef double data_t;
static const int one = 1;
static const size_t LEN = 10;
static const data_t sense_of_life = 42;
static const data_t Ada = 5;
static const data_t Alice = 7;

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

#endif //NEW_STACK_STACK_H
