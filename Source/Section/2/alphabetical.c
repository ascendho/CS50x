#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Get user's input
    if (argc != 2)
    {
        printf("Please provide a word.\n");
        return 1;
    }

    string text = argv[1];
    int len = strlen(text);

    for (int i = 0; i < len; i++)
    {
        if (!isalpha(text[i]))
        {
            printf("This is not a letter.\n");
            return 2;
        }
    }

    // Iterate through each element in the string
    for (int i = 1; i < len; i++)
    {
        if (text[i] < text[i - 1])
        {
            printf("No\n");
            return 0;
        }
    }
    // Print out yes
    printf("Yes\n");
}
