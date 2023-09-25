#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char replace_char(char c, string key);

int main(int argc, string argv[])
{
    // Check if there is any input.
    if (argc == 1)
    {
        printf("Usage: ./substitution KEY\n");

        return 1;
    }

    // Check the number of user inputs.
    if (argc > 2)
    {
        printf("Usage: ./substitution KEY\n");

        return 1;
    }

    // Check if the provided key is less or greater than 26.
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 letters.\n");

        return 1;
    }

    // Check if all the characters in the key are alphabetical.
    char key[strlen(argv[1])];

    strcpy(key, argv[1]);

    for (int i = 0; key[i] != '\0'; ++i)
    {
        if (isalpha(key[i]) == 0)
            {
            printf("Key must be numerical.\n");

            return 1;
        }
    }

    // Check if all letters of the alphabet are present.
    string alphabet = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; alphabet[i] != '\0'; ++i)
    {
        int t = 0;

        for (int j = 0; key[j] != '\0'; j++)
        {
            if (alphabet[i] == tolower(key[j]))
            {
                t++;
                break;
            }
        }

        if (t == 0)
        {
            printf("Key must contain alpha!\n");

            return 1;
        }
    }


    // Check for duplicates.
    for (int i = 0, t = 0; key[i] != '\0'; ++i)
    {
        if (key[i] == alphabet[i])
        {
            t++;
        }

        if (t > 1)
        {
            printf("They key must not contain duplicate letters.");

            return 1;
        }
    }


    // Prompt user for plaintext
    string plaintext = get_string("plaintext:  ");

    char ciphertext[strlen(plaintext)];

    // Encript plaintext
    for (int i = 0; plaintext[i] != '\0'; ++i)
    {
        ciphertext[i] = replace_char(plaintext[i], argv[1]);
    }

    // Print cihpertext
    printf("ciphertext: %.*s\n", (int)sizeof(ciphertext), ciphertext);
}

char replace_char(char c, string key)
{
    if (islower(c) != 0)
    {
        return c = tolower(key[c - 97]);
    }
    else if (isupper(c) != 0)
    {
        return c = toupper(key[c - 65]);
    }

    return c;
}