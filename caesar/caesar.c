#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool is_numeric(string argument);
char rotate(char c, int n);

int main(int argc, string argv[])
{
    // Check if there is any input.
    if (argc == 1)
    {
        printf("Usage: ./caesar key\n");

        return 1;
    }

    // Check the number of user inputs.
    if (argc > 2)
    {
        printf("Usage: ./caesar key\n");

        return 1;
    }

    // Check if the input is numeric.
    if (is_numeric(argv[1]) == 0)
    {
        printf("Usage: ./caesar key\n");

        return 1;
    }

    // Asign user input value as key.
    int key = atoi(argv[1]);

    // Prompt the user for the un-encripted text.
    string plaintext = get_string("plaintext:  ");

    // Initialize char array with the length equal to that of plaintext.
    char ciphertext[strlen(plaintext)];

    // Loop trough the plaintext and rotate every char and adding it to the ciphertext array.
    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        ciphertext[i] = rotate(plaintext[i], key);
    }

    printf("ciphertext: %s\n", ciphertext);
}

bool is_numeric(string argument)
{
    for (int i = 0; argument[i] != '\0'; ++i)
    {
        // If not a digit return 0.
        if (isdigit(argument[i]) == 0)
        {
            return 0;

            break;
        }
    }

    // If digit return 1.
    return 1;
}

char rotate(char c, int n)
{
    if (isalpha(c))
    {
        if (islower(c))
        {
            c = c - 97;

            c = (c + n) % 26;

            c = c + 97;
        }
        else if (isupper(c))
        {
            c = c - 65;

            c = (c + n) % 26;

            c = c + 65;
        }
    }

    return c;
}