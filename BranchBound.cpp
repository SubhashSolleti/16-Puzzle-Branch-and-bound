// Program to print path from root node to destination node
// for N*N -1 puzzle algorithm using Branch and Bound
// The solution assumes that instance of puzzle is solvable..
#include <bits/stdc++.h>
#include <time.h>
using namespace std;
#define N 4
void delay(float s);
void pattern();

// state space tree nodes
struct Node
{
    // stores the parent node of the current node
    // helps in tracing path when the answer is found
    Node *parent;

    // stores matrix
    int mat[N][N];

    // stores blank tile coordinates
    int x, y;

    // stores the number of misplaced tiles
    int cost;

    // stores the number of moves so far
    int level;
};

// Function to print N x N matrix
int printMatrix(int mat[N][N])
{
    cout << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}

// Function to allocate a new node
Node *newNode(int mat[N][N], int x, int y, int newX,
              int newY, int level, Node *parent)
{
    Node *node = new Node;

    // set pointer for path to root
    node->parent = parent;

    // copy data from parent node to current node
    memcpy(node->mat, mat, sizeof node->mat);

    // move tile by 1 position
    swap(node->mat[x][y], node->mat[newX][newY]);

    // set number of misplaced tiles
    node->cost = INT_MAX;

    // set number of moves so far
    node->level = level;

    // update new blank tile coordinates
    node->x = newX;
    node->y = newY;

    return node;
}

// bottom, left, top, right
int row[] = {1, 0, -1, 0};
int col[] = {0, -1, 0, 1};

// Function to calculate the number of misplaced tiles
// ie. number of non-blank tiles not in their goal position
int calculateCost(int initial[N][N], int final[N][N])
{
    int count = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (initial[i][j] && initial[i][j] != final[i][j])
                count++;
    return count;
}

// Function to check if (x, y) is a valid matrix coordinate
int isSafe(int x, int y)
{
    return (x >= 0 && x < N && y >= 0 && y < N);
}

// print path from root node to destination node
void printPath(Node *root)
{
    if (root == NULL)
        return;
    printPath(root->parent);
    printMatrix(root->mat);

    printf("\n");
}

// Comparison object to be used to order the heap
struct comp
{
    bool operator()(const Node *lhs, const Node *rhs) const
    {
        return (lhs->cost + lhs->level) > (rhs->cost + rhs->level);
    }
};

// Function to solve N*N - 1 puzzle algorithm using
// Branch and Bound. x and y are blank tile coordinates
// in initial state
void solve(int initial[N][N], int x, int y,
           int final[N][N])
{
    // Create a priority queue to store live nodes of
    // search tree;
    priority_queue<Node *, std::vector<Node *>, comp> pq;

    // create a root node and calculate its cost
    Node *root = newNode(initial, x, y, x, y, 0, NULL);
    root->cost = calculateCost(initial, final);

    // Add root to list of live nodes;
    pq.push(root);

    // Finds a live node with least cost,
    // add its childrens to list of live nodes and
    // finally deletes it from the list.
    while (!pq.empty())
    {
        // Find a live node with least estimated cost
        Node *min = pq.top();

        // The found node is deleted from the list of
        // live nodes
        pq.pop();

        // if min is an answer node
        if (min->cost == 0)
        {
            // print the path from root to destination;
            printPath(min);
            return;
        }

        // do for each child of min
        // max 4 children for a node
        for (int i = 0; i < 4; i++)
        {
            if (isSafe(min->x + row[i], min->y + col[i]))
            {
                // create a child node and calculate
                // its cost
                Node *child = newNode(min->mat, min->x,
                                      min->y, min->x + row[i],
                                      min->y + col[i],
                                      min->level + 1, min);
                child->cost = calculateCost(child->mat, final);

                // Add child to list of live nodes
                pq.push(child);
            }
        }
    }
    cout << endl;
    cout << endl;
}

int main()
{
    int initial[N][N], x, y, s;
    int final[N][N] =
        {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0}};
    system("cls");
    pattern();
    system("cls");
    do
    {
        cout << "1.Auto " << endl;
        cout << "2.Manual" << endl;
        cout << "3.Exit" << endl;
        cout << "Enter your Choice: " << endl;
        cin >> s;
        switch (s)
        {
        case 1:
        {
            int initial2[N][N] = {
                {1, 0, 3, 4},
                {5, 2, 6, 8},
                {9, 10, 7, 11},
                {13, 14, 15, 12}};
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    if (initial2[i][j] == 0)
                    {
                        x = i;
                        y = j;
                    }
                }
            }
            solve(initial2, x, y, final);
        }
        break;
        case 2:
        {
            int i, j;
            cout << "Enter the your puzzle(Enter zero for space): " << endl;
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    cin >> initial[i][j];
                    if (initial[i][j] == 0)
                    {
                        x = i;
                        y = j;
                    }
                }
            }
            printf("\tYour Puzzle is Solving ...");

            for (i = 0; i < 3; i++)
            {

                delay(2);
                printf("..");
                delay(2);
                printf("\b!\b");
                printf("\b!\b");
                printf("\b!\b");
            }
            system("cls");
            cout << "The Puzzle is solved!" << endl;
            solve(initial, x, y, final);
        }
        break;
        case 3:
            cout << "Thank You!" << endl;
            cout << "Exit is done!" << endl;
            exit(0);
        default:
            cout << "Invalid choice!" << endl;
            cout << "Enter from 1 to 3 Only" << endl;
            break;
        }
    } while (s != 3);
}
void pattern()
{
    int x;
    char text1[] = "\n \t\t\t Welcome To 16 Puzzle Solver";
    char text2[] = "\n \t\t\t\t\t By ";
    char text3[] = "\n \t\t\t\t\t GROUP 11...... ";
    char text4[] = "\n \t\t\tBranch Bound Technique";

    printf("\n\n");

    for (x = 0; text1[x] != NULL; x++)
    {
        printf("%c", text1[x]);
        delay(0.1);
    }

    printf("\n");

    for (x = 0; text2[x] != NULL; x++)
    {
        printf("%c", text2[x]);
        delay(0.1);
    }

    printf("\n");
    for (x = 0; text3[x] != NULL; x++)
    {
        printf("%c", text3[x]);
        delay(0.1);
    }
    printf("\n");
    for (x = 0; text4[x] != NULL; x++)
    {
        printf("%c", text4[x]);
        delay(0.1);
    }
    printf("\n");
    delay(0.1);
}

void delay(float s)
{
    int ms = 1000 * s;
    clock_t t = clock();
    while (clock() < t + ms)
        ;
}
