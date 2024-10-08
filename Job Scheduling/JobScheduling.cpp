﻿/* 
	Job scheduling
	This is a C++ source file, mostly written in C
	Author : Unnikrishnan Arangath
	Input : It is given in the file "input.txt",located in same directory as source
			file
	Output : Output is generated and loaded to an output.txt file in same direcotry
	Constraints: 
	1.Maximum characters in a line of input.txt is MAX_LINE_SIZE,leaving out a space between
	  start time and end time
	2.Maximum number of jobs allowed are MAX_NO_OF_JOBS.This is equivalent to number of lines
	  in input.txt + 1	
*/
#include<stdio.h> 
#include <string.h>
#include <stdlib.h>

// CONSTANTS
// maximum nunmber of characters in a line for input.txt
#define MAX_LINE_SIZE 7
// maximum number of jobs 
#define MAX_NO_OF_JOBS 100
//GLOBAL VARIABLES
// variables fetched from input.txt
// n - number of jobs,m - number of machines
int n, m;
/* 
  st is the array of start times of the n jobs in order,
  similarly et is the end time of n jobs.
  st and et used for processing.So a back up is kept of the same 
  as stBak and etBak
*/
int st[MAX_NO_OF_JOBS], et[MAX_NO_OF_JOBS], stBak[MAX_NO_OF_JOBS], etBak[MAX_NO_OF_JOBS];
// variables that are part of output(output.txt)
/* 
	Accepted job data type
	machineId - to which machine this particular job is assigned
	jobId - job index based on st/et
*/
struct acceptedJob
{
	int machineId;
	int jobIndex;
};
// allAcceptedJobs saves all jobs which are accepted 
struct acceptedJob allAcceptedJobs[MAX_NO_OF_JOBS];
// maximum number of accepted jobs with n jobs and m machines
int totalAcceptedJobs;

/******************************************************
	Reading input.txt file and fills up gloabl variables 
	like n,m,st,et and their back ups
	COMPLEXITY OF FUNCTION:O(n)
******************************************************/
void readInput()
{
	// input file is kept in current directory where this source file is kept
	static const char filename[] = "input.txt";
	FILE *file = fopen(filename, "r");
	if (file != NULL)
	{
		// MAX_LINE_SIZE means start time can max be a 3 digit number, so is for end time too
		char line[MAX_LINE_SIZE]; 
		int lineIndx = 0;
		int arrayIndex = 0;
		/* read a line */
		while (fgets(line, sizeof line, file) != NULL) //n+1
		{
			// splitting the string/line based on white spaces to get n,m,st and et
			lineIndx++;
			char *str_parts[2];
			int i = 0;
			str_parts[0] = strtok(line, " ");
			str_parts[1] = strtok(NULL, "/");
			// fetching n and m
			if (lineIndx == 1)
			{
				n = atoi(str_parts[0]);
				m = atoi(str_parts[1]);				
			}
			// filling up st and et arrays
			else
			{
				st[arrayIndex] = atoi(str_parts[0]);				
				et[arrayIndex] = atoi(str_parts[1]);
				//back up of st and et
				stBak[arrayIndex] = st[arrayIndex];
				etBak[arrayIndex] = et[arrayIndex];
				arrayIndex++;
			}
		}
		fclose(file);
	}
	else
	{
		perror(filename); 
	}
}

/******************************************************
	Returns position of the smallest element of et
	without considering invalid markings(-1)
	Returns:
	pos - smallest element's position in et
	COMPLEXITY OF FUNCTION:O(n)
*******************************************************/
int findPosOfSmallestElement() {
	int temp = INT_MAX;
	int pos = -1;
	for (int indx = 0; indx < n; indx++) {//n
		// et's position value has to be greater than zero to avoid -1/
		//invalid marking positions
		if ((et[indx] > 0) && (temp > et[indx])) {
			temp = et[indx];
			pos = indx;
		}
	}
	return pos;
}

