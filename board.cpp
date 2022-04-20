#include "board.h"
#include <iomanip>
#include <string>
#include <stdexcept>
#include <set>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

const Board::VID_T Board::escapeID_ = 'a';

Board::Board()
{
}

Board::Board(std::istream& is)
{
    vector<string > rawBoard;
    string line;
    while(getline(is, line)) {
        // skip empty lines
        stringstream ss(line);
        string temp;
        // Skip if no text on this line
        if(!(ss << temp)){
            continue;
        }
        // Must have contents, let's add it
        rawBoard.push_back(line);
    }
    // Error check
    if(rawBoard.size() == 0) {
        throw std::logic_error("0 size board");
    }
    unsigned i = 0;
    // Ensure same number of columns and rows
    while(i < rawBoard.size()) {
        if(rawBoard[i].size() != rawBoard.size()) {
            throw std::logic_error("Invalid row size");
        }
        i++;
    }

    // allocate rows for the 2D board
    grid_.resize(rawBoard.size());

    // Find and build all the Vehicle objects from the 2D grid
    for(int r = 0; r < (int)rawBoard.size(); r++) {
        // allocate columns for each row
        grid_[r].resize(rawBoard[r].size());
        for(int c = 0; c < (int)rawBoard[r].size(); c++) {
            
            // If we have vehicle in this location, update the vehicle
            if( isalpha(rawBoard[r][c] )) {
                grid_[r][c] = rawBoard[r][c];
                map<char, Vehicle>::iterator it = vehicles_.find(rawBoard[r][c]);
                if(it == vehicles_.end()) {
                    vehicles_.insert(make_pair(rawBoard[r][c], Vehicle(r, c, rawBoard[r][c])));
                }
                else {
                    if(it->second.startr == r) {
                        it->second.direction = Vehicle::HORIZONTAL;
                        it->second.length = (c - it->second.startc + 1);
#ifdef DEBUG
                        cout << "Increasing " << it->second.id << " to len " << it->second.length
                             << " and dir=" << it->second.direction << endl;
#endif
                    }
                    else {
                        it->second.direction = Vehicle::VERTICAL;
                        it->second.length = (r - it->second.startr + 1);
#ifdef DEBUG
                        cout << "Increasing " << it->second.id << " to len " << it->second.length
                             << " and dir=" << it->second.direction << endl;
#endif
                    }
                }
            }
            // Blank space
            else if( Vehicle::INVALID_ID  == rawBoard[r][c] )
            {
                grid_[r][c] = rawBoard[r][c];
            }
            else {
                throw std::logic_error("Invalid character in board");
            }
        }
    }
}

Board::Board(const Board& b) :
    vehicles_(b.vehicles_), grid_(b.grid_)
{
}

Board::~Board()
{
}

int Board::size() const 
{
    return (int) grid_.size();
}

Board::VID_T Board::at(int r, int c) const
{
    if(r >= (int)grid_.size() || c >= (int)grid_.size() || r < 0 || c < 0)
    {
        throw std::invalid_argument("Out of bounds index to at()");
    }
    return grid_[r][c];
}

const Vehicle& Board::vehicle(VID_T vid) const
{
    // can't use operator[] since it isn't a const member of map
    return vehicles_.at(vid);
}

const Vehicle& Board::escapeVehicle() const
{
    // can't use operator[] since it isn't a const member of map
    return vehicles_.at(escapeID_);
}

bool Board::solved() const
{
    // can't use operator[] since it isn't a const member of map
    const Vehicle& v = vehicles_.at(escapeID_);
    int r = v.startr;
    for(int c = v.startc + v.length; c < (int)grid_[r].size(); c++)
    {
        if(grid_[r][c] != Vehicle::INVALID_ID){
            return false;
        }
    }
    return true;
}

// Complete
bool Board::isLegalMove(VID_T vid, int amount) const
{
    // can't use operator[] since it isn't a const member of map
    const Vehicle& v = vehicles_.at(vid);
    if(v.direction == Vehicle::VERTICAL) {
        int newr = v.startr + amount;
        // Is target location still on the board
        if( (newr < 0) || ((newr + v.length) > (int) grid_.size()) ) {
            return false;
        }

        // Check if any other vehicle is blocking
        if(amount < 0)
        { 
            for(int r = v.startr-1; r >= newr; r--)
            {
                if(grid_[r][v.startc] != Vehicle::INVALID_ID)
                {
                    return false;
                }
            }
        }
        else 
        {
            for(int r = v.startr + v.length; r < v.startr + v.length + amount; r++)
            {
                if(grid_[r][v.startc] != Vehicle::INVALID_ID)
                {
                    return false;
                }
            }
        }
    }
    if(v.direction == Vehicle::HORIZONTAL) {
        int newc = v.startc + amount;

        // Is target location still on the board
        if( (newc < 0) || ((newc + v.length) > (int) grid_.size()) ) {
            return false;
        }

        // Check if any other vehicle is blocking
        if(amount < 0)
        { 
            for(int c = v.startc-1; c >= newc; c--)
            {
                if(grid_[v.startr][c] != Vehicle::INVALID_ID)
                {
                    return false;
                }
            }
        }
        else 
        {
            for(int c = v.startc + v.length; c < v.startc + v.length + amount; c++)
            {
                if(grid_[v.startr][c] != Vehicle::INVALID_ID)
                {
                    return false;
                }
            }
        }
    }
    return true;
}


