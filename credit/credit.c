#include <cs50.h>
#include <stdio.h>
#include <math.h>

long get_card_number(void);
int sum_odd_digits(long number);
int sum_double_even_digits(long number);
int get_first_digit(long number);
int get_first_two_digits(long number);
int get_digits_number(long number);
string get_result(long number, int sum);

int main(void)
{
    long card_number = get_card_number();
    int sum_digits = sum_odd_digits(card_number);
    int sum_double_digits = sum_double_even_digits(card_number);
    int total_sum = sum_digits + sum_double_digits;
    printf("%s\n", get_result(card_number, total_sum));
}

long get_card_number(void)
{
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);
    return number;
}

int sum_odd_digits(long number)
{
    int sum = 0;
    long remainder = 0;
    long dividend = number;
    long quotient;
    int i = 1;
    do
    {
        // divider = 10^i
        long divider = pow(10, i);
        quotient = dividend / divider;
        // remainder of dividend / divider
        long new_remainder = dividend % divider;
        remainder += new_remainder;
        // get remainder or 0 if even
        int odd_remainder = i % 2 == 1 ? (remainder / pow(10, i - 1)) : 0;
        // add to the sum if even number
        sum += odd_remainder;
        // dividend = initial number - sum of all remainders
        dividend = number - remainder;

        i++;
    }
    // Stop when the quotient is < 1
    while (quotient > 0);

    return sum;
}

int sum_double_even_digits(long number)
{
    int sum = 0;
    long remainder = 0;
    long dividend = number;
    long quotient;
    int i = 1;
    do
    {
        // divider = 10^i
        long divider = pow(10, i);
        quotient = dividend / divider;
        // remainder of dividend / divider
        long new_remainder = dividend % divider;
        // double the remainder or 0 if even
        int double_remainder = i % 2 == 0 ? (new_remainder / pow(10, i - 1)) * 2 : 0;

        int first_digit = double_remainder % 10;
        int second_digit = double_remainder >= 10 ? 1 : 0;
        // add to the sum if even number
        sum += first_digit + second_digit;

        // dividend = initial number - sum of all remainders
        remainder += new_remainder;
        dividend = number - remainder;

        i++;
    }
    // Stop when the quotient is < 1
    while (quotient > 0);

    return sum;
}

int get_first_digit(long number)
{
    int ultimate_number = number;
    // Condition implies that number >= 1
    while (number)
    {
        ultimate_number = number;
        number /= 10;
    }
    return ultimate_number;
}

int get_first_two_digits(long number)
{
    int ultimate_number = number;
    int penultimate_number = number;
    // Condition implies that number >= 1
    while (number)
    {
        penultimate_number = ultimate_number;
        ultimate_number = number;
        number /= 10;
    }
    return penultimate_number;
}

int get_digits_number(long number)
{
    int i = 0;
    // Condition implies that number >= 1
    while (number)
    {
        i++;
        number /= 10;
    }
    return i;
}

string get_result(long number, int sum)
{
    const string INVALID = "INVALID";

    if (sum % 10 != 0)
    {
        return INVALID;
    }

    int digits_number = get_digits_number(number);
    int first_digit = get_first_digit(number);
    int first_two_digits = get_first_two_digits(number);
    switch (digits_number)
    {
        case 13:
            if (first_digit == 4)
            {
                // VISA can have 13 digits but starts with 4
                return "VISA";
            }
            break;
        case 15:
            if (first_two_digits == 34 || first_two_digits == 37)
            {
                // AMEX have 15 digits and starts with 34 or 37
                return "AMEX";
            }
        case 16:
            if (first_digit == 4)
            {
                // VISA can have 16 digits but starts with 4
                return "VISA";
            }
            else if (first_two_digits > 50 && first_two_digits < 56)
            {
                // MASTERCARD have 16 digits and start from 51 to 55
                return "MASTERCARD";
            }
            break;
        default:
            return INVALID;
    }

    return INVALID;
}