#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
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

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    int i;
    for (i = 0; i < candidate_count; i++)
    {
        // Equality check
        if (strcmp(candidates[i], name) == 0)
        {
            break;
        }
    }

    // If we reached the end of the candidates it means we haven't found it
    if (i == candidate_count)
    {
        return false;
    }

    ranks[rank] = i;
    return true;
}

// Update preferences given one voter's ranks
//
// The function is called once for each voter, and takes as argument
// the ranks array, (recall that ranks[i] is the voter’s ith preference,
// where ranks[0] is the first preference).
//
// The function should update the global preferences array to add the current
// voter’s preferences. Recall that preferences[i][j] should represent the
// number of voters who prefer candidate i over candidate j.
//
// We may assume that every voter will rank each of the candidates.
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Shift one candidate and loop
        for (int j = i + 1; j < candidate_count; j++)
        {
            // We should increment the voters count based on the order of ranks
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}


// Record pairs of candidates where one is preferred over the other
//
// The function should add all pairs of candidates where one candidate is
// preferred to the pairs array. A pair of candidates who are tied
// (one is not preferred over the other) should not be added to the array.
//
// The function should update the global variable pair_count to be the number
// of pairs of candidates. (The pairs should thus all be stored between
// pairs[0] and pairs[pair_count - 1], inclusive).
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                // Add winner i and loser j in pair
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
}

void merge(int start, int middle, int end)
{
    // Create the two halves arrays
    int left_size = middle - start + 1;
    int right_size = end - middle;
    pair left[left_size];
    pair right[right_size];

    // Copy elements of left half to left array
    for (int i = 0; i < left_size; i++)
    {
        left[i] = pairs[start + i];
    }
    // Copy elements of right half to right array
    for (int i = 0; i < right_size; i++)
    {
        right[i] = pairs[middle + i + 1];
    }

    // Loop until there are no more elements in left or right
    int i = 0;
    int j = 0;
    int k = start;
    while (i < left_size && j < right_size)
    {
        // Check if left pair victory weaker than right pair victory
        if (preferences[left[i].winner][left[i].loser] <
            preferences[right[j].winner][right[j].loser])
        {
            pairs[k++] = left[i++];

        }
        else
        {
            pairs[k++] = right[j++];
        }
    }

    // Add remaining pairs in left
    while (i < left_size)
    {
        pairs[k++] = left[i++];
    }

    // Add remaining pairs in right
    while (j < right_size)
    {
        pairs[k++] = right[j++];
    }
}

void merge_sort(int start, int end)
{
    // Base case
    if (start >= end)
    {
        return;
    }

    int middle = (start + end) / 2;
    // Sort first half
    merge_sort(start, middle);
    // sort second half
    merge_sort(middle + 1, end);
    // We need to merge the 2 halves
    merge(start, middle, end);
}

// Sort pairs in decreasing order by strength of victory
//
// The function should add all pairs of candidates where one candidate is
// preferred to the pairs array. A pair of candidates who are tied
// (one is not preferred over the other) should not be added to the array.
//
// The function should update the global variable pair_count to be the number
// of pairs of candidates. (The pairs should thus all be stored between
// pairs[0] and pairs[pair_count - 1], inclusive).
void sort_pairs(void)
{
    //// Let's try to implement the sorting with the bubble sort algorithm
    // We should sort the array in decreasing order
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = 0; j < pair_count - i - 1; j++)
        {
            // Check if current victory is weaker than the next pair victory
            if (preferences[pairs[j].winner][pairs[j].loser] <
                preferences[pairs[j + 1].winner][pairs[j + 1].loser])
            {
                // Swap
                pair tmp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = tmp;
            }
        }
    }

    // This bubble sort algorithm has a complexity of O(n^2) meaning it can be
    // relatively slow for large arrays.
    //
    // Maybe we should try sorting by using merge sort algorithm with recursion
    // that has a complexity of O(nlog(n)).
    //
    // We should remember though that buble sort has an Omega of n so it is
    // more efficient for small arrays than merge sort which is consistently
    // O(nlog(n)) whatever the array size may be.
    //
    // Moreover the merge sort algorithm takes more memory space (in the stack
    // because of recursion) has opposed to bubble sort

    // Merge sort
    // merge_sort(0, pair_count - 1);
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("%i%i\n", pairs[i].winner, pairs[i].loser);
    // }
}


bool is_cycle(int start, int end)
{
    // If start equls end it is a cycle
    if (start == end)
    {
        return true;
    }

    // If end is lock with start it is a cycle
    if (locked[end][start])
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[end][i] && is_cycle(start, i))
        {
            return true;
        }
    }

    // No cycle
    return false;
}

void lock_pairs(void)
{
    // We have sorted the pairs in decreasing order previously so we can just
    // visit the pairs in natural order
    for (int i = 0; i < pair_count; i++)
    {
        pair p = pairs[i];
        // Lock the pair if no cycle
        if (!is_cycle(p.winner, p.loser))
        {
            // Lock
            locked[p.winner][p.loser] = true;
        }
    }
}

////
// OMG, I really tried so many complex solutions with DFS, Stack, multiple
// recursions, corner cases etc. but it was as simple as that ?!
////

// Recursive function that go through the graph
// bool is_cycle(int start, int end, bool visited[])
// {
//     // First we mak the current node as visited
//     visited[start] = true;

//     // Base case
//     if (locked[start][end])
//     {
//         // Cycle detected
//         return true;
//     }

//     // Check recursively all nodes connected the current one
//     for (int i = 0; i < candidate_count; i++)
//     {
//         if (locked[start][i] && !visited[i])
//         {
//             if (i == end || is_cycle(i, end, visited))
//             {
//                 // Cycle detected
//                 return true;
//             }
//         }
//     }

