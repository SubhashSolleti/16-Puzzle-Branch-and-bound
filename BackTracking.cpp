// Program to implement the 15-puzzle problem using backtracking method...
#include <bits/stdc++.h>
#include <time.h>
using namespace std;

void delay(float s);
void pattern();
int nodesExplored;
int nodesGenerated;

int goal[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
string optrs[] = {"UP", "DOWN", "LEFT", "RIGHT"};
int n = 4;

class Node
{
public:
    int puzzle[16];
    Node *parent;
    string optr;
    int static id;

    Node()
    {
        parent = NULL;
        optr = "";
        id++;
    }

    Node(int board[])
    {
        optr = "";
        parent = NULL;

        for (int i = 0; i < 16; i++)
            this->puzzle[i] = board[i];

        id++;
    }

    Node(Node *temp)
    {
        this->parent = temp->parent;
        optr = temp->optr;

        for (int i = 0; i < 16; i++)
            this->puzzle[i] = temp->puzzle[i];

        id++;
    }

    string Tostring()
    {
        string retStr = "";
        for (int i = 0; i < 16; i++)
        {
            stringstream ss;
            ss << puzzle[i];
            string str = ss.str();
            retStr = retStr + str;
        }
        return retStr;
    }
};

int Node ::id = 0;

void Print(int[], int);
bool validate(int[], string);
void SolPath(Node *);
int SearchBlank(int[]);
bool GoalTest(int[]);

int main()
{
    int initial[n][n], i, j;
    // Initial configuration
    // Value 0 is used for empty space
    system("cls");
    pattern();
    system("cls");
    cout << "Enter the initial puzzle, enter space with zero : " << endl;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> initial[i][j];
        }
    }

    int final[n][n] =
        {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0}};

    int puzzle[16];
    int k = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            puzzle[k] = initial[i][j];
            k++;
        }
    }

    Node node = new Node(puzzle);

    static bool flag = true;

    queue<Node> BFS_Q;

    BFS_Q.push(node);

    if (GoalTest(node.puzzle))
        Print(node.puzzle, 0);

    else
    {
        list<string> hashSet;

#pragma omp parallel num_threads(8) shared(BFS_Q, hashSet, nodesExplored, nodesGenerated)
        {
            while (flag)
            {
#pragma omp critical
                {
                    if (!BFS_Q.empty())
                    {
                    }

                    Node *current = new Node(BFS_Q.front());

                    // Pop it
                    BFS_Q.pop();

                    hashSet.push_back(current->Tostring());

                    // Increment the nodes explored
                    nodesExplored++;

                    for (i = 0; i < 4 && flag; i++)
                    {
                        if (validate(current->puzzle, optrs[i]) == true)
                        {
                            int board[16];
                            for (int j = 0; j < 16; j++)
                                board[j] = current->puzzle[j];

                            int blankIndex = SearchBlank(board);

                            if (optrs[i] == "UP")
                            {
                                board[blankIndex] = board[blankIndex - 4];
                                board[blankIndex - 4] = 0;
                            }

                            else if (optrs[i] == "DOWN")
                            {
                                board[blankIndex] = board[blankIndex + 4];
                                board[blankIndex + 4] = 0;
                            }

                            else if (optrs[i] == "LEFT")
                            {
                                board[blankIndex] = board[blankIndex - 1];
                                board[blankIndex - 1] = 0;
                            }

                            else
                            {
                                board[blankIndex] = board[blankIndex + 1];
                                board[blankIndex + 1] = 0;
                            }

                            Node *child = new Node(board);

                            child->parent = current;
                            child->optr = optrs[i];

                            nodesGenerated++;

                            if (!(find(hashSet.begin(), hashSet.end(), child->Tostring()) != hashSet.end()))
                            {
                                if (GoalTest(child->puzzle) == true)
                                {
                                    SolPath(child);
                                    flag = false;
                                }

                                BFS_Q.push(child);
                            }
                        }
                    }
                }
            }
        }
    }

    cout << "Depth First Search Completed" << endl;
    cout << "Nodes Explored: " << nodesExplored << endl;
    cout << "Nodes Generated: " << nodesGenerated << endl;
}

void Print(int puzzle[], int n)
{
    if (n == 0)
        cout << "\nSolution found : ";

    else
        cout << endl;
    cout << "Step#" << n << ": " << endl;

    for (int i = 0; i < 16; i++)
    {
        cout << puzzle[i] << " ";
        if ((i + 1) % 4 == 0)
            cout << endl;
    }
}

bool validate(int board[], string o)
{
    if (o == "UP")
    {
        if (board[0] == 0 || board[1] == 0 || board[2] == 0 || board[3] == 0)
            return false;
        else
            return true;
    }

    else if (o == "DOWN")
    {
        if (board[15] == 0 || board[14] == 0 || board[13] == 0 || board[12] == 0)
            return false;
        else
            return true;
    }

    else if (o == "LEFT")
    {
        if (board[0] == 0 || board[4] == 0 || board[8] == 0 || board[12] == 0)
            return false;
        else
            return true;
    }

    else
    {
        if (board[3] == 0 || board[7] == 0 || board[11] == 0 || board[15] == 0)
            return false;
        else
            return true;
    }
}

void SolPath(Node *head)
{
    Node *p = head;

    if (p == NULL)
        return;

    int i = 0;

    while (p != NULL)
    {
        Print(p->puzzle, i);
        p = p->parent;
        i++;
    }

    cout << endl;
}

int SearchBlank(int board[])
{
    for (int i = 0; i < 16; i++)
        if (board[i] == 0)
            return i;
}

bool GoalTest(int board[])
{
    int count = 0;
    for (int i = 0; i < 16; i++)
    {
        if (board[i] == goal[i])
            count++;
    }

    if (count == 16)
        return true;

    return false;
}
void pattern()
{
    int x;
    char text1[] = "\n \t\t\t Welcome To 16 Puzzle Solver";
    char text2[] = "\n \t\t\t\t\t By ";
    char text3[] = "\n \t\t\t\t\t GROUP 11...... ";
    char text4[] = "\n \t\t\tBack Tracking Technique";

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
