#include <cs50.h>
#include <stdio.h>

// Define candidate struct
typedef struct
{
    string name;
    int votes;
} candidate;

int main(void)
{
    // Define number of candidates
    const int num = 3;
    candidate candidates[num];

    // Populate the array with user's input
    for (int i = 0; i < num; i++)
    {
        candidates[i].name = get_string("Name: ");
        candidates[i].votes = get_int("Votes: ");
    }

    int highest_vote = 0;
    for (int i = 0; i < num; i++)
    {
        if (candidates[i].votes > highest_vote)
        {
            highest_vote = candidates[i].votes;
        }
    }

    for (int i = 0; i < num; i++)
    {
        if (candidates[i].votes == highest_vote)
        {
            printf("%s\n", candidates[i].name);
        }
    }
}
