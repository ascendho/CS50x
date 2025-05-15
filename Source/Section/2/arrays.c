#include <stdio.h>

int const size = 5;

int main(void)
{
    // Declare an array
    int numbers[size];
    numbers[0] = 1;

    // Populate an array
    for (int i = 1; i < size; i++)
    {
        numbers[i] = numbers[i - 1] * 2;
    }

    // Print out the values one by one
    for (int i = 0; i < size; i++)
    {
        printf("%i\n", numbers[i]);
    }
}
