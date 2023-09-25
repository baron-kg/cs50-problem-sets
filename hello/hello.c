#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string users_name = get_string("What's your name? \n"); //Get the users name and store it in "name"
    printf("Hello %s, hope your having a great day!\n", users_name);
}