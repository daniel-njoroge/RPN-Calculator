#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STACK_SIZE 100

// Stack structure
typedef struct {
    int top;
    double items[STACK_SIZE];
} Stack;

// Function prototypes
void push(Stack *s, double value);
double pop(Stack *s);
double evaluatePostfix(char *expression);
int precedence(char operator);
void infixToPostfix(char *infix, char *postfix);

int main() {
    char infix[100], postfix[100];

    printf("Enter an infix expression: ");
    fgets(infix, sizeof(infix), stdin);

    // Remove trailing newline character
    infix[strcspn(infix, "\n")] = '\0';

    infixToPostfix(infix, postfix);

    printf("Postfix expression: %s\n", postfix);

    double result = evaluatePostfix(postfix);
    printf("Result: %.2f\n", result);

    return 0;
}

// Function to push an element onto the stack
void push(Stack *s, double value) {
    if (s->top == STACK_SIZE - 1) {
        printf("Stack overflow\n");
        exit(EXIT_FAILURE);
    }
    s->items[++(s->top)] = value;
}

// Function to pop an element from the stack
double pop(Stack *s) {
    if (s->top == -1) {
        printf("Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return s->items[(s->top)--];
}

// Function to evaluate postfix expression
double evaluatePostfix(char *expression) {
    Stack stack;
    stack.top = -1;

    char *token = strtok(expression, " ");
    while (token != NULL) {
        if (isdigit(*token)) {
            push(&stack, atof(token));
        } else {
            double operand2 = pop(&stack);
            double operand1 = pop(&stack);
            switch (*token) {
                case '+':
                    push(&stack, operand1 + operand2);
                    break;
                case '-':
                    push(&stack, operand1 - operand2);
                    break;
                case '*':
                    push(&stack, operand1 * operand2);
                    break;
                case '/':
                    push(&stack, operand1 / operand2);
                    break;
                case '%':
                    push(&stack, (int)operand1 % (int)operand2);
                    break;
                default:
                    printf("Invalid operator\n");
                    exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, " ");
    }
    return pop(&stack);
}

// Function to determine operator precedence
int precedence(char operator) {
    switch (operator) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        default:
            return 0;
    }
}

// Function to convert infix expression to postfix
void infixToPostfix(char *infix, char *postfix) {
    Stack stack;
    stack.top = -1;

    char *ptr = infix;
    char *output = postfix;

    while (*ptr != '\0') {
        if (isdigit(*ptr)) {
            while (isdigit(*ptr) || *ptr == '.') {
                *(output++) = *(ptr++);
            }
            *(output++) = ' ';
        } else if (*ptr == '(') {
            push(&stack, *ptr);
            ptr++;
        } else if (*ptr == ')') {
            while (stack.top != -1 && stack.items[stack.top] != '(') {
                *(output++) = pop(&stack);
                *(output++) = ' ';
            }
            if (stack.top == -1) {
                printf("Mismatched parentheses\n");
                exit(EXIT_FAILURE);
            }
            pop(&stack); // Discard '('
            ptr++;
        } else if (*ptr == '+' || *ptr == '-' || *ptr == '*' || *ptr == '/' || *ptr == '%') {
            while (stack.top != -1 && precedence(stack.items[stack.top]) >= precedence(*ptr)) {
                *(output++) = pop(&stack);
                *(output++) = ' ';
            }
            push(&stack, *ptr);
            ptr++;
        } else if (*ptr == ' ') {
            ptr++; // Skip whitespaces
        } else {
            printf("Invalid character\n");
            exit(EXIT_FAILURE);
        }
    }

    // Pop remaining operators from stack
    while (stack.top != -1) {
        if (stack.items[stack.top] == '(') {
            printf("Mismatched parentheses\n");
            exit(EXIT_FAILURE);
        }
        *(output++) = pop(&stack);
        *(output++) = ' ';
    }

    // Null terminate the postfix string
    *(--output) = '\0';
}

