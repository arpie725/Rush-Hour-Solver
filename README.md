Author: Ryan Phung 
Date: Februrary 24, 2022

** Using A* Algorithm to efficiently solve Rush Hour car game. 
** When launched, typing in '?' will start the solver to determine the most efficient solution to create a clear path
** There are three types of heuristic scores that are used to determine the state of each move, and how 
    close it is from reaching the goal. (Described below)

---------------------------------------
To run program in terminal, type: "./rh boards/<choose board> <Heuristic score (1-3)>

Heur score 1: brute force search, acts like a BFS
Heur score 2: Counts the number of vehicles to the right of the escape vehicle in its row 
    (i.e. number of vehicles directly blocking its escape).
Heur score 3: Counts the direct blocking vehicles and the vehicles blocking the direct blocking vehicles
