To run program: "./rh boards/<choose board> <Heuristic score (1-3)>

Heur score 1: brute force search, acts like a BFS
Heur score 2: Counts the number of vehicles to the right of the escape vehicle in its row 
    (i.e. number of vehicles directly blocking its escape).
Heur score 3: Counts the direct blocking vehicles and the vehicles blocking the direct blocking vehicles
