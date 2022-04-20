#include "move.h"


Move::Move(Board* board)
{
    m.first = Vehicle::INVALID_ID;
    m.second = 0;
    b = board;
    g = 0;
    h = 0;
    prev = NULL;
}

Move::Move(const Board::MovePair& move, Board* b,  Move *parent)
    : m(move), b(b), prev(parent)
{
    this->g = parent->g + 1; // take the parent's value and add one to it (moving down a level)
}

Move::~Move()
{
    delete b;
}

bool Move::operator<(const Move& rhs) const
{
   // calculate the 2 f scores
   int f_lhs = this->g + this->h;
   int f_rhs = rhs.g + rhs.h;

   if(f_lhs < f_rhs) {
       return true;
   } else if(f_lhs > f_rhs) {
       return false;
   }
   // at this point, the lhs and rhs have the same f values
   if(this->h < rhs.h) { // "same f scores but move1 has smaller h score"
       return true;
   } else if(this->h > rhs.h) {
       return false;
   }
   // at this point, the lhs and rhs have the same f scores AND same h scores
   return true; // "we consider move1 < move2 to be true"
}

void Move::score(Heuristic *heur) 
{
    // using comput() in heuristic to assign the "h" score to this move
    h = heur->compute(*b);
}
