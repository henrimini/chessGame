#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>

using namespace std::chrono;
using namespace std;
string fileToRead = "saj//saj1.txt";
int count = 0;

bool isCorrectKnightMove(vector<int> move, vector<int> bishop, int size)
{
    if (move[0] >= size || move[1] >= size || move[0] < 0 || move[1] < 0)
    {
        return false;
    }
    if (move[0] == bishop[0] && move[1] == bishop[1])
    {
        return false;
    }
    return true;
}

vector<vector<int>> possibleKnightMoves(vector<int> knight, vector<int> bishop, int size)
{
    const vector<vector<int>> additions = {{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}};
    vector<vector<int>> moves;

    int x, y;
    x = knight[0];
    y = knight[1];

    for (int i = 0; i != additions.size(); i++)
    {
        vector<int> move = {x + additions[i][0], y + additions[i][1]};
        if (isCorrectKnightMove(move, bishop, size))
            moves.push_back(move);
    }
    return moves;
}

int isOnPiece(vector<int> move, vector<vector<int>> pieces)
{
    for (int i = 0; i != pieces.size(); i++)
    {
        if (move[0] == pieces[i][0] && move[1] == pieces[i][1])
            return i;
    }
    return -1;
}

vector<vector<int>> possibleBishopMoves(vector<int> knight, vector<int> bishop, vector<vector<int>> pieces, int size)
{
    vector<vector<int>> moves;
    int x, y;
    x = bishop[0];
    y = bishop[1];
    for (int i = 1; i < std::min(size - x, size - y); i++)
    {
        vector<int> move = {x + i, y + i};
        if (move[0] == knight[0] && move[1] == knight[1])
            break;
        moves.push_back(move);

        if (isOnPiece(move, pieces) != -1)
            break;
    }
    for (int i = 1; i < std::min(size - x, y + 1); i++)
    {
        vector<int> move = {x + i, y - i};
        if (move[0] == knight[0] && move[1] == knight[1])
            break;
        moves.push_back(move);

        if (isOnPiece(move, pieces) != -1)
            break;
    }
    for (int i = 1; i < std::min(x + 1, size - y); i++)
    {

        vector<int> move = {x - i, y + i};
        if (move[0] == knight[0] && move[1] == knight[1])
            break;
        moves.push_back(move);

        if (isOnPiece(move, pieces) != -1)
            break;
    }
    for (int i = 1; i <= std::min(x, y); i++)
    {
        vector<int> move = {x - i, y - i};
        if (move[0] == knight[0] && move[1] == knight[1])
            break;
        moves.push_back(move);

        if (isOnPiece(move, pieces) != -1)
            break;
    }

    return moves;
}

class GameState
{
public:
    vector<vector<int>> pieces;
    vector<string> moves;
    int turn, size, max_depth;
    vector<int> knight, bishop;

    GameState(int t, int s, int m_d, vector<int> k, vector<int> b, vector<vector<int>> p, vector<string> m)
    {
        turn = t;
        size = s;
        max_depth = m_d;
        knight = k;
        bishop = b;
        pieces = p;
        moves = m;
    }
    vector<GameState> getNextStates()
    {
        vector<GameState> nextStates;
        string nextMoveStr;
        if (turn % 2 == 0) //bishop's turn
        {
            vector<vector<int>> nextMoves = possibleBishopMoves(knight, bishop, pieces, size);
            if (nextMoves.size() == 0)
                nextMoves.push_back(bishop);
            for (int i = 0; i != nextMoves.size(); i++)
            {
                vector<string> newMoves = moves;
                vector<vector<int>> newPieces = pieces;
                int pieceNum = isOnPiece(nextMoves[i], pieces);
                if (pieceNum != -1)
                {
                    nextMoveStr = '*' + std::to_string(nextMoves[i][0]) + "-" + std::to_string(nextMoves[i][1]);
                    newPieces.erase(newPieces.begin() + pieceNum);
                }
                else
                {
                    nextMoveStr = std::to_string(nextMoves[i][0]) + "-" + std::to_string(nextMoves[i][1]);
                }
                newMoves.push_back(nextMoveStr);
                GameState g(turn + 1, size, max_depth, knight, nextMoves[i], newPieces, newMoves);
                nextStates.push_back(g);
            }
        }
        else
        {
            vector<vector<int>> nextMoves = possibleKnightMoves(knight, bishop, size);
            if (nextMoves.size() == 0)
                nextMoves.push_back(knight);
            for (int i = 0; i != nextMoves.size(); i++)
            {
                vector<string> newMoves = moves;
                vector<vector<int>> newPieces = pieces;
                int pieceNum = isOnPiece(nextMoves[i], pieces);
                if (pieceNum != -1)
                {
                    nextMoveStr = '*' + std::to_string(nextMoves[i][0]) + "-" + std::to_string(nextMoves[i][1]);
                    newPieces.erase(newPieces.begin() + pieceNum);
                }
                else
                {
                    nextMoveStr = std::to_string(nextMoves[i][0]) + "-" + std::to_string(nextMoves[i][1]);
                }
                newMoves.push_back(nextMoveStr);
                GameState g(turn + 1, size, max_depth, nextMoves[i], bishop, newPieces, newMoves);
                nextStates.push_back(g);
            }
        }
        return nextStates;
    }

    void printMoves()
    {
        for (int i = 0; i != moves.size(); i++)
        {
            cout << moves[i] << endl;
        }
    }
};

GameState minMoves(vector<GameState> games)
{
    int minIndex = 0;
    int minTurn = games[0].turn;
    for (int i = 1; i < games.size(); i++)
    {
        if (minTurn > games[i].turn && games[i].pieces.size() == 0)
        {
            minIndex = i;
            minTurn = games[i].turn;
        }
    }
    return games[minIndex];
}

GameState recursiveSol(GameState game)
{
    count++;
    if (game.pieces.size() == 0)
    {
        return game;
    }
    else if (game.turn > game.max_depth || game.turn > game.max_depth - game.pieces.size())
    {
        return game; //maybe should return nothing..
    }
    else
    {
        vector<GameState> nextStates = game.getNextStates();
        for (int i = 0; i != nextStates.size(); i++)
        {
            nextStates[i] = recursiveSol(nextStates[i]);
        }

        return minMoves(nextStates);
    }
}

main()
{
    auto start = high_resolution_clock::now();

    //int turn = 0; //even for bishop & odd for knight
    int size, max_depth;
    string saj;
    vector<vector<int>> pieces;
    vector<int> knight, bishop;
    vector<string> moves;

    ifstream MyReadFile(fileToRead);
    getline(MyReadFile, saj);
    size = std::stoi(saj);
    getline(MyReadFile, saj);
    max_depth = std::stoi(saj);

    for (int i = 0; i < size; i++)
    {
        getline(MyReadFile, saj);
        for (int j = 0; j < size; j++)
        {
            if (saj[j] == 'P')
            {
                pieces.push_back({j, i});
            }
            else if (saj[j] == 'S')
            {
                knight = {i, j};
            }
            else if (saj[j] == 'J')
            {
                bishop = {i, j};
            }
        }
    }
    MyReadFile.close();

    GameState g(0, size, max_depth, knight, bishop, pieces, moves);
    GameState ShortestSol = recursiveSol(g);

    auto stop = high_resolution_clock::now();   
    auto ms_int = duration_cast<seconds>(stop - start);
    cout << ms_int.count() << endl;

    ShortestSol.printMoves();
    //cout << count << endl;
}
