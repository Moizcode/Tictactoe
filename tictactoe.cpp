#include <bits/stdc++.h>
using namespace std;
char tictac[3][3];

// state class
class node
{
public:
    char new_board[3][3];
    int result;
    char cur_c;
    node(char board[][3], int x, int y, char c, int re)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                new_board[i][j] = board[i][j];
            }
        }
        new_board[x][y] = c;
        cur_c = c;
        result = re;
    }
    vector<node *> child;
};

// check any player win or not
bool wincheck(char c, char checknode[][3])
{
    for (int i = 0; i < 3; i++)
    {
        if (checknode[i][0] == checknode[i][1] and checknode[i][1] == checknode[i][2] and checknode[i][2] == c)
        {
            return true;
        }
        if (checknode[0][i] == checknode[1][i] and checknode[1][i] == checknode[2][i] and checknode[2][i] == c)
        {
            return true;
        }
    }
    if (checknode[0][0] == checknode[1][1] and checknode[1][1] == checknode[2][2] and checknode[2][2] == c)
    {
        return true;
    }
    if (checknode[2][0] == checknode[1][1] and checknode[1][1] == checknode[0][2] and checknode[0][2] == c)
    {
        return true;
    }
    return false;
}

// finding computer move
node *comp_play(node *curr)
{
    for (auto element : curr->child)
    {
        if (curr->result == element->result)
        {
            return element;
        }
    }
}

// player move
void player()
{
    // player1 is x
    int x, y;
    cout << "Give x and y coordinate" << endl;
jump:
    cin >> x >> y;
    if (x > 0 and x < 4 and y > 0 and y < 4)
    {
        if (tictac[x - 1][y - 1] != 'x' and tictac[x - 1][y - 1] != 'o')
        {
            tictac[x - 1][y - 1] = 'x';
        }
        else
        {
            cout << "Re-enter correct co-ordination" << endl;
            goto jump;
        }
    }
    else
    {
        cout << "Re-enter correct co-ordination" << endl;
        goto jump;
    }
}

// MIN MAx ALGO
int min_max_algo(node *head, char player)
{
    if (head->result == 1 or head->result == -1 or head->result == 0) // base condition
    {
        return head->result;
    }

    if (player == 'o') // computer will try to minimize result
    {
        int mineval = INT_MAX;
        for (auto element : head->child)
        {
            int eva = min_max_algo(element, 'x');
            mineval = min(eva, mineval);
        }
        head->result = mineval;
        return mineval;
    }
    else if (player == 'x') // player will to try to maximize result
    {
        int maxeval = INT_MIN;
        for (auto element : head->child)
        {
            int eva = min_max_algo(element, 'o');
            maxeval = max(eva, maxeval);
        }
        head->result = maxeval;
        return maxeval;
    }
}

// match current board with game tree board
bool matchboard(node *element)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (element->new_board[i][j] != tictac[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

// finding matching node
node *findnode(node *prev)
{
    for (auto element : prev->child)
    {
        if (matchboard(element))
        {
            return element;
        }
    }
}

// structure of tictactoe
void showBoard()
{
    cout << " \n \n ";
    cout << " \t \t  " << tictac[0][0] << " | " << tictac[0][1] << " | " << tictac[0][2] << endl;
    cout << "\t\t------------ \n ";
    cout << " \t \t  " << tictac[1][0] << " | " << tictac[1][1] << " | " << tictac[1][2] << endl;
    cout << "\t\t------------ \n ";
    cout << " \t \t  " << tictac[2][0] << " | " << tictac[2][1] << " | " << tictac[2][2] << endl;
    return;
}

// queue to for traversing level order game tree
queue<node *> q;

int main()
{
    int x, y;
    cout << "Give x and y coordinate" << endl; // taking initial coordinate from player
jump:
    cin >> x >> y;
    node *head;
    if (x > 0 and x < 4 and y > 0 and y < 4)
    {
        head = new node(tictac, x - 1, y - 1, 'x', INT_MIN); // Root node of game tree
        tictac[x - 1][y - 1] = 'x';                          // changing current tictac board
    }
    else
    {
        cout << "Re-enter correct co-ordination" << endl;
        goto jump;
    }
    showBoard();
    q.push(head); // push root node in queue for traversing level wise

    while (!q.empty())
    {
        node *curr = q.front();
        q.pop();
        int res;
        char c;
        if (curr->cur_c == 'x') // selecting player for next move
        {
            c = 'o';
            res = INT_MAX;
        }
        else
        {
            c = 'x';
            res = INT_MIN;
        }
        for (int i = 0; i < 3; i++) // loop for each position on board
        {
            for (int j = 0; j < 3; j++)
            {
                if (curr->new_board[i][j] != 'x' and curr->new_board[i][j] != 'o') // if position not occupied then create a node and add it to parent node
                {
                    node *new_node = new node(curr->new_board, i, j, c, res);
                    (curr->child).push_back(new_node);
                    if (wincheck(new_node->cur_c, new_node->new_board)) // checking win condition and assign result
                    {
                        if (c == 'x')
                        {
                            new_node->result = 1; // +1 for player win
                        }
                        else if (c == 'o')
                        {
                            new_node->result = -1; // -1 for computer win
                        }
                    }
                    else
                    {
                        q.push(new_node); // pushing node in queue to find their possible childs
                    }
                }
            }
        }
        if ((curr->child).empty()) // if there are no more possible child node then it is draw condition
        {
            curr->result = 0;
        }
    }
    int first_minmax = min_max_algo(head, 'o'); // Runnig minmax algo
    node *current_node = head;
    for (int i = 0; i < 4; i++) // total 4 round we have
    {
        current_node = comp_play(current_node); // finding computer move
        for (int i = 0; i < 3; i++)             // changing it in current board
        {
            for (int j = 0; j < 3; j++)
            {
                tictac[i][j] = current_node->new_board[i][j];
            }
        }
        showBoard();
        if (wincheck('o', tictac))
        {
            cout << "COMPUTER WIN" << endl;
            break;
        }
        player(); // player move
        current_node = findnode(current_node);
        showBoard();
        if (wincheck('x', tictac))
        {
            cout << "YOU WIN" << endl;
            break;
        }
        else if (i == 3)
        {
            cout << "Draw" << endl;
        }
    }
    system("pause");
    return 0;
}