/******************************************************
	Marks overlapping positions of current position 
	and current position by -1(invalid)
	Variables:
	position - position of the job based on arrays of 
	start time,st and arrays of end time,et
	COMPLEXITY OF FUNCTION:O(n)
******************************************************/
void markOverlappingpositions(int position)
{
	int ts = st[position];
	int te = et[position];
	for (int indx = 0; indx < n; indx++)//n
	{
		int t_s = st[indx];
		int t_e = et[indx];
		// leaving out same position and already invalid positions ,so extracting only the non 
		// overlapping positions to keep
		if ((indx != position) && (t_s > 0))
		{		
			// valid positions, nothing to do
			if ((t_e < ts) || (te < t_s))
			{

			}
			else
			{
				// invalid positions are being marked
				st[indx] = -1;
				et[indx] = -1;
			}
			
		}

	}
	//marking the collected smallest position to invalid in st and et
	st[position] = -1;
	et[position] = -1;
	// accepted job locations are marked invalid even in stBak and etBak
	stBak[position] = -1;
	etBak[position] = -1;
}

/******************************************************
	checks if any valid positions(not having value 
	of -1)in st and et
	Returns:
	boolean variable.If valid positions present 
	then this function returns TRUE,if not it 
	returns FALSE
	COMPLEXITY OF FUNCTION:O(n)
******************************************************/
bool checkForAnyValidPosition()
{
	for (int indx = 0; indx < n; indx++) {//n
		if (et[indx] > -1)
			// found atleast a valid position
			return true;
	}
	// no valid position found
	return false;
}

/******************************************************
	Loads st and et from stBak and etBak after 
	allotment of jobs is over for a machine id
	COMPLEXITY OF FUNCTION:O(n)
******************************************************/
void loadArraysFromBak()
{
	for (int indx = 0; indx < n; indx++) {//n
		st[indx] = stBak[indx];
		et[indx] = etBak[indx];
	}
}

/******************************************************
	Writes back output variables to output.txt file
	COMPLEXITY OF FUNCTION:O(totalAcceptedJobs)/O(n)
******************************************************/
void writeOutput()
{
	FILE *file = fopen("output.txt", "w");
	if (file == NULL)
	{
		printf("Error : could not create output.txt file");
		exit(1);
	}
	// first line of the file has total/maximum accepted jobs
	fprintf(file, "%d\n", totalAcceptedJobs);
	int curMachId = 1,machId;
	for (int indx = 0; indx < totalAcceptedJobs; indx++) {//totalAcceptedJobs/n
		machId = allAcceptedJobs[indx].machineId;
		if (machId != curMachId)
		{
			fprintf(file, "\n%d ", allAcceptedJobs[indx].jobIndex);
			curMachId = machId;
		}
		else
		{
			fprintf(file, "%d ", allAcceptedJobs[indx].jobIndex);
		}
	}
	fclose(file);
}

/******************************************************
	Main function
	COMPLEXITY OF Main()/whole program:O(m*n^2),
	O(number of machines * total number of jobs).
	Explanation:
	Each function call in main has O(n) complexity.
	But the nested while loop in "Actual processing" 
	stub has a complexity of O(m(n(n+n+n)+n), 
	which is prominent,that is effectively of O(n*n*m).
******************************************************/
int main()
{
	// loads data from input.txt file located in current directory
	readInput();//O(n)
	// Actual processing - START
	int posOfSmall = -1;
	int jobNumber = 0;
	int allottedMachineIndx = 1;
	while (allottedMachineIndx <= m)//m times/O(m)
	{
		//checkForAnyValidPosition has complexity of O(n),
		//max number of valid positions => n/number of jobs.
		//while loop runs for worst by n/number of jobs.
		while (checkForAnyValidPosition())//while loop - O(n)
		{
			posOfSmall = findPosOfSmallestElement();//O(n)						
			struct acceptedJob job;
			// posOfSmall+1 is used as actual index is array index+1
			job.jobIndex = posOfSmall+1;
			job.machineId = allottedMachineIndx;
			allAcceptedJobs[jobNumber] = job;
			markOverlappingpositions(posOfSmall);//O(n)
			jobNumber++;
		}
		// allotment for next machine id
		allottedMachineIndx++;
		// loading st and et from back ups
		loadArraysFromBak();//O(n)
	}
	// totalAcceptedJobs is same as jobNumber
	totalAcceptedJobs = jobNumber;
	// Actual processing - END
	// generating output.txt file
	writeOutput();//O(n)
	return 0;
}
