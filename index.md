# Description of the solution

## class GameState

I represented the current board state with a class __GameState__.
It has many attributes : 
- The __turn__ is is on : starts at 0. when the turn is even, the bishop mush make a move and when it is odd, it is the knight's turn
- The __size__ of the board
- __max_depth__ : we stop after we reach this number or there is still _x_ pieces to take in less than _max_depth - x_ turns

All pieces are represented as a vector of 2 int (x, y)
so we have : 
- __pieces__, a vector of a vector of int, representing all basic pieces left on the board,
- __knight__ and __bishop__, respectively the coordinates of the knight and the bishop
- __moves__, a vector of string representing the way to get to this state of the board

after that we have function to get the possible moves of the bishop and of the knight.
if a knight or a bishop picks up a piece, we remove this piece from the vector pieces.
with that done, we can get all next possible game states as a vector of GameState.

## main funct

in this function, we get read the saj file and we put the results as a GameState of turn 0.

## recursive funct

with what is defined above, the recursive function is really easy. It gets as an argument a boardState and returns also a boardState.
With that boardState, it returns the GameState with the minimum turn of this function with all next possible states.

If the number of turn is higher than the max_depth or there s still x pieces to take in less than max_depth - x turns, we return this game
If the game is finished (there are no more pieces), we also return the game.
 

 ## improvements of the algorithm by using dfs

 Unfortunately, the recursive function only works for saj1. So I tried to imprve this solution by using dfs and also recording the states we went throught and no expanding twice the same state. (i consider 2 states to be the same if the bishop, the knight, the pieces and the turn is the same modulo 2). but we still have to check the turn it is on and see if our new turn is lower than the old and in this case we must again developp the tree otherwise we might miss a good solution...

I then applyed the visitedstates on the recursive function on the file gameV2 (i duplicated the file just in case i need to go back to former states) and the number of recursive call was a lot less but it was still not enough to make saj2.txt work...

