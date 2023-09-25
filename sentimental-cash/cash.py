from cs50 import get_float
from math import floor

def main():
    # Prompt user for number of dollars
    while True:
        try:
            dollars = get_float("How much change is owed? ")
            if dollars < 0:
                print("Please enter a positive value greater than 0")
                continue
            break
        except ValueError:
            print("Please enter a number.")

    # Convert dollars to cents
    cents = floor(dollars * 100)

    # Calculate quarters and subtract from total owed
    quarters = calculate_quarters(cents)
    cents = cents - (quarters * 25)

    # Calculate dimes and subtract from total owed
    dimes = calculate_dimes(cents)
    cents = cents - (dimes * 10)

    # Calculate nickles and subtract from total owed
    nickels = calculate_nickels(cents)
    cents = cents - (nickels * 5)

    # Calculate pennies
    pennies = calculate_pennies(cents)

    # Print number of coins owed
    owed = quarters + dimes + nickels + pennies
    print(owed)


def calculate_quarters(cents):
    return cents // 25


def calculate_dimes(cents):
    return cents // 10


def calculate_nickels(cents):
    return cents // 5


def calculate_pennies(cents):
    return cents // 1

main()