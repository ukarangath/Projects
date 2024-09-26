import java.util.Comparator;
/**
 * @author Unnikrishnan 
 * 
 * @purpose File for Question Number 3: Compare matrix locations to sort according to the 
 * conflicts count for particular location.Used for the Minconflicts local search for NQueen.
 *
 */
public class MatrixLocComparator implements Comparator<MatrixLocation> {

	@Override
	public int compare(MatrixLocation L1, MatrixLocation L2) {
		if(L1.conflictsCount < L2.conflictsCount)
			return -1;
		else if(L1.conflictsCount > L2.conflictsCount)
			return 1;
		else
			return 0;
	}

}
