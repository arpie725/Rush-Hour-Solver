#ifndef HEUR_H
#define HEUR_H

#include "board.h"

class Heuristic
{
public:
    virtual ~Heuristic() { }
    virtual size_t compute(const Board& b) = 0;
};

class BFSHeuristic : public Heuristic
{
public:
    size_t compute(const Board& b);
};

class DirectHeuristic : public Heuristic
{
public:
    size_t compute(const Board& b);
};

class IndirectHeuristic : public Heuristic
{
    // adding helper func here:
    // check if a vehicle exists above
    bool isVehicleAbove(const Board& b, const Vehicle& v); // checks if there is a vehicle above v
    // check if a vehicle exists below   
    bool isVehicleBelow(const Board& b, const Vehicle& v); 

    // return the vehicle above or below (ASSUMES THERE IS ONE!!!)
    Vehicle vehicleAbove(const Board& b, const Vehicle& v);
    Vehicle vehicleBelow(const Board& b, const Vehicle& v);

    bool maxMoveUpReal(const Board& b, const Vehicle& v); // checks if the vehicle can move up allowing the col with 'a' to be free
    bool maxMoveDownReal(const Board& b, const Vehicle& v); // checks if the vehicel can move down alloing the col with 'a' to be free
    bool maxMoveUpHypo(const Board& b, const Vehicle& v); // can the direct blocking veh hypothetically even work?
    bool maxMoveDownHypo(const Board& b, const Vehicle& v); // can the direct blocking veh hypothetically work??

public:
    size_t compute(const Board& b);
};


#endif
