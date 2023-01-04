import cs50


def main():
    card_number = get_card_number()
    sum_digits = sum_odd_digits(card_number)
    sum_double_digits = sum_double_even_digits(card_number)
    total_sum = sum_digits + sum_double_digits
    print(f"{get_result(card_number, total_sum)}")


def get_card_number():
    """Ask the card number to the user"""
    while (True):
        number = cs50.get_int("Number: ")
        if number > 0:
            break
    return number


def sum_odd_digits(number):
    sum = 0
    remainder = 0
    dividend = number
    i = 1
    while (True):
        divider = 10 ** i
        quotient = dividend / divider
        new_remainder = dividend % divider
        remainder += new_remainder
        odd_remainder = int(remainder / 10 ** (i - 1)) if i % 2 == 1 else 0
        sum += odd_remainder
        dividend = number - remainder

        i += 1

        if quotient < 1:
            break

    return sum


def sum_double_even_digits(number):
    sum = 0
    remainder = 0
    dividend = number
    i = 1
    while (True):
        divider = 10 ** i
        quotient = dividend / divider
        new_remainder = dividend % divider
        double_remainder = (new_remainder / 10 ** (i - 1)) * 2 if i % 2 == 0 else 0
        first_digit = double_remainder % 10
        second_digit = 1 if double_remainder >= 10 else 0
        sum += first_digit + second_digit
        remainder += new_remainder
        dividend = number - remainder

        i += 1

        if quotient < 1:
            break

    return sum


def get_first_digit(number):
    str_number = str(number)
    return int(str_number[0])


def get_first_n_digits(number, n):
    str_number = str(number)
    return int(str_number[:n])


def count_digits(number):
    str_number = str(number)
    return len(str_number)


def get_result(number, sum):
    INVALID = "INVALID"

    if sum % 10 != 0:
        return INVALID

    digits_number = count_digits(number)
    first_digit = get_first_n_digits(number, 1)
    first_two_digits = get_first_n_digits(number, 2)
    match digits_number:
        case 13:
            if first_digit == 4:
                # VISA can have 13 digits but starts with 4
                return "VISA"
        case 15:
            if first_two_digits == 34 or first_two_digits == 37:
                # AMEX have 15 digits and starts with 34 or 37
                return "AMEX"
        case 16:
            if first_digit == 4:
                # VISA can have 16 digits but starts with 4
                return "VISA"
            elif first_two_digits > 50 and first_two_digits < 56:
                # MASTERCARD have 16 digits and start from 51 to 55
                return "MASTERCARD"
        case _:
            return INVALID

    return INVALID


if __name__ == "__main__":
    main()