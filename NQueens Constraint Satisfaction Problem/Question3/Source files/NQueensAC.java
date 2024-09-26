import java.util.Arrays;
/**
 * @author Unnikrishnan 
 * 
 * @purpose : Main File for Question Number 3 : NQueens solution using arc-consistency checking
 * 
 * Note: debug-prints are retained in the code,this will help to understand implementation.
 * Needs to enter value of N manually at main() method.
 *
 */
public class NQueensAC {
	public int[][] solution;
	public static int assignments;
	//LCV
	public static int[] alteredRow;

	/*
	 * Constructor 
	 */	
	public NQueensAC(int N) {
		solution = new int[N][N];
		for (int rowIndex = 0; rowIndex < N; rowIndex++) {
			for (int columnIndex = 0; columnIndex < N; columnIndex++) {
				solution[rowIndex][columnIndex] = 0;
			}
		}
	}
	/*
	 * Main method which prints the result if found a solution.
	 * This method calls arcConsistencyInit,which initiates arc consistency check 
	 */		
	public void solve(int N) {
		//place N number of queens in N*N chess board
		if(arcConsistencyInit()){
			//print the result - Final
			System.out.println("SOLUTION MATRIX");
			for (int rowIndex = 0; rowIndex < N; rowIndex++) {
				for (int columnIndex = 0; columnIndex < N; columnIndex++) {
					System.out.print(" " + solution[rowIndex][columnIndex]);
				}
				System.out.println();
			}
		}else{
			System.out.println("NO SOLUTION EXISTS");
		}
	}
	/*
	 * Method for debugging purpose
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
	 * This method checks diagonal(right upper and right lower) and row wise constraints for a particular matrix location.
	 * Diagonal constraints are checked in right direction as the assignment happens in normal column order,low to high.
	 * If one location is assigned with '1',it means queen is kept there.This method marks the diagonal and row constraint 
	 * locations with value '2'(Constraint location).If another assignment triggers constraint at same location then previous value is incremented with 2.
	 * Similarly if queen is moved out from that location(value turns from 1 to 0) then all constraint locations are unchecked
	 * by decrementing location's value by 2.This is just a convention used here.
	 * This method has a flag,it determines what operation needs tobe done
	 * flag= 0 - Unset constraints
	 * flag = 1 - Set constraints
	 * flag = 2 - Clearing off the matrix after finding a solution,deleting all values > 1 to zero.
	 * 
	 * Debug prints given for more clarification
	 */
	public void altLocConstraints(int flag,int[][] matrixConsistency,int row,int column)
	{
		if(column == (matrixConsistency.length - 1))
		{
			System.out.println("Last colum doesnt have to set any constraints,exit");
			return;
		}
		//Set constraints for the loc: row,column
		if(flag == 1)
		{
			System.out.println("Setting constraints for loc :"+row+":"+column);
			//clearing that full row to 2(invalid locations)
			for (int rowIndex = column + 1; rowIndex < matrixConsistency.length ; rowIndex++)
				matrixConsistency[row][rowIndex] = matrixConsistency[row][rowIndex] + 2;
			//making all right upper diagonal locations of (0,0) invalid(value 2)
			for (int rowIndex = row, columnIndex = column; rowIndex >= 0 && columnIndex < matrixConsistency.length ; rowIndex--, columnIndex++)
				matrixConsistency[rowIndex][columnIndex] = matrixConsistency[rowIndex][columnIndex] + 2;
			// making all right lower diagonal locations of (0,0) invalid(value 2)
			for (int rowIndex = row, columnIndex = column; rowIndex < matrixConsistency.length && columnIndex < matrixConsistency.length ; rowIndex++, columnIndex++)
				matrixConsistency[rowIndex][columnIndex] =  matrixConsistency[rowIndex][columnIndex] + 2;
		}
		//Uncheck constraints for the loc: row,column
		else if(flag == 0)
		{
			System.out.println("Unchecking constraints for loc :"+row+":"+column);
			//row clearing
			for (int rowIndex = column + 1; rowIndex < matrixConsistency.length ; rowIndex++)
				matrixConsistency[row][rowIndex] = matrixConsistency[row][rowIndex] - 2;
			//making all right upper diagonal locations of (0,0) invalid(value 2)
			for (int rowIndex = row, columnIndex = column; rowIndex >= 0 && columnIndex < matrixConsistency.length ; rowIndex--, columnIndex++)
				matrixConsistency[rowIndex][columnIndex] = matrixConsistency[rowIndex][columnIndex] -2 ;
			// making all right lower diagonal locations of (0,0) invalid(value 2)
			for (int rowIndex = row, columnIndex = column; rowIndex < matrixConsistency.length && columnIndex < matrixConsistency.length ; rowIndex++, columnIndex++)
				matrixConsistency[rowIndex][columnIndex] = matrixConsistency[rowIndex][columnIndex] -2 ;
		}	
		else if(flag ==2)
		{
			System.out.println("Clearing off the matrix after finding a solution");
			for (int rowIndex = 0; rowIndex < matrixConsistency.length; rowIndex++) {
				for (int columnIndex = 0; columnIndex < matrixConsistency.length; columnIndex++) {
					if(matrixConsistency[rowIndex][columnIndex] > 1)
						matrixConsistency[rowIndex][columnIndex] = 0;
				}
			}
		}
	}
	/*
	 * This method is the initialization step for arc consistency.
	 * Generates a matrix of size N and assigns first queen at  leftmost top location,(0,0).
	 * This method calls recursive arcConsistensyRec method.
	 * If arcConsistensyRec returns true then there is a solution,if not No solution exists.
	 */
	public boolean arcConsistencyInit(){
		int N = solution.length;
		int[][] matrixConsistency = new int[N][N];
		//Initial value assignment 
		int queen = 0;
		int row = 0;
		altLocConstraints(1,matrixConsistency,0,0);
		//first assignment at (0,0) -> 1,first queen placed at (0,0),doing this assignment after all
		//diagonal check to avoid the rewriting to 2 at that location
		matrixConsistency[row][queen] = 1;
		assignments++;
		System.out.println("Starting");
		printMatrix(matrixConsistency,N);
		//Recursion starts here - ARC Consistency check
		if(arcConsistensyRec(matrixConsistency,queen +1))
		{
			solution = matrixConsistency;
			return true;
		}
		else
		{
			System.out.println("No solution exists");
			return false;
		}
	}
	/*
	 * Method which implements arc consistency check(AC3) that is forward checking + with look ahead,
	 * which means it checks till it finds a solution.
	 * This starts with a matrix of size N whose first location (0,0) is assigned with first queen.
	 * Execution algorithm has following steps:
	 * 1.For next column/queen,checks for free slots(0)
	 * 2.If free slots available then puts queen there and checks consistency with next column
	 * 3.If consistency check fails/no slots for next column then back track previous assignment
	 * 4.If consistency check passes then proceed with next queen.
	 * 5.If no slots available at current column then go back to previous column and alter the assignment of the 
	 *   queen there,if there are no open positions there then keep going until it reaches first column.
	 * 6.If queen is successfully assigned at last column then it is the solution.If back tracking 
	 * reaches first column with out any open slots then solution does not exist.
	 * 
	 * Note : If a previous assignment at column C fails then that position is given with a value '3'(Failed location).
	 * This is just a convention used in this program.
	 * Location with a value 2 - constrained location.
	 * Location with a value 3 - Invalid/failed location
	 * 
	 * Note: Observe debug prints before solution.
	 * 
	 */
	public boolean arcConsistensyRec(int[][] matrixConsistency,int queen){
		int row = 0;
		boolean slotAvailable = false;
		for(row = 0; row< matrixConsistency.length; row++)
		{
			if(matrixConsistency[row][queen] == 1)
			{
				System.out.println("Found previous assignment at :"+row+":"+queen);
				System.out.println("Checking whether free slot available at this column after that row slot");
				for(int indexRow = row + 1; indexRow< matrixConsistency.length; indexRow++)
				{	
					if(matrixConsistency[indexRow][queen] == 0)
					{
						System.out.println("back track previous 1's assignment at : "+row+":"+queen);
						altLocConstraints(0,matrixConsistency,row,queen);
						System.out.println("marking this previous 1's assignment location as invalid '3'");
						matrixConsistency[row][queen] = 3;	
						assignments++;
						printMatrix(matrixConsistency,matrixConsistency.length);
						row = indexRow;
						System.out.println("Giving next open slot at : "+row+":"+queen);
						slotAvailable = true;
						break;
					}
				}
				System.out.println("No open slot at :"+queen);
				printMatrix(matrixConsistency,matrixConsistency.length);
			}				
			if(matrixConsistency[row][queen] == 0)
			{
				System.out.println("slot available at :"+row+":"+queen);
				printMatrix(matrixConsistency,matrixConsistency.length);
				slotAvailable = true;
				break;
			}
		}
		if(slotAvailable)
		{
			altLocConstraints(1,matrixConsistency,row,queen);
			matrixConsistency[row][queen] = 1;
			assignments++;
			System.out.println("filled availableslot :");
			printMatrix(matrixConsistency,matrixConsistency.length);
			
			//Exit check for recursion
			if(queen == (matrixConsistency.length - 1))
			{
				System.out.println("Last colum,exiting");
				//clearing solution matrix
				altLocConstraints(2,matrixConsistency,0,0);
				return true;
			}
			
			boolean slotAvailableNext = false;
			//checks whether next column has all places invalid to place next queen
			for(int indexRow = 0; indexRow< matrixConsistency.length; indexRow++)
				if(matrixConsistency[indexRow][queen + 1] == 0)
					slotAvailableNext = true;
			System.out.println("slot available next column :"+(queen + 1)+":"+slotAvailableNext);
			if(!slotAvailableNext)
			{
				System.out.println("back track : "+row+":"+queen);
				altLocConstraints(0,matrixConsistency,row,queen);
				//back track previous assignment and mark this location invalid
				matrixConsistency[row][queen] = 3;
				assignments++;
				//recursive call for same queen assignment
				System.out.println("recursive call for same queen assignment : "+queen);
				printMatrix(matrixConsistency,matrixConsistency.length);
				arcConsistensyRec(matrixConsistency,queen);
			}
			//slot is available in next column
			else
			{
				System.out.println("went smooth,next is : "+(queen+1)+"-queen");
				printMatrix(matrixConsistency,matrixConsistency.length);
				//recursive call for next queen assignment
				arcConsistensyRec(matrixConsistency,queen + 1);
			}
		}
		//no place vacant in this column,back track previous column
		else
		{
			System.out.println("No open slot at column: "+queen);
			printMatrix(matrixConsistency,matrixConsistency.length);
			System.out.println("Clearing off all 3s and 1s from current column:"+queen);
			//clearing off all '3' and '1's values from the column(clearing previous assignments and invalid locations)
			for(int indexRow = 0; indexRow< matrixConsistency.length; indexRow++){
				if(matrixConsistency[indexRow][queen] == 3)
					matrixConsistency[indexRow][queen] = 0;
				if(matrixConsistency[indexRow][queen] == 1){
					System.out.println("Clearing off the constraints of 1's");
					altLocConstraints(0,matrixConsistency,indexRow,queen);
					matrixConsistency[indexRow][queen] = 0;
					assignments++;
				}
			}
			if(queen > 0)
					queen--;
			else if(queen == 0)
			{//no solution
				System.out.println("No solution,queen == 0 and no slots");
				return false;
			}
			printMatrix(matrixConsistency,matrixConsistency.length);	
			System.out.println("recursive call for previous queen assignment : "+queen);
			arcConsistensyRec(matrixConsistency,queen);
			}
		return true;
	}
	public static void main(String[] args) {
		//Needs to enter value of N manually
		int N = 10;
		NQueensAC q = new NQueensAC(N);
		q.solve(N);
		System.out.println("Total number of assignments in solving "+N+"-queen problem is : "+assignments);
	}

}