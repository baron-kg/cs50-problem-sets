#include <cs50.h>
#include <stdio.h>

string check_card(long int card);

int main(void)
{
    long int card = get_long("Please enter your credit card number.\n");
    long int for_length_cal = card;
    long int for_last_two_digits = card;

    int first_digit_sum = 0;
    int second_digit_sum = 0;
    int final_sum = 0;
    long int last_two_digits;

    int mod1 = 0;
    int mod2 = 0;

    //Calculate card length
    int length = 0;
    do
    {
        for_length_cal = for_length_cal / 10;
        length++;
    }
    while (for_length_cal > 0);
    //printf("%i\n", length);

    // Check if length is valid
    if (length != 13 && length != 15 && length != 16)
    {
        printf("INVALID\n");
        return 0;
    }

    //Calculate the checksum
    do
    {
        mod1 = card % 10;
        card = card / 10;
        first_digit_sum = first_digit_sum + mod1;

        mod2 = card % 10;
        card = card / 10;

        mod2 = mod2 * 2;
        if(mod2 > 9)
        {
            mod2 = (mod2 / 10) + (mod2 % 10);
        }
        second_digit_sum = second_digit_sum + mod2;

    }
    while (card > 0);

    final_sum = first_digit_sum + second_digit_sum;
    //printf("%i\n", final_sum);

    //Check Luhn
    if (final_sum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    //Determain last two digits
    for(int i = 0; i < (length - 2); ++i)
    {
        for_last_two_digits = for_last_two_digits / 10;
        last_two_digits = for_last_two_digits;
    }
    //printf("%li\n", last_two_digits);

    if ((last_two_digits / 10 == 3) && (last_two_digits % 10 == 4 || last_two_digits % 10 == 7))
    {
        printf("AMEX\n");
    }
    else if ((last_two_digits / 10 == 5) && ( 0 < last_two_digits % 10 && last_two_digits % 10 < 6))
    {
        printf("MASTERCARD\n");
    }
    else if (last_two_digits / 10 == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}