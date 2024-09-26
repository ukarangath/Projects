-----------------------------------------------------------------------------------------------							
Question1 : Located in folder Question1.
Given Python code for solving N-queens using simple backtracking is modified to java as 
I am comfortable to code in java than python.

Source files : 
NQueensBT.java - which is the main file.Program is well documented and have debug prints
which helps in understanding the logic during a run.Implementation is same as given in the python code,recursive
back tracking.Counting the number of assignments is incorporated with back tracking.

Test_ResultsAndAnalysis.txt - Gives the test results and the analysis.
-----------------------------------------------------------------------------------------------	
Question2 : Located in folder Question2.

Source files : 
NQueensLCV.java - which is the main file.Program is well documented and have debug prints
which helps in understanding the logic during a run.In this least-constraining-value heuristicis used to find 
the solution for NQueen problem.Assignment of the queens at different columns are based on the most constrained
variable/least constraint value.If a column/queen is have other columns at left and right side,then it is the 
most constrained variable which has to be assigned before.This logic is used here.For example if there are 5 queens,then 
it is assigned in an order of 2,3,1,4,0.		

Test_ResultsAndAnalysis.txt - Gives the test results and the analysis.
-----------------------------------------------------------------------------------------------	
Question3 : Located in folder Question3.

Source files : 
NQueensAC.java - which is the main file.Program is well documented and have debug prints
which helps in understanding the logic during a run.This is an implementation of arc-consistency checking		
for solving NQueen problem.This is the implementation of AC3 algorithm.This checks the assignment stability
till the last assignment,if fails then does a different assignment if available.If not available till the first 
column then there is no solution exists.
Note : Here I have given total number of assignments during consistency check with another matrix.
This may not be the real number of assignments as per the question.In a arc consistency check 
with complete look ahead the final number of assignments is N.

Test_ResultsAndAnalysis.txt - Gives the test results and the analysis.
-----------------------------------------------------------------------------------------------	
Question4 : Located in folder Question4.

Source files : 
MatrixLocation.java - This is a custom data type class corresponds to a location in a matrix which 
has values like row,column and a new type like count of conflicts/constraints.Which means the total
number of constraints this location assignment with other assignments in the matrix.

MatrixLocComparator.java - comparator for MatrixLocation custom data type.This is for sorting MatrixLocation
objects based on the count of conflicts.

NQueenMinconflicts.java - which is the main file.Program is well documented and have debug prints
which helps in understanding the logic during a run.Default number of runs is 50,which is given in file
as MAX_STEPS.Program makes random assignment of queens and then calculate corresponding conflicts count.
Then moves the location which has maximum count of conflicts.Keep doing this until MAX_STEPS have reached
or total conflicts of all assignments are zero(Found solution),which ever is earlier.		

Test_ResultsAndAnalysis.txt - Gives the test results and the analysis.
-----------------------------------------------------------------------------------------------	