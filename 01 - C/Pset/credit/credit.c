#include <cs50.h>
#include <stdio.h>

bool luhn_check(long numbers);
int even_digit(int n);

int main(void)
{

    long numbers;
    do
    {
        numbers = get_long("Number: ");
    }
    while (numbers < 1);

    bool is_valid = luhn_check(numbers);

    if (!is_valid)
    {
        printf("INVALID\n");
        return 0;
    }

    int digits = 0;
    while (numbers > 100)
    {
        numbers = numbers / 10;
        digits++;
    }

    if (numbers == 34 || numbers == 37)
    {
        if (digits + 2 == 15)
        {
            printf("AMEX\n");
            return 0;
        }
        printf("INVALID\n");
    }
    else if (numbers == 51 || numbers == 52 || numbers == 53 || numbers == 54 || numbers == 55)
    {
        if (digits + 2 == 16)
        {
            printf("MASTERCARD\n");
            return 0;
        }
        printf("INVALID\n");
    }
    else
    {
        numbers = numbers / 10;
        if (numbers == 4)
        {
            if (digits + 2 == 13 || digits + 2 == 16)
            {
                printf("VISA\n");
                return 0;
            }
            printf("INVALID\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    return 0;
}

bool luhn_check(long numbers)
{
    int digit = 1;
    int odd_sum = 0;
    int even_sum = 0;
    for (; numbers > 0; numbers = numbers / 10, digit++)
    {
        int n = numbers % 10;
        if (digit % 2 == 1)
        {
            // Odd digit
            odd_sum += n;
        }
        else
        {
            // Even digit
            even_sum += even_digit(n);
        }
    }

    int sum = odd_sum + even_sum;

    if (sum % 10 == 0)
    {
        return true;
    }
    return false;
}

int even_digit(int n)
{
    int sum = 0;
    int product = n * 2;
    if (product >= 10)
    {
        for (int i = 0; i < 2; i++)
        {

            int single = product % 10;
            sum += single;
            product = product / 10;
        }
    }
    else
    {
        sum += product;
    }

    return sum;
}
