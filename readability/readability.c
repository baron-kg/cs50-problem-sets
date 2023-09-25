#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");

    int number_of_letters = count_letters(text);
    int number_of_words = count_words(text);
    int number_of_sentences = count_sentences(text);

    float L = (float) number_of_letters / (float) number_of_words * 100;
    float S = (float) number_of_sentences / (float) number_of_words * 100;

    int coleman_liau_index = round(0.0588 * L - 0.296 * S - 15.8);

    if (coleman_liau_index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (coleman_liau_index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", coleman_liau_index);
    }
}

int count_letters(string text)
{
    int number_of_letters = 0;

    for (int i = 0; i < strlen(text); ++i)
    {
        if (isalpha(text[i]))
        {
            number_of_letters++;
        }
    }

    return number_of_letters;
}

int count_words(string text)
{
    int number_of_words = 0;

    for (int i = 0; i < strlen(text); ++i)
    {
        if (text[i] == ' ')
        {
            number_of_words++;
        }
    }

    number_of_words++; // Increment once more assuming that the number of words is the number of spaces plus 1.

    return number_of_words;
}

int count_sentences(string text)
{
    int number_of_sentences = 0;

    for (int i = 0; i < strlen(text); ++i)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            number_of_sentences++;
        }
    }

    return number_of_sentences;
}