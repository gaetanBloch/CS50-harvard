#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Equality check
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    string winners[candidate_count];
    int winner_index = 0;
    int highest_votes = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        int votes = candidates[i].votes;
        if (votes > 0)
        {
            if (votes > highest_votes)
            {
                winner_index = 0;
                highest_votes = votes;
                // Empty winners array
                memset(winners, 0, sizeof winners);
                winners[0] = candidates[i].name;
            }
            else if (votes == highest_votes)
            {
                // Add a winner
                winners[++winner_index] = candidates[i].name;
            }
        }
    }

    for (int j = 0; j < winner_index + 1; j++)
    {
        printf("%s\n", winners[j]);
    }
}