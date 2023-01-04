import cs50


def main():
    dollars = get_dollars()

    quarters = compute_quarters(dollars)
    print(f"quarters = {quarters}")
    dollars = round(dollars - quarters * 0.25, 2)

    dimes = compute_dimes(dollars)
    print(f"dimes = {dimes}")
    dollars = round(dollars - dimes * 0.10, 2)

    nickels = compute_nickels(dollars)
    print(f"nickels = {nickels}")
    dollars = round(dollars - nickels * 0.05, 2)

    pennies = compute_pennies(dollars)
    print(f"pennies = {pennies}")
    dollars = round(dollars - pennies * 0.01, 2)

    # Sum coins
    coins = quarters + dimes + nickels + pennies

    # Print total number of coins to give the customer
    print(f"{coins}")


def get_dollars():
    """Ask how many dollars the customer is owed"""
    while (True):
        dollars = cs50.get_float("Change owned: ")
        if dollars > 0:
            break
    return dollars


def compute_coins(coins, coin_value):
    coin_value = int(100 * coin_value)
    coins = int(100 * coins)
    coins_count = 0
    for _ in range(coin_value, coins + 1, coin_value):
        coins_count += 1
    return coins_count


def compute_quarters(dollars):
    """Calculate the number of quarters to give the customer"""
    return compute_coins(dollars, 0.25)


def compute_dimes(dollars):
    """Calculate the number of dimes to give the customer"""
    return compute_coins(dollars, 0.10)


def compute_nickels(dollars):
    """Calculate the number of nickels to give the customer"""
    return compute_coins(dollars, 0.05)


def compute_pennies(dollars):
    """Calculate the number of pennies to give the customer"""
    return compute_coins(dollars, 0.01)


if __name__ == "__main__":
    main()