//     // No cycle
//     return false;
// }

// Recursive DFS (Depth First Search)
//
// We have to visit the whole graph using either DFS or BFS as my knowledge
// I choose DFS (Depth First Search) that usually uses recursion and
// a Stack data structure (LIFO).
// I use an array of visited node and backtracking once I've reached
// a leaf of the tree (no neighbours), a node with no
// unvisited neighbours or all neighbours are locked (equivalent)
//
// bool dfs(int start, int end, bool visited[])
// {
//     // Base case
//     if (locked[start][end])
//     {
//         // Cycle detected
//         return true;
//     }

//     visited[start] = true;

//     // For all candidates
//     for (int i = 0; i < candidate_count; i++)
//     {
//         if (locked[i][start] || visited[i])
//         {
//             // Skip
//             continue;
//         }

//         // Check recursively all nodes connected the current one
//         if (dfs(i, end, visited))
//         {
//             return true;
//         }
//     }

//     // No path found
//     return false;
// }

// bool is_cycle(int start, int end)
// {
//     // First we initilaize the visited array and
//     // mark the current node as visited
//     bool visited[candidate_count];
//     memset(visited, false, sizeof(visited));
//     visited[start] = true;

//     // DFS
//     bool result = dfs(start, end, visited);

//     // Reset the visited array
//     memset(visited, false, sizeof(visited));

//     return result;
// }
//
//void lock_pairs(void)
//{
//    // We use a Stack for DFS
//    pair stack[pair_count];
//    int top = -1;
//
//    // Fill the Stack
//    for (int i = 0; i < pair_count; i++)
//    {
//        stack[++top] = pairs[i];
//    }
//
//    // Clear the global locked array
//    for (int i = 0; i < candidate_count; i++)
//    {
//        for (int j = 0; j < candidate_count; j++)
//        {
//            locked[i][j] = false;
//        }
//    }
//
//    // Pop pairs from the stack and add them to the graph until the stack
//    // is empty
//    while (top >= 0)
//    {
//        pair p = stack[top--];
//
//        // Check if the adding the edge (winner, loser) to graph create cycle
//        if (!is_cycle(p.winner, p.loser))
//        {
//            locked[p.winner][p.loser] = true;
//        }
//        else
//        {
//            // If adding the pair creates a cycle,
//            // break the cycle by reversing the order of the pair
//            stack[++top] = (pair)
//            {
//                p.loser,
//                p.winner
//            };
//        }
//    }
//}

// Lock pairs into the candidate graph in order, without creating cycles
//
// The function should create the locked graph, adding all edges in decreasing
// order of victory strength so long as the edge would not create a cycle.
// void lock_pairs(void)
// {
//     // We have sorted the pairs in decreasing order previously so we can just
//     // visit the pairs in natural order
//     for (int i = 0; i < pair_count; i++)
//     {

//         // Init all nodes to unvisited
//         bool visited[candidate_count];
//         for (int j = 0; j < candidate_count; j++)
//         {
//             visited[j] = false;
//         }

//         // Lock the pair if no cycle
//         if (!is_cycle(pairs[i].winner, pairs[i].loser, visited))
//         {
//             // Lock
//             locked[pairs[i].winner][pairs[i].loser] = true;
//         }
//     }
// }

//void lock_pairs(void)
//{
//    // We need to also check that any of the pairs that came before in the
//    // list have already been locked in and would create a cycle.
//    //
//    // We also need to chedck whether adding the current pair would create
//    // a cycle with any of the pairs that come after in the list
//    //
//    // Not only that adding the current pair would add a cycle
//
//    pair locked_pairs[candidate_count * (candidate_count - 1) / 2];
//    int locked_pair_count = 0;
//
//    pair remaining_pairs[candidate_count * (candidate_count - 1) / 2];
//    int remaining_pair_count = 0;
//
//    // We have sorted the pairs in decreasing order previously so we can just
//    // visit the pairs in natural order
//    for (int i = 0; i < pair_count; i++)
//    {
//        pair p = pairs[i];
//
//        // Check if adding the current pair would create a cycle
//        bool creates_cycle = false;
//        for (int j = i + 1; j < locked_pair_count; j++)
//        {
//            // pair lp = locked_pairs[j];
//            pair ap = pairs[j];
//            // if (is_cycle(lp.winner, p.loser))
//            if (is_cycle(ap.winner, p.loser))
//            {
//                creates_cycle = true;
//                break;
//            }
//        }
//
//        // If adding the current pair would create a cycle, we store it in
//        // the remaining pairs
//        if (creates_cycle)
//        {
//            remaining_pairs[remaining_pair_count++] = p;
//        }
//        // If not, we lock the current pair
//        else
//        {
//            // Lock the pair if no cycle
//            locked[p.winner][p.loser] = true;
//            locked_pairs[locked_pair_count++] = p;
//        }
//    }
//
//    // if there are any remaining pairs, we try to lock them agai
//    if (remaining_pair_count > 0)
//    {
//        // Copy the remaining pairs back into the pairs array
//        for (int i = 0; i < remaining_pair_count; i++)
//        {
//            pairs[i] = remaining_pairs[i];
//        }
//
//        pair_count = remaining_pair_count;
//
//        // Try to lock remaining pairs
//        lock_pairs();
//    }
//}

////
////
////

// Print the winner of the election
//
// The function should print out the name of the candidate who is the source
// of the graph. You may assume there will not be more than one source.
void print_winner(void)
{
    // We just need to identify the source of the graph by checking if a
    // candidate is not locked in by any other candidate
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_source = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                is_source = false;
                break;
            }
        }

        if (is_source)
        {
            // Printing the winner
            printf("%s\n", candidates[i]);
            return;
        }
    }
}