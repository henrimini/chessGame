#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <omp.h>
#include <mpi.h>
#include <cstring>

using namespace std;
//The algorithm is not completely finished as i need to debug a bit but the structure is there

string fileToRead = "saj//saj4.txt";
vector<string> result;
int max_depth;
char gameStateChar[500];
char resultChar[100];

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
    int turn, size;
    vector<int> knight, bishop;

    GameState(int t, int s, vector<int> k, vector<int> b, vector<vector<int>> p, vector<string> m)
    {
        turn = t;
        size = s;
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
                GameState g(turn + 1, size, knight, nextMoves[i], newPieces, newMoves);
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
                GameState g(turn + 1, size, nextMoves[i], bishop, newPieces, newMoves);
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

string gameStateToString(GameState g)
{
    string s = "";
    vector<vector<int>> pieces = g.pieces;
    for (int i = 0; i < pieces.size(); i++)
    {
        s += std::to_string(pieces[i][0]) + "," + std::to_string(pieces[i][1]) + ",";
    }
    s += "!"; //jump line
    for (int i = 0; i != g.moves.size(); i++)
    {
        s += g.moves[i] + ",";
    }
    s += "!";
    s += std::to_string(g.turn) + "!";
    s += std::to_string(g.size) + "!";
    s += std::to_string(g.knight[0]) + "," + std::to_string(g.knight[1]) + "!";
    s += std::to_string(g.bishop[0]) + "," + std::to_string(g.bishop[1]);
    return s;
}

vector<string> split(const string &str, const string &delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos)
            pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty())
            tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

GameState stringToGameState(string s)
{
    vector<vector<int>> pieces;
    vector<string> gameVector = split(s, "!");
    vector<string> piecesVector = split(gameVector[0], ",");
    for (int i = 0; i < piecesVector.size() - 1; i += 2)
    {
        pieces.push_back({stoi(piecesVector[i]), stoi(piecesVector[i + 1])});
    }
    vector<string> moves = split(gameVector[1], ",");
    vector<string> knightVector = split(gameVector[4], ",");
    vector<string> bishopVector = split(gameVector[5], ",");
    vector<int> kight = {stoi(knightVector[0]), stoi(knightVector[1])};
    vector<int> bishop = {stoi(bishopVector[0]), stoi(bishopVector[1])};
    GameState g(stoi(gameVector[2]), stoi(gameVector[3]), kight, bishop, pieces, moves);
    return g;
}

bool isSameVectorOfCoor(vector<vector<int>> v1, vector<vector<int>> v2)
{
    if (v1.size() != v2.size())
        return false;
    for (int i = 0; i != v1.size(); i++)
    {
        if (v1[i][0] != v2[i][0] || v1[i][1] != v2[i][1])
            return false;
    }
    return true;
}

vector<GameState> visitedStates;

bool isVisited(GameState game)
{
    for (int j = 0; j != visitedStates.size(); j++)
    {
        if (visitedStates[j].bishop[0] == game.bishop[0] && visitedStates[j].bishop[1] == game.bishop[1] && visitedStates[j].knight[0] == game.knight[0] && visitedStates[j].knight[1] == game.knight[1] && visitedStates[j].turn % 2 == game.turn % 2 && isSameVectorOfCoor(visitedStates[j].pieces, game.pieces))
        {
            if (visitedStates[j].turn > game.turn)
            {
                visitedStates[j].turn = game.turn;
                return false;
            }
            return true;
        }
    }

#pragma omp critical
    visitedStates.push_back(game);

    return false;
}

void recursiveSolWTask(GameState game)
{
    if (game.pieces.size() == 0)
    {
        if (result.size() == 0 || result.size() > game.moves.size())
        {
#pragma omp critical
            {
                result = game.moves;
                max_depth = game.moves.size();
            }
        }
    }
    else if (!(game.turn > max_depth || game.turn > max_depth - game.pieces.size() /*|| isVisited(game)*/))
    {
        vector<GameState> nextStates = game.getNextStates();
        for (int i = 0; i != nextStates.size(); i++)
        {
            recursiveSolWTask(nextStates[i]);
        }
    }
}

