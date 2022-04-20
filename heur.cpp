#include <cmath>
#include "heur.h"
#include <vector>
#include <set>
using namespace std;

size_t BFSHeuristic::compute(const Board& b)
{
    return 0U;
}


size_t DirectHeuristic::compute(const Board& b)
{
    size_t count = 0;
    // retrieve the escape vehicle (a)
    Vehicle escapeA = b.escapeVehicle();
    int moving = escapeA.startc + escapeA.length - 1; // puts the pos at the far right of 'a' vehicle
    while(moving++ < b.size() - 1) {
        char temp = b.at(escapeA.startr, moving);
        if(temp != Vehicle::INVALID_ID) { // if the loc is occupied by a vehicle
            Vehicle tempV = b.vehicle(temp);
            if(tempV.direction == Vehicle::VERTICAL) { // checking if the vehicle's direction is vert
                count++;
            } 
            // update: there will not be a horizontal vehicle blocking 'a' b/c logically makes no sense
        }
    }
    return count;
}

size_t IndirectHeuristic::compute(const Board& b)
{
    size_t count = 0;
    // creating a set 
    set<char> vehicleSet;
    // need to iterate each column in a's row 
    int row = b.escapeVehicle().startr; // unchanged
    int col = b.escapeVehicle().startc + b.escapeVehicle().length; // column changes searching for a direct blocking veh
    for(int col_creep = col; col_creep < b.size(); col_creep++) { // move from left to right
        if(b.at(row, col_creep) != Vehicle::INVALID_ID) { // a vehicle is blocking 'a'
            count++; // incrementing count for the direct blocking vehicle
            Vehicle v = b.vehicle(b.at(row, col_creep)); // retrieve this vehicle store in 'v'
            // test if there are 2 blockages preventing the direct veh from freeing
            if(maxMoveUpHypo(b, v) && maxMoveDownHypo(b, v) && !maxMoveUpReal(b, v) && !maxMoveDownReal(b, v)) {
                continue;
            }
            // now seeing if moving up can hypothetically free the blocked space
            if(maxMoveUpHypo(b, v) && !maxMoveDownReal(b, v) && !maxMoveDownHypo(b, v)) {
                // the vehicle can hypothetically move up to clear. now need to check if it is blocked
                if(!maxMoveUpReal(b, v)) {
                    // now need to get this indirect blocking veh
                    while(isVehicleAbove(b, v)) {
                        Vehicle temp_v;
                        temp_v = vehicleAbove(b, v);

                        if(vehicleSet.find(temp_v.id) == vehicleSet.end()) { 
                            // the vehicle is new and has not been used before
                            vehicleSet.insert(temp_v.id);
                            count++;
                        }
                        v = temp_v; // updating the 'v'
                    }
                }
            } else if(maxMoveDownHypo(b, v) && !maxMoveUpReal(b, v) && !maxMoveUpHypo(b, v)) { // now seeing if moving down can hypothetically free the blocked space
                if(!maxMoveDownReal(b, v)) {
                    // now need to get the indirect blocking veh
                    while(isVehicleBelow(b, v)) {
                        Vehicle temp_v;
                        temp_v = vehicleBelow(b, v);
                        if(vehicleSet.find(temp_v.id) == vehicleSet.end()) {
                            // the vehicle is new and has not been used before 
                            vehicleSet.insert(temp_v.id);
                            count++;
                        }
                        v = temp_v; // updating the 'v'
                    }
                }
            }
        }
    }
    return count;
}

bool IndirectHeuristic::isVehicleAbove(const Board& b, const Vehicle& v) {
    int row_start_loc = v.startr - 1;
    int col = v.startc; // this location will be unchanged
    for(int r = row_start_loc; r >= 0; r--) {
        if(b.at(r, col) != Vehicle::INVALID_ID) { // if there is a vehicle in the path
            return true;
        }
    }
    return false;
}

bool IndirectHeuristic::isVehicleBelow(const Board& b, const Vehicle& v ) {
    int row_start_loc = v.startr + v.length;
    int col = v.startc; // this loc is unchanged
    for(int r = row_start_loc; r < b.size(); r++) {
        if(b.at(r, col) != Vehicle::INVALID_ID) { // if there is a vhicle in the path
            return true;
        }
    }
    return false;
}

bool IndirectHeuristic::maxMoveUpReal(const Board& b, const Vehicle& v) {
    int col = v.startc;
    int tracker = 0;
    int moves_needed = v.startr + v.length - b.escapeVehicle().startr;
    for(int r = v.startr - 1; r >= 0; r--) {
        if(b.at(r, col) == Vehicle::INVALID_ID) {
            tracker++;
        } else {
            break;
        }
    }
    if(tracker >= moves_needed) {
        return true;
    }
    return false;
}

bool IndirectHeuristic::maxMoveDownReal(const Board& b, const Vehicle& v) {
    int col = v.startc;
    int tracker = 0;
    int moves_needed = b.escapeVehicle().startr - v.startr + 1;
    for(int r = v.startr + v.length; r < b.size(); r++) {
        if(b.at(r, col) == Vehicle::INVALID_ID) {
            tracker++;
        } else {
            break;
        }
    }
    if(tracker >= moves_needed) {
        return true;
    }
    return false;
}

bool IndirectHeuristic::maxMoveUpHypo(const Board& b, const Vehicle& v) {
    int moves_needed = v.startr + v.length - b.escapeVehicle().startr;
    if(v.startr >= moves_needed) {
        return true;
    }
    return false;
}

bool IndirectHeuristic::maxMoveDownHypo(const Board& b, const Vehicle& v) {
    int moves_needed = b.escapeVehicle().startr - v.startr + 1;
    int max = b.size() -  (v.startr + v.length);
    if(max >= moves_needed) {
        return true;
    }
    return false;
}

Vehicle IndirectHeuristic::vehicleAbove(const Board& b, const Vehicle& v) {
    int c = v.startc;
    int r = v.startr - 1;
    while(r >= 0) {
        Board::VID_T cur = b.at(r, c);
        if(cur != Vehicle::INVALID_ID) {
            return b.vehicle(cur);
        }
        r--;
    }
    // designed my function so that the code will never go into this
    throw std::logic_error(" ");
}

Vehicle IndirectHeuristic::vehicleBelow(const Board& b, const Vehicle& v) {
    int c = v.startc;
    int r = v.startr + v.length;
    while(r < b.size()) {
        Board::VID_T cur = b.at(r, c);
        if(cur != Vehicle::INVALID_ID) {
            return b.vehicle(cur);
        }
        r++;
    }
    // designed my function so that the code will never go into this
    throw std::logic_error(" ");
}