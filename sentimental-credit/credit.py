from cs50 import get_int
from re import match

# Prompt for card and check if lenght is valid
cardNumber = get_int("Please enter your card number: ")
if len(str(cardNumber)) not in [13, 15, 16]:
    print("INVALID")
    exit()

# Calculate checksum
card = cardNumber
firstDigitSum = 0
secondDigitSum = 0

while card > 0:
    firstDigitSum = firstDigitSum + (card % 10)
    card = card // 10

    everyOtherDigit = card % 10
    card = card // 10

    everyOtherDigit *= 2
    if everyOtherDigit > 9:
        everyOtherDigit = (everyOtherDigit // 10) + (everyOtherDigit % 10)
    secondDigitSum += everyOtherDigit

checksum = firstDigitSum + secondDigitSum

# Check if card checksum is valid
if (checksum % 10) != 0:
    print("INVALID")
    exit()

# Determine and print card manufacturer
if len(str(cardNumber)) == 15 and int(str(cardNumber)[:2]) in [34, 37]:
    print("AMEX")
elif len(str(cardNumber)) == 16 and int(str(cardNumber)[:2]) in [51, 52, 53, 54, 55]:
    print("MASTERCARD")
elif len(str(cardNumber)) == 13 or 16 and int(str(cardNumber)[:1]) == 4:
    print("VISA")
else:
    print("INVALID")