bool Board::move(VID_T vid, int amount)
{
    if(isLegalMove(vid, amount)) {
        // copying the current state of the board onto the "history_" stack
        // making a copy of "vehicles_" before it gets modified
        VehicleMap mapCopy(vehicles_);
        // also making copy of "grid_"
        VehicleGrid gridCopy(grid_);
        // now push this copy onto the stack 
        history_.push(make_pair(mapCopy, gridCopy));
    
        Vehicle& v = vehicles_[vid];
        if(v.direction == Vehicle::VERTICAL) {
            v.startr += amount;
        }
        else {
            v.startc += amount;
        }
        updateBoard();
        return true;
    }
    else {
        return false;
    }

}


void Board::undoLastMove()
{
    pair<VehicleMap, VehicleGrid> cur = history_.top();
    // can use cur.first or cur.second 
    vehicles_ = cur.first; // reverting vehicles_ to the prev state
    grid_ = cur.second; // reverting grid_ to the prev state
    // at the end, just call pop() to remove the previous move from the stack
    history_.pop();
}

Board::MovePairList Board::potentialMoves() const
{
    // returning a list of MovePairs that are stored in a vector
    // MovePair contains the vehicle ID "a, b, c ..." and the amount it should move (int)
    Board::MovePairList returnVec; // create the vector
    VehicleMap::const_iterator mapIt;
    for(mapIt = vehicles_.begin(); mapIt != vehicles_.end(); ++mapIt) { // iterate over every single vehicle
        // need to check if this vehicle moves horizontal or vertical
        int idxPos = 1;
        int idxNeg = -1;
        int boardSize = this->size();
        while(idxNeg > -boardSize) { // workign with moving left or up
            if(!isLegalMove((*mapIt).first, idxNeg)) { // if the move is invalid, no need to keep going
                break;
            }
            // at this point the move is valid so add it to the vecotr
            returnVec.push_back(make_pair((*mapIt).first, idxNeg));
            idxNeg--;
        }
        while(idxPos < boardSize) { // working with moving right or down
            if(!isLegalMove((*mapIt).first, idxPos)) { // if the move is invalid, no need to keep going
                break;
            }
            // at this point the move is valid so add it to the vector
            returnVec.push_back(make_pair((*mapIt).first, idxPos));
            idxPos++;
        }
    }
    // now return the vector which is filled with all potential moves
    return returnVec;
}

bool Board::operator<(const Board& other) const
{
    string s1, s2;
    if(this->size() != other.size()) {
        return false;
    }
    for(int i = 0; i < (int) grid_.size(); i++) {
        for(int j = 0; j < (int) grid_.size(); j++) {
            s1 += grid_[i][j];
            s2 += other.grid_[i][j];
        }
    }
    return s1 < s2;
}

std::ostream& operator<<(std::ostream& os, const Board& b)
{
    for(int r = 0; r < (int)b.grid_.size(); r++) {
        for(int c = 0; c < (int)b.grid_[r].size(); c++) {
            os << b.grid_[r][c];
        }
        os << endl;
    }
    return os;
}

void Board::updateBoard()
{
    for(int r = 0; r < (int)grid_.size(); r++) {
        for(int c = 0; c < (int)grid_[r].size(); c++) {
            grid_[r][c] = '.';
        }
    }
    for (auto mypair : vehicles_)
    {
        if(mypair.second.direction == Vehicle::VERTICAL)
        {
            for(int r = 0; r < mypair.second.length; r++)
            {
                grid_[mypair.second.startr + r][mypair.second.startc] = mypair.second.id;
            }
        }
        else 
        {
            // horizontal
            for(int c = 0; c < mypair.second.length; c++)
            {
                grid_[mypair.second.startr][mypair.second.startc + c] = mypair.second.id;
            }
        }
    }
}