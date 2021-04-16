#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer canditate i over canditate j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool loopcheck(int a, int b);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }


    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

//CHECKED
// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            //check ranks
            //printf("\n%i\n", ranks[rank]);
            return true;
        }
    }
    return false;
}

//CHECKED
// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = 1; j < candidate_count - i; j++)
        {
            int w = ranks[i];
            int l = ranks[i + j];
            preferences[w][l]++;
        }

    }

    //check preferences
    //for(int i = 0; i < candidate_count; i++)
    //{
    //    for(int j = 0; j < candidate_count; j++)
    //    {
    //        printf("%i\t", preferences[i][j]);
    //    }
    //    printf("\n");
    //}

    return;
}

//CHECKED
// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int idx = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[idx].winner = i;
                pairs[idx].loser = j;

                idx++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[idx].winner = j;

                idx++;
            }
        }
    }

    pair_count = idx;

    //CHECK SORTING pairs BEFORE

    //for(int i = 0; i < pair_count; i++)
    //{
    //    printf("w: %i, l: %i\t", pairs[i].winner, pairs[i].loser);
    //}
    //printf("\n");

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //I know the position of the winner and loser thanks to the array pairs --> strength inside the matrix preferences
    int biggest = preferences[pairs[0].winner][pairs[0].loser];
    int temp_idx = 0;
    pair tmp[1];

    for (int i = 0; i < pair_count; i++)
    {
        if (preferences[pairs[i].winner][pairs[i].loser] > biggest)
        {
            tmp[0] = pairs[temp_idx];
            //pair tmp.loser = pairs.loser[temp_idx];

            pairs[temp_idx] = pairs[i];
            pairs[i] = tmp[temp_idx];

            temp_idx = i;

        }

    }

    //CHECK SORTING pairs AFTER

    //for(int i = 0; i < pair_count; i++)
    //{
    //    printf("\nw: %i, l: %i\t", pairs[i].winner, pairs[i].loser);
    //}
    //printf("\n");


    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!loopcheck(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    //for(int i = 0; i < candidate_count; i++)
    //{
    //    for(int j = 0; j < candidate_count; j++)
    //    {
    //        printf("%i\t", locked[i][j]);
    //    }
    //    printf("\n");
    //}

    return;
}


bool loopcheck(int a, int b)
{
    if (locked[b][a] == true)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][a] == true)
        {
            return loopcheck(i, b);
        }
    }
    return false;
}



// Print the winner of the election
void print_winner(void)
{

    for (int i = 0; i < candidate_count; i++)
    {
        int check_sum = 0;

        for (int j = 0; j < candidate_count; j++)
        {
            check_sum = check_sum + locked[j][i];
        }

        if (check_sum == 0)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}




