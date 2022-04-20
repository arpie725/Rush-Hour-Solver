#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include "solver.h"

using namespace std;

Solver::Solver(const Board& b, Heuristic *heur)
{
    b_ = b;
    heur_ = heur;
    expansions_ = 0;
}

Solver::~Solver()
{
    // iterate over the closed_list set deleting each Move* obj
    set<Move*, MoveBoardComp>::iterator it;
    for(it = closed_list.begin(); it != closed_list.end(); ++it) {
        delete *it;
    }
}


bool Solver::run() {
    MoveHeap open_list;
    Board* firstBoard = new Board(b_);
    Move* firstBoardMove = new Move(firstBoard);
    open_list.push(firstBoardMove);
    closed_list.insert(firstBoardMove);
    // checking if solved
    if(firstBoard->solved()) {
        return true;
    }
    while(!open_list.empty()) {
        Move* parent = open_list.top();
        open_list.pop();
        // add parent board to the closed list right away
        closed_list.insert(parent);
        //making a board out of this parent
        Board parentsBoard = *parent->b;
        // now finding the potential moves
        Board::MovePairList potentialMoves = parentsBoard.potentialMoves();
        // iterate through each potential move making new boards
        for(int i = 0; i < (int) potentialMoves.size(); i++) {
            expansions_++;
            Board* potenMoveBoard = new Board(parentsBoard);
            // actually move this board
            potenMoveBoard->move(potentialMoves[i].first, potentialMoves[i].second);
            // check if this board won the game 
            if(potenMoveBoard->solved()) {
                // store the last move
                while(parent->prev != NULL) {
                    solution_.push_back(parent->m);
                    parent = parent->prev;
                }
                std::reverse(solution_.begin(), solution_.end());
                solution_.push_back(potentialMoves[i]);
                delete potenMoveBoard;
                return true;
            } else {
                // create a new Move for the board that was just made
                Move* potenMoveBoardsMove = new Move(potentialMoves[i], potenMoveBoard, parent);
                // make sure it was not already in the closed list
                if(closed_list.find(potenMoveBoardsMove) == closed_list.end()) {
                    // it was not in the closed list so add it and make it closed
                    potenMoveBoardsMove->score(heur_);
                    open_list.push(potenMoveBoardsMove);
                    closed_list.insert(potenMoveBoardsMove);
                } else {
                    // it already existed so just delete this move right here
                    delete potenMoveBoardsMove;
                }
            }
        }
    }
    return false;
}


Board::MovePairList Solver::solution() const
{
    return solution_;
}

size_t Solver::numExpansions() const
{
    return expansions_;
}
