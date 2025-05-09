#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Constants for arithmetic operators
 */
#define SUM '+'
#define SUBTRACTION '-'
#define MULTIPLICATION '*'
#define DIVISION '/'
const char *OPERATOR_SYMBOLS = "+-*/";

/**
 * Function pointer type for arithmetic operations
 * @param a First operand
 * @param b Second operand
 * @return Result of the operation
 */
typedef double (*OperationFunc)(double, double);

/**
 * Adds two numbers
 * @param a First operand
 * @param b Second operand
 * @return Sum of a and b
 */
double add(double a, double b) { return a + b; }

/**
 * Subtracts second number from first
 * @param a First operand
 * @param b Second operand
 * @return Difference of a and b
 */
double subtract(double a, double b) { return a - b; }

/**
 * Multiplies two numbers
 * @param a First operand
 * @param b Second operand
 * @return Product of a and b
 */
double multiply(double a, double b) { return a * b; }

/**
 * Divides first number by second
 * @param a First operand (dividend)
 * @param b Second operand (divisor)
 * @return Quotient of a and b, or 0 if b is 0
 */
double divide(double a, double b) { return b != 0 ? a / b : 0; }

// Function declarations
OperationFunc get_operation(char operator);
char *read_line();
void print_output(const char *num1, const char *num2, char operator, const char *result);
char *double_to_string(double value, int show_decimals);
char *remove_spaces(const char *str);

/**
 * Main function - entry point of the program
 * Reads a mathematical expression, evaluates it, and displays the result
 * @return 0 on success, 1 on error
 */
int main(void)
{
    printf("Ingresa una operación (ej: 1+2): ");
    char *query = read_line();

    char *num1 = NULL;
    char *num2 = NULL;
    char operator = '\0';
    char *result = NULL;

    // Check if input reading was successful
    if (!query)
    {
        printf("Error al leer la entrada\n");
        return 1;
    }

    char *copy = strdup(query);

    // Split the input string at the operator to extract operands
    num1 = strtok(copy, OPERATOR_SYMBOLS);
    num2 = strtok(NULL, OPERATOR_SYMBOLS);

    // Extract the operator from the original input
    operator = query[strlen(num1)];

    // Remove any whitespace from operands
    num1 = remove_spaces(num1);
    num2 = remove_spaces(num2);

    // Validate that both operands are numbers
    if (num1 == NULL || num2 == NULL || !isdigit(*num1) || !isdigit(*num2))
    {
        printf("Solo se permiten números\n");
        free(copy);
        free(query);
        return 1;
    }

    // Validate that an operator was found
    if (operator == '\0')
    {
        printf("No se encontró operador\n");
        free(copy);
        free(query);
        return 1;
    }

    // Get the appropriate function for the operation
    OperationFunc operation = get_operation(operator);

    // Validate that the operator is supported
    if (!operation)
    {
        printf("Operador no válido\n");
        free(copy);
        free(query);
        return 1;
    }

    if (operator == DIVISION && atof(num2) == 0)
    {
        print_output(num1, num2, operator, "INDEFINIDO");
        free(copy);
        free(query);
        return 1;
    }
    

    // Calculate the result and convert it to a string without decimals
    double calc_result = operation(atof(num1), atof(num2));
    result = double_to_string(calc_result, 0); // 0 = no decimals

    // Check if string conversion was successful
    if (!result)
    {
        printf("Error al convertir el resultado a string\n");
        free(copy);
        free(query);
        return 1;
    }

    // Display the formatted result
    print_output(num1, num2, operator, result);

    // Free all dynamically allocated memory
    free(result);
    free(copy);
    free(query);
    return 0;
}

/**
 * Converts a double value to a string with optional decimal places
 * @param value The double value to convert
 * @param show_decimals Flag to indicate whether to show decimal places (1) or not (0)
 * @return Dynamically allocated string containing the converted value, or NULL on error
 */
char *double_to_string(double value, int show_decimals)
{
    // Calculate required buffer size
    int buffer_size;

    if (value == 0)
    {
        buffer_size = 20; // Sufficient for "0"
    }
    else
    {
        // For integers, log10 + 1 gives the number of digits
        // Add margin for sign and null terminator
        buffer_size = (int)(fabs(log10(fabs(value))) + 20);
    }

    // Allocate memory for the buffer
    char *buffer = (char *)malloc(buffer_size);
    if (!buffer)
        return NULL;

    // Convert the double to string
    if (show_decimals)
    {
        snprintf(buffer, buffer_size, "%.10g", value);
    }
    else
    {
        // Convert to integer and then to string to remove decimals
        long long int_value = (long long)value;
        snprintf(buffer, buffer_size, "%lld", int_value);
    }

    return buffer;
}

/**
 * Reads a line of input from stdin with dynamic memory allocation
 * @return Dynamically allocated string containing the input line, or NULL on error
 */
char *read_line()
{
    char *buffer = NULL;
    size_t capacity = 0;
    size_t length = 0;
    int c;

    // Read characters until newline or EOF
    while ((c = getchar()) != EOF && c != '\n')
    {
        // If buffer is full, double its capacity
        if (length + 1 >= capacity)
        {
            capacity = capacity ? capacity * 2 : 16;
            char *new_buffer = realloc(buffer, capacity);
            if (!new_buffer)
            {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
        buffer[length++] = c;
    }

    // Add null terminator
    if (buffer)
    {
        buffer[length] = '\0';
    }
    else
    {
        // Handle empty input
        buffer = malloc(1);
        if (buffer)
            buffer[0] = '\0';
    }

    return buffer;
}

/**
 * Removes all whitespace from a string.
 * @param str Original string
 * @return New string without spaces (must be freed with free())
 */
char *remove_spaces(const char *str)
{
    size_t len = strlen(str);
    char *result = (char *)malloc(len + 1); // Maximum possible space (without spaces)
    if (!result)
        return NULL; // Memory error

    char *ptr = result;
    for (size_t i = 0; i < len; i++)
    {
        if (str[i] != ' ' && str[i] != '\t')
        { // Remove spaces and tabs
            *ptr++ = str[i];
        }
    }
    *ptr = '\0'; // Null-terminate the new string
    return result;
}

/**
 * Returns the appropriate function pointer for the given operator
 * @param operator The arithmetic operator character
 * @return Function pointer to the corresponding operation, or NULL if not supported
 */
OperationFunc get_operation(char operator)
{
    switch (operator)
    {
    case SUM:
        return add;
    case SUBTRACTION:
        return subtract;
    case MULTIPLICATION:
        return multiply;
    case DIVISION:
        return divide;
    default:
        return NULL;
    }
}

/**
 * Formats and displays the calculation in a right-aligned format
 * @param num1 First operand as string
 * @param num2 Second operand as string
 * @param operator The arithmetic operator character
 * @param result The calculation result as string
 */
void print_output(const char *num1, const char *num2, char operator, const char *result)
{
    // Determine the maximum width needed for alignment
    int max_width = 0;
    int len1 = strlen(num1);
    int len2 = strlen(num2) + 2; // +2 for operator and space
    int len_result = strlen(result);

    if (len1 > max_width)
        max_width = len1;
    if (len2 > max_width)
        max_width = len2;
    if (len_result > max_width)
        max_width = len_result;

    // Print with right alignment
    printf("%*s\n", max_width, num1);
    printf("%c%*s\n", operator, max_width - 1, num2);

    // Print separator line
    for (int i = 0; i < max_width; i++)
    {
        printf("-");
    }
    printf("\n");

    // Print result with right alignment
    printf("%*s\n", max_width, result);
}