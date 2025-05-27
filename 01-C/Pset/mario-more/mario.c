#include <cs50.h>
#include <stdio.h>

void print_leftRow(int spaces, int bricks);
void print_rightRow(int bricks);

int main(void)
{
    // Prompt the user for the pyramid's height
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    // Print a pyramid of that height
    for (int i = 0; i < n; i++)
    {
        // Print row of bricks
        print_leftRow(n - 1 - i, i + 1);
        printf("  ");
        print_rightRow(i + 1);
    }

    return 0;
}

void print_leftRow(int spaces, int bricks)
{
    // Print spaces
    for (int i = 0; i < spaces; i++)
        printf(" ");

    // Print bricks
    for (int i = 0; i < bricks; i++)
        printf("#");
}

void print_rightRow(int bricks)
{

    // Print bricks
    for (int i = 0; i < bricks; i++)
        printf("#");

    printf("\n");
}
