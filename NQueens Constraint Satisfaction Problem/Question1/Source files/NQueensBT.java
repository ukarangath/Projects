
import java.util.Arrays;

/**
 * @author Unnikrishnan
 * 
 * @purpose : Main File for Question Number 1 : NQueens solution using back tracking,rewritten in java
 * 
 * Note: debug-prints are retained in the code,this will help to understand implementation.
 * Needs to enter value of N manually at main() method.
 *
 */

public class NQueensBT {
	public int[][] solution;
	public static int assignments;
	/*
	 * Constructor 
	 */
	public NQueensBT(int N) {
		solution = new int[N][N];
		for (int row = 0; row < N; row++) {
			for (int column = 0; column < N; column++) {
				solution[row][column] = 0;
			}
		}
	}
	/*
	 * Main method which prints the result if found a solution 
	 */	
	public void solve(int N) {
		//place N number of queens in N*N chess board
		if(placeQueens(0, N)){
			System.out.println("solve(N)");
			//print the result - Final
			for (int row = 0; row < N; row++) {
				for (int column = 0; column < N; column++) {
					System.out.print(" " + solution[row][column]);
				}
				System.out.println();
			}
		}else{
			System.out.println("NO SOLUTION EXISTS");
		}
	}
	/*
	 * Method which implements recursive back tracking algorithm 
	 */	
	public boolean placeQueens(int queen, int N) {
		System.out.println("--------------------");
		System.out.println("placeQueens(queen,N) - "+queen+":"+N);
		// will place the Queens one at a time, for column wise
		if(queen==N){
			//if we are here that means we have solved the problem
			System.out.println("placeQueens EXIT - true,queens == N");
			return true;
		}
		for (int row = 0; row < N; row++) {
			// check if queen can be placed at any row,if not fails and runs back track
			if (canPlace(solution, row, queen)) {
				// place the queen
				System.out.println("placeQueens - queen placed at - "+row+":"+queen);
				solution[row][queen] = 1;
				assignments++;
				// solve  for next queen - recursion
				if(placeQueens(queen+1, N)){
					System.out.println("placeQueens EXIT(unravelling) - true,queen - "+queen);
					return true;
				}
				//if we are here that means above placement didn't work
				//BACKTRACK - unraveling,only limit is until this for-loop bracket
				System.out.println("BACKTRACK : row column/queen - "+row+":"+queen);
				solution[row][queen]=0;
				assignments++;
			}
		}
		//if we are here that means we haven't found solution
		System.out.println("placeQueens EXIT - HAVE NOT found a solution : queen - "+queen);
		return false;

	}

	/*
	 * Method to check if queen can be placed at matrix[row][column] - column corresponds to queen here.
	 * Checks row wise and diagonal constraints too
	 */
	public boolean canPlace(int[][] matrix, int row, int column) {
		// since we are filling one column at a time,
		// we will check if no queen is placed in that particular row
		System.out.println("**canPlace(matrix,row,queen/column) ENTER: row column - "+row+":"+column);
		for (int rowIndex = 0; rowIndex < column; rowIndex++) {
			System.out.println("Entering row chk: row column - "+row+":"+rowIndex);
			if (matrix[row][rowIndex] == 1) {
				System.out.println("canPlace EXIT - Returning row : row column - "+row+":"+rowIndex);
				return false;
			}
		}
		// we are filling one column at a time,so we need to check the upper and
		// diagonal as well
		// check upper diagonal
		for (int rowIndex = row, columnIndex = column; rowIndex >= 0 && columnIndex >= 0; rowIndex--, columnIndex--) {
			System.out.println("Entering upper diagonal chk : row column - "+rowIndex+":"+columnIndex);
			if (matrix[rowIndex][columnIndex] == 1) {
				System.out.println("canPlace EXIT - Returning upper diagonal : row column - "+rowIndex+":"+columnIndex);
				return false;
			}
		}

		// check lower diagonal
		for (int rowIndex = row, columnIndex = column; rowIndex < matrix.length && columnIndex >= 0; rowIndex++, columnIndex--) {
			System.out.println("Entering lower diagonal chk: row column - "+rowIndex+":"+columnIndex);
			if (matrix[rowIndex][columnIndex] == 1) {
				System.out.println("canPlace EXIT - Returning lower diagonal : row column - "+rowIndex+":"+columnIndex);
				return false;
			}
		}
		// if we are here that means we are safe to place Queen at row,column
		System.out.println("canPlace EXIT - FINAL : safe to place Queen at row,column - "+ row+":"+ column);
		return true;
	}

	public static void main(String[] args) {
		//Needs to enter value of N manually
		int N = 8;
		NQueensBT q = new NQueensBT(N);
		q.solve(N);
		System.out.println("Total number of assignments in solving "+N+"-queen problem is : "+assignments);
	}

}
