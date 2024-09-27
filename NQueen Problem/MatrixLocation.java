/**
 * Matrix location as a custom variable.
 * Used for the Minconflicts local search for NQueen.
 * 
 *
 */
public class MatrixLocation{

	  public int row;
	  public int column;
	  //Count of conflicts for this particular location
	  public int conflictsCount;

	  public MatrixLocation(int row,int column) {
	    this.row = row;
	    this.column = column;
	    //Initial value is set to zero
	    this.conflictsCount = 0;
	  }

	  public int getLeft() { return row; }
	  public int getRight() { return column; }
	  public int getConflictsCount() { return conflictsCount; }

	  @Override
	  public boolean equals(Object o) {
	    if (!(o instanceof MatrixLocation)) return false;
	    MatrixLocation pairo = (MatrixLocation) o;
	    if((this.row == pairo.row) && (this.column == pairo.column))
			return true;
		else
			return false;
	  }
	}
