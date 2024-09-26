
import java.util.Arrays;
import java.util.Random;
/**
 * @author Unnikrishnan 
 * 
 * @purpose : Main File for Question Number 4 : NQueens solution using min-conflicts local search
 * 
 * Note: debug-prints are retained in the code,this will help to understand implementation.
 * Needs to enter value of N manually at main() method.
 *
 */
public class NQueenMinconflicts {
	public int[][] solution;
	public static int assignments; 
	public final int MAX_STEPS = 50;

	/*
	 * Constructor 
	 */		
	public NQueenMinconflicts(int N) {
		solution = new int[N][N];
		//Initialization step 
		for (int rowIndex = 0; rowIndex < N; rowIndex++) {
			for (int columnIndex = 0; columnIndex < N; columnIndex++) {
				solution[rowIndex][columnIndex] = 0;
			}
		}		
	}	
	/*
	 * Main method which prints the result if found a solution.
	 * According to minconflicts local search queens are assigned randomly.
	 * Here rand variable gives random row locations for the queen at a particular column.
	 * Initially matrix is assigned with queens randomly in each columns. 
	 * Custom datatype/class named MatrixLocation is used to hold row,colum,conflicts count of a
	 * queen location(source is located at different file,MatrixLocation.java and MatrixLocComparator,java
	 * ,for sorting the matrixlocation variables according to the conflicts count).
	 * 
	 * printQueenLocations - prints the queen location with conflict count at along side.
	 * updateConflicts - updates the conflict count variable of each matrix location.
	 * Above two methods are called with moveQueens to the maximum times - MAX_STEPS(50,can be changed).
	 * 
	 *If the sum of the conflicts of all queen locations turns to zero means solution is found in 
	 * maximum 50 steps.If not there is no solution found in a run of 50 steps.
	 * This does not mean there is no solution.It means itjust did not find solution.
	 */
	public void solve(int N) {
		if(N < 4){
			System.out.println("NO SOLUTION EXISTS");
			return;
		}
		int randomRow;
		//total conflicts 
		int totalconflicts = 0;
		Random rand = new Random();
		//Saves the location of four queens
		MatrixLocation[] queenLocations = new MatrixLocation[N];
		//Randomly put each queen in a row of each column
		System.out.println("Initial step,Randomly put all queens in the matrix");
		for (int queen = 0; queen < N; queen++) {
			//Generates a row randomly in the limit of 0 to 3
			randomRow = rand.nextInt((3 - 0) + 1) + 0;
			solution[randomRow][queen] = 1;
			queenLocations[queen] = new MatrixLocation(randomRow, queen);
		}
		printMatrix(solution,N);
		//Printing the current queen locations
		printQueenLocations(queenLocations);
		updateConflicts(queenLocations);
		//runs for MAX_STEPS,if finds a solution then there is a solution if not there is no solution found
		for(int step = 0; step < MAX_STEPS ; step++){
			for(int queen = 0; queen < N; queen++){
				totalconflicts = totalconflicts + queenLocations[queen].conflictsCount;
			}
			if(totalconflicts == 0){
				//print the result - Final
				System.out.println("FOUND SOLUTION");
				for (int rowIndex = 0; rowIndex < N; rowIndex++) {
					for (int columnIndex = 0; columnIndex < N; columnIndex++) {
						System.out.print(" " + solution[rowIndex][columnIndex]);
					}
					System.out.println();
				}	
				return;
			}
			//Resetting totalconflicts
			totalconflicts = 0;
			moveQueens(queenLocations);
			updateConflicts(queenLocations);
			printMatrix(solution,N);
			//Printing the current queen locations
			printQueenLocations(queenLocations);
		}
		//Here reached means we did not find any solution
		System.out.println("NO SOLUTION FOUND BY RUNNING - "+MAX_STEPS+" times");
	}
	/*
	 * Prints the matrix for debugging purpose 
	 */
	public void printMatrix(int[][] matrix,int N){
		System.out.println("-----------------------");
		for (int rowIndex = 0; rowIndex < N; rowIndex++) {
			for (int columnIndex = 0; columnIndex < N; columnIndex++) {
				System.out.print(" " + matrix[rowIndex][columnIndex]);
			}
			System.out.println();
		}
		System.out.println("-----------------------");
	}
	/*
	 * Prints the MatrixLocation custom variable for debugging purpose.
	 * One single run of this method gives out locations of all queens and the conflicts count of the same location 
	 * with current assignment. 
	 */
	public void printQueenLocations(MatrixLocation[] queenLocations){
		for(int queen = 0;queen < queenLocations.length ;queen++){
			//prints queen location and conflicts count corresponing to that locatio
			System.out.println("("+queenLocations[queen].row+","+queenLocations[queen].column+")"
					+ " - "+queenLocations[queen].conflictsCount);
		}
	}
	/*
	 * This method is for moving the queen location which has maximum conflicts count.
	 * First of all it sorts MatrixLocation custom variables in an order of conflictsCount.
	 * Then select the  MatrixLocation variable which has maximum conflictscount and moves its location 
	 * one step over the row(In one run of this method)
	 * This method clears the conflictscount of all MatrixLocation variables,so a call to updateConflicts is mandatory
	 * after a call to this method.This is due to the reason that,any small change in any of the queen location may 
	 * alter the conflicts count of every queen locations.
	 */
	public void moveQueens(MatrixLocation[] queenLocations){
		System.out.println("Sorting(ascending) the matrix location objects based on the conflicts count");
		Arrays.sort(queenLocations, new MatrixLocComparator());
		System.out.println("Printing the conflicts count for all locations");
		printQueenLocations(queenLocations);
		System.out.println("Move the max conflicted queeen at:"
				+ "("+queenLocations[queenLocations.length -1].row+","
						+ ""+queenLocations[queenLocations.length -1].column+") by one row");
		if(queenLocations[queenLocations.length -1].row == 0){
			solution[queenLocations[queenLocations.length -1].row][queenLocations[queenLocations.length -1].column] = 0;
			(queenLocations[queenLocations.length -1].row)++;
		    solution[queenLocations[queenLocations.length -1].row][queenLocations[queenLocations.length -1].column] = 1;
		    assignments++;
		}
		else if(queenLocations[queenLocations.length -1].row == queenLocations.length -1){
			solution[queenLocations[queenLocations.length -1].row][queenLocations[queenLocations.length -1].column] = 0;
			(queenLocations[queenLocations.length -1].row)--;
		    solution[queenLocations[queenLocations.length -1].row][queenLocations[queenLocations.length -1].column] = 1;
		    assignments++;
		}
		else{
			//this can be decrement too - random selection
			solution[queenLocations[queenLocations.length -1].row][queenLocations[queenLocations.length -1].column] = 0;
			(queenLocations[queenLocations.length -1].row)++;
		    solution[queenLocations[queenLocations.length -1].row][queenLocations[queenLocations.length -1].column] = 1;
		    assignments++;
		}
		//Reset conflicts count for all queen location after movement - updateConflicts needs to be called
		//after a call to this method
		System.out.println("Resetting conflicts count for all locations after movement");
		for(int queen = 0;queen  < queenLocations.length;queen ++){
			queenLocations[queen].conflictsCount = 0;
		}
	}
	/*
	 * Sets number of conflicts for each queen(Location)/MatrixLocation custom variables.
	 * This method checks for row wise constraints,left and right upper & lower diagonal constraints too.
	 */
	public void updateConflicts(MatrixLocation[] queenLocations) {
		
		for(int numQueens = 0;numQueens < queenLocations.length;numQueens ++){
		
			int row = queenLocations[numQueens].row;
			int column = queenLocations[numQueens].column;
			
			System.out.println("***Counting conflicts for queenLocation : row column - "+row+":"+column);
			
			//System.out.println("Counting row wise conflicts");
			for (int rowIndex = 0; rowIndex < solution.length; rowIndex++) {
				//Finding OTHER(rowIndex!= column) 1's locations in the same row
				if (solution[row][rowIndex] == 1 && rowIndex!= column) {
					System.out.println("Row conflict found");
					queenLocations[numQueens].conflictsCount++;
				}
			}
			//System.out.println("Counting left upper diagonal conflicts");
			for (int rowIndex = row, columnIndex = column; rowIndex >= 0 && columnIndex >= 0; rowIndex--, columnIndex--) {
				if (solution[rowIndex][columnIndex] == 1 && rowIndex!= row && columnIndex!= column) {
					System.out.println("Left upper diagonal conflict found");
					queenLocations[numQueens].conflictsCount++;
				}
			}
			
			//System.out.println("Counting right lower diagonal conflicts");
			for (int rowIndex = row, columnIndex = column; rowIndex < solution.length && columnIndex < solution.length ; rowIndex++, columnIndex++) {
				if (solution[rowIndex][columnIndex] == 1 && rowIndex!= row && columnIndex!= column) {
					System.out.println("Right lower diagonal conflict found");
					queenLocations[numQueens].conflictsCount++;
				}
			}		
	
			//System.out.println("Counting left lower diagonal conflicts");
			for (int rowIndex = row, columnIndex = column; rowIndex < solution.length && columnIndex >= 0; rowIndex++, columnIndex--) {
				if (solution[rowIndex][columnIndex] == 1 && rowIndex!= row && columnIndex!= column ) {
					System.out.println("Left lower diagonal conflict found");
					queenLocations[numQueens].conflictsCount++;
				}
			}
			
			//System.out.println("Counting right upper diagonal conflicts");
			for (int rowIndex = row, columnIndex = column; rowIndex >= 0 && columnIndex < solution.length ; rowIndex--, columnIndex++) {
				if (solution[rowIndex][columnIndex] == 1 && rowIndex!= row && columnIndex!= column) {
					System.out.println("Right upper diagonal conflict found");
					queenLocations[numQueens].conflictsCount++;
				}
			}
			System.out.println("***Conflicts count for queenLocation : row column - "
					+ ""+row+":"+column+" is - "+queenLocations[numQueens].conflictsCount);
		}	
	}
	public static void main(String[] args) {
		//Need to enter N manually
		int N = 5;
		long startTime = System.currentTimeMillis();	
		NQueenMinconflicts q = new NQueenMinconflicts(N);
		q.solve(N);
		long endTime = System.currentTimeMillis();
		System.out.println("Took "+(endTime - startTime) + " ms");
		System.out.println("Total number of assignments for - "+N+"-queen problem is : "+assignments);
	}

}
