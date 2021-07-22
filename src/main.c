#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void interpret(char* buf, int* i, int** stack, int* stack_len, int* ptr);

/**
 * A small BrainF*ck interperter  
 **/

int main(int argc, char** argv) {

    // check for the correct file extension and correct number of
    // command line arguments  
    if (argc != 2) {
        perror("Wrong number of command line arguments\n");
        return 0;
    } else {
        const char *dot = strrchr(argv[1], '.');
        if(!dot || dot == argv[1] || strcmp(dot + 1, "bf") != 0) {
            perror("Wrong file extension\n");
            return 0;
        }
    }

    // read all the contents of the file dynamically, storing it in a buffer
    int c;
    int buf_len = 1;
    char* buf = (char*)calloc(buf_len, sizeof(char));
    FILE *file;

    // read from a bf source file, storing the characters in a buffer 
    // the buffer dynamically allocats memory when a character is read
    // if a character is invalid, the program is terminated 
    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Could not open file\n");
    } do {
        c = fgetc(file);
        if (feof(file)) {
            break;
        } else if (c != '+' && c != '-' && c != ']' && c != '[' && c != '>' && c != '<' && c != '.' && c != ',') {
            continue;
            // printf("Invalid token: %c\n", c);
            // return 1;
        }

        buf[buf_len-1] = c;
        buf_len++;
        buf = realloc(buf, buf_len * sizeof(char));
    } while (1);

    // loop through the buffer array, interpreting characters
    int stack_len = 1;
    int* stack = (int*)calloc(stack_len, sizeof(int));
    int ptr = 0;
    for (int i = 0; i < buf_len; ++i) {
        interpret(buf, &i, &stack, &stack_len, &ptr);
    }
}

void interpret(char* buf, int* i, int** stack, int* stack_len, int* ptr) {
    if (buf[*i] == '+')
        (*stack)[*ptr]++;
    else if (buf[*i] == '-')
        (*stack)[*ptr]--;
    else if (buf[*i] == '<')
        (*ptr)--;
    else if (buf[*i] == '>') {
        // allocate memory on the stack if needed 
        if ((*stack_len)-1 == *ptr) {
            (*stack_len)++;
            *stack = realloc(*stack, *stack_len * sizeof(int));
            (*stack)[*stack_len-1] = 0;
        }
        (*ptr)++;
    }
    else if (buf[*i] == '.')
        putchar((*stack)[*ptr]);
    else if (buf[*i] == ',')
        (*stack)[*ptr] = getchar();
    else if (buf[*i] == '[') {
        int index = *i;
        // if the location in the stack is not zero
        if ((*stack)[*ptr])
        // loop until the stack location is not zero
            while(1) {
                (*i)++;
                if (buf[*i] == ']') {
                    if ((*stack)[*ptr] == 0)
                        break;
                    *i = index;
                    (*i)++;
                }
                interpret(buf, i, stack, stack_len, ptr);
            }
        // if it was zero, jump to the end of the loop
        else {
            int count = 1;
            while (count) {
                (*i)++;
                if (buf[*i] == ']')
                    count--;
                else if (buf[*i] == '[')
                    count++;
            }
        }
    }
}