void recursiveSol(GameState game)
{
    if (game.pieces.size() == 0)
    {
        if (result.size() == 0 || result.size() > game.moves.size())
        {
#pragma omp critical
            {
                result = game.moves;
                max_depth = game.moves.size();
            }
        }
    }
    else if (!(game.turn > max_depth || game.turn > max_depth - game.pieces.size() || isVisited(game)))
    {
        vector<GameState> nextStates = game.getNextStates();
        for (int i = 0; i != nextStates.size(); i++)
        {
            if (game.turn < 4)
            {
#pragma omp task
                recursiveSol(nextStates[i]);
#pragma omp taskwait
            }
            else
            {
                recursiveSolWTask(nextStates[i]);
            }
        }
    }
}

vector<GameState> createQueue(GameState startingState, int maxLength)
{
    vector<GameState> queue;
    vector<GameState> nextQueue;

    queue.push_back(startingState);
    while (queue.size() < maxLength)
    {
        for (int i = 0; i != queue.size(); i++)
        {
            if (!isVisited(queue[i]))
            {
                vector<GameState> nextStates = queue[i].getNextStates();
                for (int j = 0; j != nextStates.size(); j++)
                    nextQueue.push_back(nextStates[j]);
            }
        }
        queue = nextQueue;
        nextQueue = {};
    }
    return queue;
}

main(int argc, char **argv)
{
    //int turn = 0; //even for bishop & odd for knight
    MPI_Init(&argc, &argv); // initialize the MPI library
    int proc_num, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // setup the initial state
    int size;
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

    GameState g(0, size, knight, bishop, pieces, moves);
    //initial state done

    vector<GameState> queue = createQueue(g, 10);

    MPI_Status status;
    int freeSlave;
    static const int tag_work = 0;
    static const int tag_done = 1;
    static const int tag_finished = 2;

    if (proc_num == 0)
    { // master process :
        for (int dest = 1; dest < num_procs; dest++)
        {
            strcpy(gameStateChar, gameStateToString(queue[0]).c_str());
            MPI_Send(&gameStateChar, 500, MPI_CHAR, dest, tag_work, MPI_COMM_WORLD); //initial work distribution
            queue.erase(queue.begin());
        }
        int working_slaves = num_procs - 1; // the number of slaves
        while (working_slaves)              // (we stop when no more slaves are working and the queue is empty)
        {                                   // the main loop
            MPI_Recv(&resultChar, 100, MPI_CHAR, MPI_ANY_SOURCE, tag_done, MPI_COMM_WORLD, &status);

            if (result.empty() || split(resultChar, ",").size() < result.size())
            {
                result = split(resultChar, ",");
            }

            if (queue.size()) // if there is more work to do
            {
                strcpy(gameStateChar, gameStateToString(queue[0]).c_str());
                MPI_Send(&gameStateChar, 500, MPI_CHAR, status.MPI_SOURCE, tag_work, MPI_COMM_WORLD);
                queue.erase(queue.begin());
            }
            else
            {
                MPI_Send(&gameStateChar, 500, MPI_CHAR, status.MPI_SOURCE, tag_finished, MPI_COMM_WORLD);
                working_slaves--;
            }
        }

        for (int i = 0; i != result.size(); i++)
        {
            cout << result[i] << endl;
        }
    }
    else
    { // slave processes:
        while (true)
        {
            MPI_Recv(&gameStateChar, 500, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == tag_finished)
                break; // end of computation
            else
            { // do the work
                g = stringToGameState(gameStateChar);
                vector<GameState> queue2 = createQueue(g, 10);

#pragma omp parallel for
                for (int i = 0; i < queue2.size(); i++)
                {
                    recursiveSol(queue2[i]);
                }
                string resultStr = "";
                for (int i = 0; i != result.size(); i++)
                {
                    resultStr += result[i] + ",";
                }
                strcpy(resultChar, resultStr.c_str());
                MPI_Send(&resultChar, 100, MPI_CHAR, 0, tag_done, MPI_COMM_WORLD);
            }
        }
    }
    MPI_Finalize();
}
