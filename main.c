#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    MAX_LEN = 10000,
    STOP_CHAR = '\n',
};

char
get_number(char **pnumb)
{
    char *numb = *pnumb;
    int count = 0;
    int c;

    do {
        c = getchar();
        if (c == '+' || c == '-' || c == '*' || c == '\0' || c == STOP_CHAR) {
            numb[count++] = '\0';
            numb = realloc(numb, count);
            return c;
        }
        numb[count++] = c;

    } while (1);
}

char
*skip_zeros(char *number)
{
    while(*number == '0') {
        number++;
    }
    return number;
}

void
print(char *number)
{
    if (*number == '-'){
        printf("-");
        number++;
    }
    while (*number == '0') {
        number++;
    }
    if (!*number) {
        printf("0\n");
        return;
    }
    printf("%s\n", number);
}

void
swap_lines(char **first, char **second)
{
    char *t = *first;
    *first = *second;
    *second = t;
}

void
swap_numbers(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int
set_in_order(char **first, char **second, int *len1, int *len2)
{
    if (*len1 < *len2 || (*len1 == *len2 && strcmp(*first, *second) == -1)) {
        swap_lines(first, second);
        swap_numbers(len1, len2);
        return 1;
    }
    return 0;
}

char
*add(char *first, char *second)
{
    first = skip_zeros(first);
    second = skip_zeros(second);

    int len1 = strlen(first);
    int len2 = strlen(second);

    set_in_order(&first, &second, &len1, &len2);
    int differ = len1 - len2;

    char *result = malloc(len1 + 2);
    result[len1 + 1] = '\0';

    int carry = 0;
    for (int i = len2 - 1; i >= 0; --i) {
        int digit = second[i] - '0' + first[i + differ] - '0' + carry;
        carry = digit / 10;
        digit %= 10;
        result[i + differ + 1] = digit + '0';
    }
    for (int i = differ - 1; i >= 0; --i) {
        int digit = first[i] - '0' + carry;
        carry = digit / 10;
        digit %= 10;
        result[i + 1] = digit + '0';
    }
    result[0] = carry + '0';
    return result;
}

char
*subtract(char *first, char *second)
{
    int len1 = strlen(first);
    int len2 = strlen(second);

    char sign = '0';
    if (set_in_order(&first, &second, &len1, &len2)) {
        sign = '-';
    }
    int differ = len1 - len2;

    char *result = malloc(len1 + 2);
    result[len1 + 1] = '\0';

    for (int i = len2 - 1; i >= 0; --i) {
        if (first[i + differ] < '0') {
            first[i + differ] = '9';
            first[i + differ - 1] -= 1;
        }

        int digit = (first[i + differ] - '0') - (second[i] - '0');
        if (digit < 0) {
            digit += 10;
            first[i + differ - 1] -= 1;
        }
        result[i + differ + 1] = digit + '0';
    }

    for (int i = differ - 1; i >= 0; --i) {
        if (first[i] < '0') {
            first[i] = '9';
            first[i - 1] -= 1;
        }
        result[i + 1] = first[i];
    }

    result[0] = sign;
    return result;
}

char
*multiply_by_digit(char *multiplier, int len, int digit)
{
    char *result = malloc(len + 2);
    result[len + 1] = '\0';

    int carry = 0;
    for (int i = len - 1; i >= 0; --i) {
        int dig = (multiplier[i] - '0') * digit + carry;
        result[i + 1] = dig % 10 + '0';
        carry = dig / 10;
    }
    result[0] = carry + '0';

    return result;
}

char
*shift(char *number, int power)
{
    int len = strlen(number);
    number = realloc(number, len + power + 1);
    for (int i = len; i < len + power; ++i) {
        number[i] = '0';
    }
    number[len + power] = '\0';
    return number;
}

char
*multiply(char *first, char *second)
{
    int len1 = strlen(first);
    int len2 = strlen(second);

    set_in_order(&first, &second, &len1, &len2);

    char *result = multiply_by_digit(first, len1, second[len2 - 1] - '0');

    int power = 1;
    for (int i = len2 - 2; i >= 0; --i) {
        char *temp = multiply_by_digit(first, len1, second[i] - '0');
        temp = shift(temp, power++);
        result = add(result, temp);
        free(temp);
    }

    return result;
}


int
main(void)
{
    char *first_number = malloc(MAX_LEN + 1);
    char *second_number = malloc(MAX_LEN + 1);

    char oper = get_number(&first_number);
    get_number(&second_number);

    char *result = NULL;

    switch(oper)
    {
        case '+':
            result = add(first_number, second_number);
            break;
        case '-':
            result = subtract(first_number, second_number);
            break;
        case '*':
            result = multiply(first_number, second_number);
        default:
            break;
    }

    print(result);
    free(result);
    return 0;
}
