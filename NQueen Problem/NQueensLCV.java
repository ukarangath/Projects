import java.util.Arrays;
/**
 * NQueens solution using least-constraining-value heuristic.
 * Needs to enter value of N manually at main() method.
 *
 */
public class NQueensLCV {
	public int[][] solution;
	public static int assignments;
	//array of altered order for least-constraining-value heuristic
	public static int[] alteredOrder;
	
	/*
	 * Constructor 
	 */	
	public NQueensLCV(int N) {
		solution = new int[N][N];
		for (int rowIndex = 0; rowIndex < N; rowIndex++) {
			for (int j = 0; j < N; j++) {
				solution[rowIndex][j] = 0;
			}
		}
	}
	/*
	 * Method which generates the most constrained variable order of the column locations.
	 * If there are 5 queens/columns-then order of assigning will be - 2,3,1,4,0,5.
	 * During a run debug prints will give clear idea of the same.
	 */
	public void alterRowOrder(int N){
		alteredOrder = new int[N];
		int index = 0;
		for(int value = 0; value < N ; value++)
		{
			if(value % 2 != 0)
				alteredOrder[index++] = value;
		}
		for(int value = 0; value < N ; value++)
		{
			if(value % 2 == 0)
				alteredOrder[index++] = value;
		}
		System.out.println("row : "+Arrays.toString(alteredOrder));		
	}
	
	/*
	 * Main method which prints the result if found a solution 
	 */	
	public void solve(int N) {
		//LCV
		alterRowOrder(N);
		//place N number of queens in N*N chess board
		if(placeQueens(0, N)){
			System.out.println("solve(N)");
			//print the result - Final
			for (int rowIndex = 0; rowIndex < N; rowIndex++) {
				for (int j = 0; j < N; j++) {
					System.out.print(" " + solution[rowIndex][j]);
				}
				System.out.println();
			}
		}else{
			System.out.println("NO SOLUTION EXISTS");
		}
	}
	/*
	 * Method which implements least-constraining-value heuristic for assignment of the queens.
	 * Queens are assigned using the altered order given by the array - alteredOrder
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
			if (canPlace(solution, row, alteredOrder[queen])) {
				// place the queen
				System.out.println("placeQueens - queen placed at - "+row+":"+alteredOrder[queen]);
				solution[row][alteredOrder[queen]] = 1;
				assignments++;
				// solve  for next queen - recursion
				if(placeQueens(queen+1, N)){
					System.out.println("placeQueens EXIT(unravelling) - true,queen - "+alteredOrder[queen]);
					return true;
				}
				//if we are here that means above placement didn't work
				//BACKTRACK - unraveling,only limit is until this for-loop bracket
				System.out.println("BACKTRACK : row column/queen - "+row+":"+alteredOrder[queen]);
				solution[row][alteredOrder[queen]] = 0;
				assignments++;
			}
		}
		//if we are here that means we haven't found solution
		System.out.println("placeQueens EXIT - HAVE NOT found a solution : queen - "+alteredOrder[queen]);
		return false;

	}

	/*
	 * Method to check if queen can be placed at matrix[row][column] - column corresponds to queen here.
	 * Checks row wise and diagonal constraints too.
	 * Need to check all diagonal locations(left and right as well) due the the altered order assignment of queens.
	 * Debug prints are given for better understanding.
	 */
	public boolean canPlace(int[][] matrix, int row, int column) {
		// since we are filling one column at a time,
		// we will check if no queen is placed in that particular row
		System.out.println("**canPlace(matrix,row,queen/column) ENTER: row column - "+row+":"+column);
		for (int rowIndex = 0; rowIndex < matrix.length; rowIndex++) {
			System.out.println("Entering row chk: row column - "+row+":"+rowIndex);
			if (matrix[row][rowIndex] == 1) {
				System.out.println("canPlace EXIT - Returning row : row column - "+row+":"+rowIndex);
				return false;
			}
		}
		// we are filling one column at a time,so we need to check the upper and
		// diagonal as well
		// check left upper diagonal
		for (int rowIndex = row, j = column; rowIndex >= 0 && j >= 0; rowIndex--, j--) {
			System.out.println("Entering upper diagonal chk : row column - "+rowIndex+":"+j);
			if (matrix[rowIndex][j] == 1) {
				System.out.println("canPlace EXIT - Returning upper diagonal : row column - "+rowIndex+":"+j);
				return false;
			}
		}
		
		// check right lower diagonal
		for (int rowIndex = row, j = column; rowIndex < matrix.length && j < matrix.length ; rowIndex++, j++) {
			System.out.println("Entering upper diagonal chk : row column - "+rowIndex+":"+j);
			if (matrix[rowIndex][j] == 1) {
				System.out.println("canPlace EXIT - Returning upper diagonal : row column - "+rowIndex+":"+j);
				return false;
			}
		}		

		// check left lower diagonal
		for (int rowIndex = row, j = column; rowIndex < matrix.length && j >= 0; rowIndex++, j--) {
			System.out.println("Entering lower diagonal chk: row column - "+rowIndex+":"+j);
			if (matrix[rowIndex][j] == 1) {
				System.out.println("canPlace EXIT - Returning lower diagonal : row column - "+rowIndex+":"+j);
				return false;
			}
		}
		
		// check right upper diagonal
		for (int rowIndex = row, j = column; rowIndex >= 0 && j < matrix.length ; rowIndex--, j++) {
			System.out.println("Entering lower diagonal chk: row column - "+rowIndex+":"+j);
			if (matrix[rowIndex][j] == 1) {
				System.out.println("canPlace EXIT - Returning lower diagonal : row column - "+rowIndex+":"+j);
				return false;
			}
		}
		// if we are here that means we are safe to place Queen at row,column
		System.out.println("canPlace EXIT - FINAL : safe to place Queen at row,column - "+ row+":"+ column);
		return true;
	}

	public static void main(String[] args) {
		//Needs to enter value of N manually
		int N = 4;
		NQueensLCV q = new NQueensLCV(N);
		q.solve(N);
		System.out.println("Total number of assignments in solving "+N+"-queen problem is : "+assignments);
	}

}