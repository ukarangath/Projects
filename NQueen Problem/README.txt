Overview of N-Queens Implementations

NQueensBT.java - This is the main file. The program is well-documented with debug prints that assist in 
understanding the logic during execution. The implementation follows a recursive backtracking approach, 
with an additional feature that counts the number of assignments made during backtracking.

NQueensLCV.java - This is the main file implementing the least-constraining-value heuristic to solve the 
N-Queens problem. Assignments are based on the most constrained variable, ensuring that queens are placed 
in columns that minimize future conflicts. For example, with 5 queens, the assignment order is 
2, 3, 1, 4, 0, reflecting this heuristic.

NQueensAC.java - This file implements arc-consistency (AC3) to solve the N-Queens problem. It ensures 
assignment stability by checking consistency across all assignments. If any inconsistency arises, the 
algorithm attempts alternative assignments. If no valid assignments are possible, it concludes that no 
solution exists.

MatrixLocation.java - A custom data type representing a location in the matrix. It holds values for row, 
column, and conflict count, indicating the number of constraints associated with a particular assignment 
in the matrix.

MatrixLocComparator.java - A comparator class for sorting MatrixLocation objects based on the conflict count, 
allowing for efficient constraint handling.

NQueenMinConflicts.java - This is the main file implementing a random restart with the min-conflicts heuristic. 
The program randomly assigns queens, calculates conflicts, and iteratively moves the queen with the highest 
conflict until either a solution is found or a maximum number of steps is reached.

