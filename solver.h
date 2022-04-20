#ifndef SOLVER_H
#define SOLVER_H
#include <vector>
#include <map>
#include <set>
#include "board.h"
#include "move.h"
#include "heur.h"
#include "heap.h"

class Solver
{
public:
    /// Typedef for the open-list set.  
    typedef Heap<Move*, MoveScoreComp > MoveHeap;

    /// Typedef for the closed-list set.  
    typedef std::set<Move*, MoveBoardComp> MoveSet;

    /**
     * @brief Constructs a solver object
     * 
     * @param[in] b Board from which to start the A* search 
     * @param[in] heur Pointer to derived Heuristic implementation 
     *                 that will be used to score Boards. 
     */
    Solver(const Board& b, Heuristic *heur);

    /**
     * @brief Destroy the Solver object
     * 
     */
    ~Solver();

    /**
     * @brief Runs the A* algorithm on the board passed to the constructor
     * 
     * @return true if a solution was found
     * @return false if no solution was found
     */
    bool run();

    /**
     * @brief Returns the solution produce by the LAST call to run().
     *        Should return the moves required to solve the puzzle
     *        in order from the first move to the last move needed to
     *        solve the board
     * 
     * @return Board::MovePairList 
     */
    Board::MovePairList solution() const;


    /**
     * @brief Return how many expansions were performed by the last call
     * to run()
     * 
     * @return int 
     */
    size_t numExpansions() const;

private:
    Board b_;
    Board::MovePairList solution_;
    size_t expansions_;
    Heuristic *heur_;
    // adding closed_list as a data member to help with dealloc later
    MoveSet closed_list;
};

#endif
