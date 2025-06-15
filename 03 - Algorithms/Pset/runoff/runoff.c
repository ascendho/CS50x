#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    for(int i = 0; i < candidate_count; i++)
        if(strcmp(name, candidates[i].name) == 0){
            preferences[voter][rank] = i;
            return true;
        }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    for(int i = 0; i < voter_count; i++)
    {
        for(int j = 0; j < candidate_count; j++)
        {
            int candidate_index = preferences[i][j];
            if(!candidates[candidate_index].eliminated)
            {
                candidates[candidate_index].votes++;
                break;
            }
        }
    }
    
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO

    int votes = 0;
    int maxIndex = 0;

    for(int i = 0; i < candidate_count; i++)
         // 只记录未被淘汰的候选人的总票数
        if(!candidates[i].eliminated){
            votes += candidates[i].votes;

              // 同时记录下票数最多的候选人
            if(candidates[maxIndex].votes < candidates[i].votes)
                maxIndex = i;
        }

    // 当前总选票的一半
    votes /= 2;

    // 持有最多票数的候选人选票 > 所有选票的一半
    if(candidates[maxIndex].votes > votes){
        printf("%s\n", candidates[maxIndex].name);
        return true;
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    // 选民的个数为候选人选票可能的最大个数（每人只能给任意一位候选人投一张选票）
    int min = voter_count;
    for(int i = 0; i < candidate_count; i++)
        if(!candidates[i].eliminated){
            if(min > candidates[i].votes)
                min = candidates[i].votes;
        }

    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    bool isTie = true;

    for(int i = 0; i < candidate_count; i++)
        if(!candidates[i].eliminated){
            if(min < candidates[i].votes){
                isTie = false;
                break;
            }
    }

    if(isTie)
        return true;

    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
   for(int i = 0; i < candidate_count; i++)
        if(!candidates[i].eliminated)
            if(min == candidates[i].votes)
                candidates[i].eliminated = true;

    return;
}
