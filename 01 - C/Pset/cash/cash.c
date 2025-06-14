#include <cs50.h>
#include <stdio.h>

int calculate(int cents, int denomination);

#define QUARTER 25
#define DIME 10
#define NICKLE 5
#define PENNY 1

int main(void)
{
    // Prompt the user for change owed, in cents
    int cents;
    do
    {
        cents = get_int("Change owed: ");
    }
    while (cents < 0);

    int quarters = calculate(cents, QUARTER);
    cents -= quarters * QUARTER;

    int dimes = calculate(cents, DIME);
    cents -= dimes * DIME;

    int nickles = calculate(cents, NICKLE);
    cents -= nickles * NICKLE;

    int pennies = calculate(cents, PENNY);
    cents -= pennies * PENNY;

    printf("%d\n", quarters + dimes + nickles + pennies);

    return 0;
}

int calculate(int cents, int denomination)
{
    return cents / denomination;
}
