/*
	Link State Routing Simulator
	This is a C++ source file which contains all function definisions
	Author : Unnikrishnan
	IDE: Visual studio community 2019
*/

/*This block of code must be before including header files and here
This is to disable warnings and error generated via _CRT_SECURE_NO_DEPRECATE
(Only in the case of Visual studio latest versions)*/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h> 
#include <string.h>
#include <iostream>
//Project specific header file
#include "header.h"

using namespace std;

//GLOBAL VARIABLES
//Number of routers/vertices avaialble in the network
int numberOfRouters = 0;
//If source router is selected by user(option 2) then selectedSourceRouterIndex value will be a non zero value
int selectedSourceRouterIndex = 0;
//If destination router is selected by user(option 3) then selectedDestinationRouterIndex value will be a non zero value
int selectedDestinationRouterIndex = 0;

/********************* FUNCTION DECLARATIONS RELATED TO LINK STATE ROUTING ALGORITHM ************************/
/************************************************************************************************************
	Displays all paths from the source router to the destination router. This function is only called by
	the function getPathsFromSourceToDestination. The idea used here is DFS.
	Returns:
		Displays all paths from sourceRouterIndex to destinationRouterIndex
	Input:
		sourceRouterIndex - source router index is either selected via option 2 or default source router
							index
		destinationRouterIndex - destination router index is selected or modified via option 3
		topologyMatrix - network topology matrix
************************************************************************************************************/
void displayAllPaths(int sourceRouterIndex, int destinationRouterIndex, int** topologyMatrix)
{
	/*No need to check the values of sourceRouterIndex & destinationRouterIndex, as those values 
	are checked already in function getPathsFromSourceToDestination, which is the caller of this 
	function*/
	/*This array holds the whether a router with particular index is visited(1) or not(0). Initially
	all routr indices are un-visited => 0*. Here can not create a static array but can create a 
	dynamic array of variable size 'numberOfRouters'*/
	int *visited = NULL;
	/*Calloc allocates memory and set all values to zero*/
	visited = (int*)calloc(numberOfRouters, sizeof(int));
	//Always number of routers is more than 3 to run the algorithm
	if((visited == NULL) || (visited[2] != 0))
	{
		cout << "Error : Memory allocation failed, Try again!" << endl;
		return;
	}
	/*path stores actual vertices and pathIndx is current index in path*/
	int* path = NULL;
	path = (int*)calloc(numberOfRouters, sizeof(int));
	if ((path == NULL) || (path[2] != 0))
	{
		cout << "Error : Memory allocation failed, Try again!" << endl;
		return;
	}
	int pathIndx = 0;
	/*Changing all indices to array based indices*/
	int sRI = sourceRouterIndex - 1;
	int dRI = destinationRouterIndex - 1;
	/*Calls the helper recursive function which handles further part of this function*/
	int indx = 0;
	cout << "\nAll paths between router " << sourceRouterIndex << " and " << destinationRouterIndex << " are:" << endl;
	cout << "[" << endl;
	displayAllPathsHelper(sRI, dRI, visited, path, topologyMatrix, pathIndx);
	cout << "]" << endl;
	//Freeing heap memory
	free(visited);
	free(path);
}

/************************************************************************************************************
	This function is a helper function to displayAllPaths. This is a recursive function which uses DFS 
	algorithm to print out all paths between source and destination router
	Returns:
		Displays all paths from sourceRouterIndex to destinationRouterIndex
	Input:
		sRI - (source router index - 1) is either selected via option 2 or default source router
							index
		dRI - (destination router index - 1) is selected or modified via option 3
		tM - network topology matrix
************************************************************************************************************/
void displayAllPathsHelper(int sRI, int dRI, int* visited, int* path, int** tM ,int& pathIndx)
{
	// Mark the current node and store it in path[] 
	visited[sRI] = 1;
	path[pathIndx] = sRI+1;
	pathIndx++;
	/*If current destination index is same as source index then print current path itself*/
	if (sRI == dRI)
	{
		cout << path[0];
		for (int indx = 1; indx < pathIndx; indx++)
			cout << "-" << path[indx] ;
		cout << endl;

	}
	else
	{
		for (int indx = 0; indx < numberOfRouters ; indx++)
		{
			/*A router has an adjacent path to another router means the cost of that edge \
			must be a value other than 0 and -1. Zero cost is for the same router to itself and
			-1 cost if there is no direct path between two routers. Note: Here negative edge
			costs are not abandoned*/
			if ((visited[indx] == 0) && (tM[sRI][indx] != 0) && (tM[sRI][indx] != -1))
				//Recursive call of displayAllPathsHelper
				displayAllPathsHelper(indx, dRI, visited, path, tM, pathIndx);
		}
	}
	//Remove current router index from path and mark it as unvisited 
	pathIndx--;
	visited[sRI] = 0;
}
/************************************************************************************************************
	This function finds shortest path between two router indices using Dijkstra's algorithm for single
	source shortest path. This function works in two modes,
	1. Mode = 1 => prints shortest path with respect to cost between two nodes sourceRouterIndex and 
	destinationRouterIndex, This is related to option 3 of main menu.
	2. Mode = 2 => prints the router which is best suitable for broadcast with costs to other nodes. It 
	is  the router which has shortest paths to all the other routers in the network along with the cost.
	The sum of the costs from this router to all other routers is the lowest. If more than one router 
	satisfies the criteria, chooses one arbitrarily. This is related to option 5 of main menu.
	Returns:
		Prints shortest path with cost or the router best suitable for broadcast with the cost to
		other routers
	Input:
		sourceRouterIndex - (source router index - 1) is either selected via option 2 or default source router
							index
		destinationRouterIndex - (destination router index - 1) is selected or modified via option 3
		topologyMatrix - network topology matrix
		mode - 1 for finding shortes path 2 for finding best suitable router for broadcast. In this case
		sourceRouterIndex and destinationRouterIndex values are invalid, can be given as 0
************************************************************************************************************/
void getShortestPath(int sourceRouterIndex, int destinationRouterIndex, int** topologyMatrix, int mode)
{
	int indx = 0;
	/*****Initialization step*****/
	/*An array that holds cumulative distance from the source node to all other nodes. Initial value is LARGEST_POSITIVE_WEIGHT which is
	equivalent to infinity*/
	int* cumDist = (int*)malloc(numberOfRouters*sizeof(int));
	if (cumDist != NULL)
		for (indx = 0; indx < numberOfRouters; indx++)
			cumDist[indx] = LARGEST_POSITIVE_WEIGHT;
	else
	{
		cout << "Error : Memory allocation failed, Try again!" << endl;
		return;
	}
	/*An array that holds the predecessor node for all other nodes. Initial value is INVALID_NUMBER which is a large negative 
	number and says that there is no predecessor node for the node under consideration*/
	int* predessorNode = (int*)calloc(numberOfRouters, sizeof(int));
	if (predessorNode != NULL)
		for (indx = 0; indx < numberOfRouters; indx++)
			predessorNode[indx] = INVALID_NUMBER;
	else
	{
		cout << "Error : Memory allocation failed, Try again!" << endl;
		return;
	}
	/*An array that holds if a node is visited or not. Visited nodes are marked with a value of 1, if not visited 
	then default value is 0*/
	int* nodeVisitedInfo = (int*)calloc(numberOfRouters, sizeof(int));
	//There are minimum of 3 nodes in the topology always
	if (nodeVisitedInfo == NULL || nodeVisitedInfo[2] != 0)
	{		cout << "Error : Memory allocation failed, Try again!" << endl;
			return;
	}
	//For mode = 1, just shortest path finder between source and destination
	if (mode == 1)
	{
		//Changing source and destination indices to array based ones
		int sourceRouterArrIndx = sourceRouterIndex - 1;
		int destRouterArrIndx = destinationRouterIndex - 1;
		//Initially only starting node mark as visited and mark its cumulative distance to itself as 0
		nodeVisitedInfo[sourceRouterArrIndx] = 1;
		cumDist[sourceRouterArrIndx] = 0;
		/*countOfVisitedNodes is the count of visited nodes. Source node is visited , so initial value is 1. countOfVisitedNodes
		follows index of numberOfRouters*/
		int countOfVisitedNodes = 1;
		//Initialization step: Below loop runs for all unvisited nodes
		for (indx = 1; indx <= numberOfRouters; indx++)
		{
			//There exists an edge from source to (indx+1) router
			if ((topologyMatrix[sourceRouterArrIndx][indx - 1] != 0) && (topologyMatrix[sourceRouterArrIndx][indx - 1] != -1))
			{
				//Cumulative distance to (indx+1) router
				cumDist[indx - 1] = topologyMatrix[sourceRouterArrIndx][indx - 1];
				//Current predecessor of (indx+1) router is source router
				predessorNode[indx - 1] = sourceRouterArrIndx;
			}

		}
		int leastCumDistNodeIndex = INVALID_NUMBER;
		//while(there are unvisited nodes)
		while (countOfVisitedNodes < numberOfRouters)
		{
			//Calls helper function which finds the node's index which has least cumulative distance among all unvisited nodes
			leastCumDistNodeIndex = getShortestPathHelper(countOfVisitedNodes, nodeVisitedInfo, cumDist);
			if(leastCumDistNodeIndex == INVALID_NUMBER)
			{
				cout << "Error : Memory allocation failed, Try again!" << endl;
				return;
			}
			//Marks as visited
			nodeVisitedInfo[leastCumDistNodeIndex] = 1;
			//Incrementing count of visited nodes
			countOfVisitedNodes++;
			//Traverse through all adjacent nodes of leastCumDistNodeIndex nodes which are not visited yet
			for (indx = 0; indx < numberOfRouters; indx++)
			{
				if ((topologyMatrix[leastCumDistNodeIndex][indx] != 0) && (topologyMatrix[leastCumDistNodeIndex][indx] != -1) &&
					(nodeVisitedInfo[indx] == 0))
				{
					if (cumDist[leastCumDistNodeIndex] + topologyMatrix[leastCumDistNodeIndex][indx] < cumDist[indx])
					{
						cumDist[indx] = cumDist[leastCumDistNodeIndex] + topologyMatrix[leastCumDistNodeIndex][indx];
						predessorNode[indx] = leastCumDistNodeIndex;
					}
				}
			}
		}
		/*Path to deistination comes in reverse way as we are tracking through the precessor array(predessorNode)
		, revPathToDestination array have the rverse paths which must be printed reverse to disply right path from
		source to destination*/
		int* revPathToDestination = (int*)malloc(numberOfRouters * sizeof(int));
		if (revPathToDestination == NULL)
		{
			cout << "Error : Memory allocation failed, Try again!" << endl;
			return;
		}
		//Initializing the indices with INVALID_NUMBER index, which is invalid(negative highest value)
		for (indx = 0; indx < numberOfRouters; indx++)
			revPathToDestination[indx] = INVALID_NUMBER;
		/*initially the nodeIndx/revPathToDestination[0] value is destination node index and it must reach source
		index in below loop*/
		int nodeIndx = destRouterArrIndx;
		revPathToDestination[0] = destRouterArrIndx;
		int pathLength = 0;
		while (nodeIndx != sourceRouterArrIndx)
		{
			nodeIndx = predessorNode[nodeIndx];
			/*No predecessor node found for destination rotuer, which means no path exists to it through source router*/
			if (nodeIndx == INVALID_NUMBER)
			{
				cout << "\nThere is no path between router " << sourceRouterIndex << " and " << destinationRouterIndex << " as"
					" they are not connected at all" << endl;
				return;
			}
			pathLength++;
			revPathToDestination[pathLength] = nodeIndx;
		}
		//Prints shortest path between source and destination router along with the cost
		cout << "\nShortest path between router " << sourceRouterIndex << " and " << destinationRouterIndex << " is:" << endl;
		printf("%d", (sourceRouterArrIndx + 1));
		for (indx = (numberOfRouters - 1); indx >= 0; indx--) {
			if ((revPathToDestination[indx] != sourceRouterArrIndx) && (revPathToDestination[indx] != INVALID_NUMBER))
			{
				printf("-");
				printf("%d", (revPathToDestination[indx] + 1));
			}
		}
		cout << "\nCorresponding path cost is: " << cumDist[destRouterArrIndx] << endl;
		//Freeing heap memory
		free(revPathToDestination);
		revPathToDestination = NULL;
	}
	/*For mode = 2, finds the node which is best suitable for broadcast, means it has minimum total path costs to 
	remaining nodes*/
	else if (mode == 2)
	{
		/*sumOfShortestDistOfAll contains each routers sum of shortest distance to all remaining nodes*/
		int* sumOfShortestDistOfAll = (int*)malloc(numberOfRouters * sizeof(int));
		if (sumOfShortestDistOfAll == NULL)
		{
			cout << "Error : Memory allocation failed, Try again!" << endl;
			return;
		}
		/*Intializing the value to max value, LARGEST_POSITIVE_WEIGHT(infinity) as we have to find the least of all of those values.
		A node whose sumOfShortestDistOfAll is LARGEST_POSITIVE_WEIGHT means that it does not have any path to any node, means if is
		just a complete disconnected node*/
		for (indx = 0; indx < numberOfRouters; indx++)
			sumOfShortestDistOfAll[indx] = LARGEST_POSITIVE_WEIGHT;
		/*Finds the sum of shortest path to all nodes from each node of the network topology and feeds the same to
		sumOfShortestDistOfAll*/
		int nodesIndex = 0;
		int cumSum = 0;
		for (nodesIndex = 0; nodesIndex < numberOfRouters; nodesIndex++)
		{
			//Initially only starting node mark as visited and mark its cumulative distance to itself as 0
			nodeVisitedInfo[nodesIndex] = 1;
			cumDist[nodesIndex] = 0;
			/*countOfVisitedNodes is the count of visited nodes. Source node is visited , so initial value is 1. countOfVisitedNodes
			follows index of numberOfRouters*/
			int countOfVisitedNodes = 1;
			//Initialization step: Below loop runs for all unvisited nodes
			for (indx = 1; indx <= numberOfRouters; indx++)
			{
				//There exists an edge from source to (indx+1) router
				if ((topologyMatrix[nodesIndex][indx - 1] != 0) && (topologyMatrix[nodesIndex][indx - 1] != -1))
				{
					//Cumulative distance to (indx+1) router
					cumDist[indx - 1] = topologyMatrix[nodesIndex][indx - 1];
					//Current predecessor of (indx+1) router is source router
					predessorNode[indx - 1] = nodesIndex;
				}

			}
			int leastCumDistNodeIndex = INVALID_NUMBER;
			//while(there are unvisited nodes)
			while (countOfVisitedNodes < numberOfRouters)
			{
				//Calls helper function which finds the node's index which has least cumulative distance among all unvisited nodes
				leastCumDistNodeIndex = getShortestPathHelper(countOfVisitedNodes, nodeVisitedInfo, cumDist);
				if (leastCumDistNodeIndex == INVALID_NUMBER)
				{
					cout << "Error : Memory allocation failed, Try again!" << endl;
					return;
				}
				//Marks as visited
				nodeVisitedInfo[leastCumDistNodeIndex] = 1;
				//Incrementing count of visited nodes
				countOfVisitedNodes++;
				//Traverse through all adjacent nodes of leastCumDistNodeIndex nodes which are not visited yet
				for (indx = 0; indx < numberOfRouters; indx++)
				{
					if ((topologyMatrix[leastCumDistNodeIndex][indx] != 0) && (topologyMatrix[leastCumDistNodeIndex][indx] != -1) &&
						(nodeVisitedInfo[indx] == 0))
					{
						if (cumDist[leastCumDistNodeIndex] + topologyMatrix[leastCumDistNodeIndex][indx] < cumDist[indx])
						{
							cumDist[indx] = cumDist[leastCumDistNodeIndex] + topologyMatrix[leastCumDistNodeIndex][indx];
							predessorNode[indx] = leastCumDistNodeIndex;
						}
					}
				}
			}
			for (indx = 0; indx < numberOfRouters; indx++)
			{
				if (cumDist[indx] == LARGEST_POSITIVE_WEIGHT)
				{
					cumSum = LARGEST_POSITIVE_WEIGHT;
					break;
				}
				else
					cumSum = cumSum + cumDist[indx];
			}
			sumOfShortestDistOfAll[nodesIndex] = cumSum;
			//Clearing all variable values before next iteration
			cumSum = 0;
			for (indx = 0; indx < numberOfRouters; indx++)
			{
				cumDist[indx] = LARGEST_POSITIVE_WEIGHT;
				predessorNode[indx] = INVALID_NUMBER;
				nodeVisitedInfo[indx] = 0;
			}
		}
		/*Finds the smallest of sumOfShortestDistOfAll, which corresponds to the best router for broadcast, which has least path
		cost to all other nodes*/
		//Initial assignment
		int smallestVal = sumOfShortestDistOfAll[0];
		int smallestIndex = 0;
		for (indx = 0; indx < numberOfRouters; indx++)
		{
			if (smallestVal > sumOfShortestDistOfAll[indx])
			{
				smallestVal = sumOfShortestDistOfAll[indx];
				//Saves the position of smallest among sumOfShortestDistOfAll
				smallestIndex = indx;
			}
		}
		if (sumOfShortestDistOfAll[smallestIndex] >= LARGEST_POSITIVE_WEIGHT)
		{
			cout << "\nAll routers in the network is disconnected from atleast one another router, thismeans no router "
				"is perfect for broadcast" << endl;
		}
		else
			cout << "\nBest router for broadcast is " << (smallestIndex + 1) << " with overall path "
				"cost as " << sumOfShortestDistOfAll[smallestIndex] << endl;
		//Freeing heap memory
		free(sumOfShortestDistOfAll);
		sumOfShortestDistOfAll = NULL;
	}
	//Freeing heap memory
	free(cumDist);
	cumDist = NULL;
	free(predessorNode);
	predessorNode = NULL;
	free(nodeVisitedInfo);
	nodeVisitedInfo = NULL;
}

/************************************************************************************************************
	This is a helper function to getShortestPath function, which helps the main function to find least 
	cumulative distance bearing node among unvisited nodes
	Returns:
		Returns the node index(wrt array[nodeVisitedInfo/cumDist])which had least cumulative distance among 
		all unvisited nodes.
	Input:
	    countOfVisitedNodes - 
		nodeVisitedInfo - An array that holds if a node is visited or not. Visited nodes are marked with a 
		value of 1, if not visited then default value is 0 
		cumDist - An array that holds cumulative distance from the source node to all other nodes.
************************************************************************************************************/
int getShortestPathHelper(int countOfVisitedNodes, int* nodeVisitedInfo, int* cumDist)
{
	/*countOfVisitedNodes is always less than numberOfRouters as this function is called from a  loop 
	whose condition is "while (countOfVisitedNodes < numberOfRouters)"*/
	int countOfUnvisitedNodes = numberOfRouters - countOfVisitedNodes;
	int indx = 0;
	/*If only one unvisited node is present then function returns the index of the same node
	as the node whose cumulative distance is the least among all unvisited nodes*/
	if (countOfUnvisitedNodes == 1)
		for (indx = 0; indx < numberOfRouters; indx++)
			if (nodeVisitedInfo[indx] == 0)
				return indx;
	//If countOfUnvisitedNodes > 1
	int* indicesOfUnvisitedNodes = (int*)malloc(countOfUnvisitedNodes * sizeof(int));
	if (indicesOfUnvisitedNodes == NULL)
	{
		cout << "Error : Memory allocation failed, Try again!" << endl;
		return INVALID_NUMBER;
	}
	int* cumDistOfUnvisitedNodes = (int*)malloc(countOfUnvisitedNodes * sizeof(int));
	if (cumDistOfUnvisitedNodes == NULL)
	{
		cout << "Error : Memory allocation failed, Try again!" << endl;
		return INVALID_NUMBER;
	}
	int indux = 0;
	//Saving unvisited locations and their cumulative distances
	for (indx = 0; indx < numberOfRouters; indx++)
		if (nodeVisitedInfo[indx] == 0)
		{
			indicesOfUnvisitedNodes[indux] = indx;
			cumDistOfUnvisitedNodes[indux] = cumDist[indx];
			indux++;
		}
	//Finds the smallest cumlative distance among unvisited nodes
	indux = 0;
	int smallestVal = cumDistOfUnvisitedNodes[0];
	int smallestIndex = 0;
	for (indux = 1; indux < countOfUnvisitedNodes; indux++) 
	{
		if (smallestVal > cumDistOfUnvisitedNodes[indux])
		{
			smallestVal = cumDistOfUnvisitedNodes[indux];
			//Saves the position of smallest cumulative distance wrt the array cumDistOfUnvisitedNodes
			smallestIndex = indux;
		}
	}
	/*Returns smallest cumulative distance's index wrt [nodeVisitedInfo and cumDist](same index), 
	i.e node's index which has least cumulative distance among all unvisited nodes*/
	int smallestUnvisitedCumDistNode = indicesOfUnvisitedNodes[smallestIndex];
	//Freeing heap memory
	free(indicesOfUnvisitedNodes);
	indicesOfUnvisitedNodes = NULL;
	free(cumDistOfUnvisitedNodes);
	cumDistOfUnvisitedNodes = NULL;
	return (smallestUnvisitedCumDistNode);
}

/****************************** FUNCTION DECLARATIONS FOR PROJECT FRAMEWORK ********************************/
/************************************************************************************************************
	Prints topology matrix based on numberOfRouters variable
	Returns:
		Prints topology matrix
	Input:
		topologyMatrix - network topology matrix
************************************************************************************************************/
void printTopologyMatrix(int** topologyMatrix)
{
	if (topologyMatrix == NULL)
	{
		cout << "\nError : Passed topology matrix is NULL, Run option 1" << endl;
		return;
	}
	cout << "\n  Topology Matrix" << endl;
	cout << "(Number of routers: " << numberOfRouters << ")" << endl;
	cout << "----------------------" << endl;
	int indx = 0, rowIndx = 0, colIndx = 0;
	for (rowIndx = 0; rowIndx < numberOfRouters; rowIndx++)
	{
		printf("  ");
		for (colIndx = 0; colIndx < numberOfRouters; colIndx++)
		{
			printf("%d", topologyMatrix[rowIndx][colIndx]);
			printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

/************************************************************************************************************
	Shows the router which has shortest paths to all the other routers in the network along with the cost.
	The sum of the costs from this router to all other routers is minimum. If more than one router satisfies
	the criteria, then chooses one among them arbitrarily.
	Returns:
		Prints best suitable router index for broadcast
	Input:
		topologyMatrix - generated network topology matrix
************************************************************************************************************/
void getBestRouterForBroadcast(int** topologyMatrix)
{
	cout << "\nBest Router for Broadcast" << endl;
	cout << "-------------------------" << endl;
	if (topologyMatrix == NULL)
	{
		cout << "\nError : Topology matrix is uninitialized, Run option 1" << endl;
		return;
	}
	//Finds best suitable router for broadcast, this function works for the same with mode value 2
	getShortestPath(0, 0, topologyMatrix, 2);
}

/************************************************************************************************************
	Edits topology matrix after initialization, there are two such operations allowed,
	1. Add a router/vertex
	2. Delete a router/vertex
	Returns:
		New modified Network topology matrix(updatedTopologyMatrix) or NULL, upon failure
	Input:
		topologyMatrix - generated network topology matrix
************************************************************************************************************/
int** updateNetworkTopology(int** topologyMatrix)
{
	cout << "\nUpdate Network Topology: Add or Delete a router from this network topology" << endl;
	cout << "--------------------------------------------------------------------------" << endl;
	if (topologyMatrix == NULL)
	{
		cout << "\nError : Topology matrix is uninitialized, Run option 1" << endl;
		return NULL;
	}
	int optionNum = 0;
	cout << "\nSelect one of below options to proceed further" << endl;
	cout << "1. Add a router" << endl;
	cout << "2. Delete a router" << endl;
	cin >> optionNum;
	//Fails if input is not an integer and out of range
	if (cin.fail() || optionNum < 1 || optionNum > 2)
	{
		//Clears error flags
		cin.clear();
		//Either next numeric_limits<streamsize>::max() characters or the characters until '\n' shall be ignored, whichever comes first
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nError : Invalid option, Try again!" << endl;
		return NULL;
	}
	//Updated topology matrix
	int** updatedTopologyMatrix = NULL;
	//Adds a router to topology matrix
	if (optionNum == 1)
	{
		//Updating global variable numRouters as one router is being added to topology
		numberOfRouters++;
		//Initializing updatedTopologyMatrix
		//Rows
		updatedTopologyMatrix = (int**)malloc(numberOfRouters * sizeof(int*));
		if (updatedTopologyMatrix == NULL)
		{
			cout << "Error : Memory allocation failed, Try again!" << endl;
			return NULL;
		}
		int indx = 0, rowIndx = 0, colIndx = 0;
		//Allotting memory for columns
		for (; indx < numberOfRouters; indx++)
		{
			updatedTopologyMatrix[indx] = (int*)malloc(numberOfRouters * sizeof(int));
			if (updatedTopologyMatrix[indx] == NULL)
			{
				cout << "Error : Memory allocation failed, Try again!" << endl;
				return NULL;
			}
		}
		//Copy over all old elements from previous topology matrix to new one
		for (rowIndx = 0; rowIndx < (numberOfRouters - 1); rowIndx++)
			for (colIndx = 0; colIndx < (numberOfRouters - 1); colIndx++)
				updatedTopologyMatrix[rowIndx][colIndx] = topologyMatrix[rowIndx][colIndx];
		cout << "\nCurrently there are " << (numberOfRouters - 1) << " routers, the new router will"
			" be added at index " << numberOfRouters << endl;
		cout << "\nPlease provide the costs of edges to and from the newly added router below,\nNote: If the there is no"
			" direct path/path itself, then provide a cost value of -1(equivalent to infinity)" << endl;
		//Cost from router to itself is zero
		updatedTopologyMatrix[numberOfRouters - 1][numberOfRouters - 1] = 0;
		indx = 0;
		for (; indx < (numberOfRouters - 1); indx++)
		{
			cout << "\nEnter cost from router " << (indx + 1) << " to new router " << numberOfRouters << ":" << endl;
			cin >> updatedTopologyMatrix[indx][numberOfRouters - 1];
			//Fails if input is not an integer and out of range
			if (cin.fail())
			{
				//Reverting value of numberOfRouters
				numberOfRouters--;
				//Freeing old topology matrix memory
				free(updatedTopologyMatrix);
				updatedTopologyMatrix = NULL;
				//Clears error flags
				cin.clear();
				//Either next numeric_limits<streamsize>::max() characters or the characters until '\n' shall be ignored, whichever comes first
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "\nError : Invalid option, Try again!" << endl;
				return NULL;
			}
			cout << "\nEnter cost from new router " << numberOfRouters << " to the router " << (indx + 1) << ":" << endl;
			cin >> updatedTopologyMatrix[numberOfRouters - 1][indx];
			//Fails if input is not an integer and out of range
			if (cin.fail())
			{
				//Reverting value of numberOfRouters
				numberOfRouters--;
				//Freeing old topology matrix memory
				free(updatedTopologyMatrix);
				updatedTopologyMatrix = NULL;
				//Clears error flags
				cin.clear();
				//Either next numeric_limits<streamsize>::max() characters or the characters until '\n' shall be ignored, whichever comes first
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "\nError : Invalid option, Try again!" << endl;
				return NULL;
			}
		}
		//Freeing old topology matrix memory
		free(topologyMatrix);
		//To avoid dangling pointer(access the same location again after freeing), topologyMatrix is set to NULL
		topologyMatrix = NULL;
		cout << "---------------------------------------------" << endl;
		cout << "\nReview topology matrix after adding router " << numberOfRouters << ":\n" << endl;
		//Prints updated topology matrix
		printTopologyMatrix(updatedTopologyMatrix);
		/*If there are saved locations of source and destination routers by option 1 and 2 then just
		output all paths and the shortest path among them*/
		getPathsFromSourceToDestination(updatedTopologyMatrix);
	}
	//Deletes a router from topology matrix
	else if (optionNum == 2)
	{
		if (numberOfRouters == 3)
		{
			cout << "\nAborting : Number of routers available in network is 3, further deletion of routers would make "
				"the number of routers less than 3, which is the minimum number of routers required to "
				"run Link state routing algorithm" << endl;
			return NULL;
		}
		else
		{
			int routerIndx = 0;
			cout << "\nEnter the index of the router which has to be deleted:" << endl;
			cin >> routerIndx;
			//Fails if input is not an integer and out of range
			if (cin.fail() || routerIndx < 1 || routerIndx > numberOfRouters)
			{
				//Clears error flags
				cin.clear();
				//Either next numeric_limits<streamsize>::max() characters or the characters until '\n' shall be ignored, whichever comes first
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "\nError : Invalid router index, Try again!" << endl;
				return NULL;
			}
			//Updating global variable numRouters as one router is being deleted from the topology
			numberOfRouters--;
			//Initializing updatedTopologyMatrix
			//Rows
			updatedTopologyMatrix = (int**)malloc(numberOfRouters * sizeof(int*));
			if (updatedTopologyMatrix == NULL)
			{
				//Reverting value of numberOfRouters
				numberOfRouters++;
				cout << "Error : Memory allocation failed, Try again!" << endl;
				return NULL;
			}
			int indx = 0, rowIndx = 0, colIndx = 0;
			//Allotting memory for columns
			for (; indx < numberOfRouters; indx++)
			{
				updatedTopologyMatrix[indx] = (int*)malloc(numberOfRouters * sizeof(int));
				if (updatedTopologyMatrix[indx] == NULL)
				{
					cout << "Error : Memory allocation failed, Try again!" << endl;
					return NULL;
				}
			}
			/*Copy over the elements from topologyMatrix to updatedTopologyMatrix is done by below logic,
			 Case 1. Never copies the matrix locations which has either row or col or both indices same as
			 (routerIndx -1),***this is the priority case and must run first
			 Case 2. For the indices which has either row or col or both index greater than (routerIndx -1)
			 ,after indices are passed through priority case 1: here every such greater index is reduced
			 by one which is the new location of updatedTopologyMatrix to which the value has to be copied.
			 Case 3. All the locations whose both indices less than (routerIndx -1) are copied to
			 updatedTopologyMatrix's same indices without any change*/
			int tempRowIndx = INVALID_NUMBER, tempColIndx = INVALID_NUMBER;
			for (rowIndx = 0; rowIndx < (numberOfRouters + 1); rowIndx++)
			{
				//Part of case - 1(priority case), test for rowIndx == (routerIndx - 1)
				if (rowIndx != (routerIndx - 1))
				{
					for (colIndx = 0; colIndx < (numberOfRouters + 1); colIndx++)
					{
						//Final part of case - 1(priority case), test for colIndx == (routerIndx - 1)
						if (colIndx != (routerIndx - 1))
						{
							/*Case 3: simply copies these locations*/
							if (rowIndx < (routerIndx - 1) && colIndx < (routerIndx - 1))
								updatedTopologyMatrix[rowIndx][colIndx] = topologyMatrix[rowIndx][colIndx];
							/*Case 2*/
							else if (rowIndx > (routerIndx - 1))
							{
								tempRowIndx = rowIndx - 1;
								if (colIndx > (routerIndx - 1))
								{
									tempColIndx = colIndx - 1;
									updatedTopologyMatrix[tempRowIndx][tempColIndx] = topologyMatrix[rowIndx][colIndx];
								}
								else
									updatedTopologyMatrix[tempRowIndx][colIndx] = topologyMatrix[rowIndx][colIndx];

							}
							else if (colIndx > (routerIndx - 1))
							{
								tempColIndx = colIndx - 1;
								if (rowIndx > (routerIndx - 1))
								{
									tempRowIndx = tempRowIndx - 1;
									updatedTopologyMatrix[tempRowIndx][tempColIndx] = topologyMatrix[rowIndx][colIndx];
								}
								else
									updatedTopologyMatrix[rowIndx][tempColIndx] = topologyMatrix[rowIndx][colIndx];

							}
						}
					}
				}
			}
			//Freeing old topology matrix memory
			free(topologyMatrix);
			//To avoid dangling pointer(access the same location again after freeing), topologyMatrix is set to NULL
			topologyMatrix = NULL;
			cout << "---------------------------------------------" << endl;
			cout << "\nReview topology matrix after adding router " << numberOfRouters << ":\n" << endl;
			//Prints updated topology matrix
			printTopologyMatrix(updatedTopologyMatrix);
			/*If there are saved locations of source and destination routers by option 1 and 2 then just
			output all paths and the shortest path among them, except if the deleted router index is same
			as selectedDestinationRouterIndex or selectedSourceRouterIndex. In this case user need to re-enter 
			the source and destination indices again*/
			if (selectedDestinationRouterIndex == routerIndx)
				selectedDestinationRouterIndex = 0;
			if (selectedSourceRouterIndex == routerIndx)
				selectedSourceRouterIndex = 0;
			getPathsFromSourceToDestination(updatedTopologyMatrix);
		}

	}
	return updatedTopologyMatrix;
}

/************************************************************************************************************
	Displays all paths from the source to the destination router, in addition to that it displays the
	shortest path from source to destination,among these listed paths
	Returns:
		Prints all paths from source to destination and the shortest path too
	Input:
		topologyMatrix - generated topology matrix
************************************************************************************************************/
void getPathsFromSourceToDestination(int** topologyMatrix)
{
	cout << "\nPaths from Source to Destination" << endl;
	cout << "--------------------------------" << endl;
	if (topologyMatrix == NULL)
	{
		cout << "\nError : Topology matrix is uninitialized, Run option 1" << endl;
		return;
	}
	//Destination router index
	int destnRouterIndx = INVALID_NUMBER;
	cout << "Select the destination router:" << endl;
	cout << "\nEnter 0 if you want to load from previously selected destination router index OR \nEnter non zero router index "
		"to clear previous selection and to select a new destionation router" << endl;
	cin >> destnRouterIndx;
	//CASE1 - valid input check
	//Fails if input is not an integer and out of range
	if (cin.fail() || destnRouterIndx < 0 || destnRouterIndx > numberOfRouters)
	{
		//Clears error flags
		cin.clear();
		//Either next numeric_limits<streamsize>::max() characters or the characters until '\n' shall be ignored, whichever comes first
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nError : Invalid destination router selection, Try again!" << endl;
		return;
	}
	//CASE1 - destnRouterIndx == 0, which has two sub cases, selectedDestinationRouterIndex is zero and greater than zero
	//No destination router selected before
	if (destnRouterIndx == 0 && selectedDestinationRouterIndex == 0)
	{
		cout << "Error: No destination router is selected previously OR previously selected router is deleted,"
			" so enter a new destination router index: " << endl;
		cin >> destnRouterIndx;
		//Fails if input is not an integer and out of range
		if (cin.fail() || destnRouterIndx < 1 || destnRouterIndx > numberOfRouters)
		{
			//Clears error flags
			cin.clear();
			//Either next numeric_limits<streamsize>::max() characters or the characters until '\n' shall be ignored, whichever comes first
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nError : Invalid destination router index, Try again!" << endl;
			return;
		}
		//Saving the selected destination router value
		selectedDestinationRouterIndex = destnRouterIndx;
	}
	//User wants destnRouterIndx to be same as previusly selected destination router index
	else if (destnRouterIndx == 0 && selectedDestinationRouterIndex > 0)
	{
		destnRouterIndx = selectedDestinationRouterIndex;
		cout << "Destination router is taken same as previously selected destination router, and it is: " << destnRouterIndx  << endl;
	}
	//CASE3 - entered destnRouterIndx value is greater than zero
	else if (destnRouterIndx > 0)
		//Saving the selected destination router value
		selectedDestinationRouterIndex = destnRouterIndx;		
	int srcRouterIndx = 0;
	if (selectedSourceRouterIndex == 0)
		/*If router index is not selected via option 2 then source router index is default value
		which is 1(DEFAULT_SOURCE_ROUTER_INDEX)*/
		srcRouterIndx = DEFAULT_SOURCE_ROUTER_INDEX;
	else
		//Source router index is selected via option 2
		srcRouterIndx = selectedSourceRouterIndex;
	cout << "\nSource router index is: " << srcRouterIndx << endl;
	cout << "Destination router index is: " << destnRouterIndx << endl;
	if (srcRouterIndx == destnRouterIndx)
	{
		cout << "\nSelected source and destination routers are same, Try again with "
			"different routers to find all paths and shortest paths among them" << endl;
		return;
	}
	//Displaying all paths between source and destination routers
	displayAllPaths(srcRouterIndx, destnRouterIndx, topologyMatrix);
	//Finds shortest path between source and destination routers
	getShortestPath(srcRouterIndx, destnRouterIndx, topologyMatrix, 1);
}

/************************************************************************************************************
	Creates forward table for a selected router/vertex in current network topology
	Returns:
		Prints forward table of selected router
	Input:
		topologyMatrix - generated topology matrix
************************************************************************************************************/
void createForwardTable(int** topologyMatrix)
{
	cout << "\nCreate a Forward Table" << endl;
	cout << "----------------------" << endl;
	if (topologyMatrix == NULL)
	{
		cout << "\nError : Topology matrix is uninitialized, Run option 1" << endl;
		return;
	}
	//routerIndex is the index of router, whose forward table is to be printed
	int routerIndex = 0;
	cout << "Select a source router: " << endl;
	cin >> routerIndex;
	//Fails if input is not an integer and out of range
	if (cin.fail() || routerIndex < 1 || routerIndex > numberOfRouters)
	{
		//Clears error flags
		cin.clear();
		//Either next numeric_limits<streamsize>::max() characters or the characters until '\n' shall be ignored, whichever comes first
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nError : Invalid source router index, Try again!" << endl;
		return;
	}
	//Saving selected router index to global variable
	selectedSourceRouterIndex = routerIndex;
	cout << "\nRouter " << routerIndex << " Connection Table\n" << endl;
	cout << endl;
	cout << "Destination\tInterface" << endl;
	cout << "==============================" << endl;
	int indx = 0;
	for (; indx < numberOfRouters; indx++)
	{
		/*If path cost is zero and row index and column index are same means it is the cost from a router to 
		itself, which is zero always so it has to print '-'*/
		if((topologyMatrix[routerIndex - 1][indx] == 0) && ((routerIndex - 1) == (indx)))
			cout << "    " << indx + 1 << "               -" << endl;
		//If path cost is -1 means, it has to print 'Infinity', no direct path exists
		else if (topologyMatrix[routerIndex - 1][indx] == -1)
			cout << "    " << indx + 1 << "            Infinity" << endl;
		//If path cost is neither 0 not -1 then print that same value
		else
			cout << "    " << indx + 1 << "               " << topologyMatrix[routerIndex - 1][indx] << endl;
	}
	cout << "==============================" << endl;
}

/************************************************************************************************************
	Initialization of network topology details from the input text file
	Returns:
		Network topology matrix
	Input:
		User need to input network topology data file name(text file) without extension, which must
		be present at the same location where this source file is present
************************************************************************************************************/
int** initNetworkTopology()
{
	cout << "\nInitializing Network Topology" << endl;
	cout << "-----------------------------" << endl;
	/*File name holder for topology matrix data file "+4" size is kept for ".txt"*/
	char fileName[FILE_NAME_LENGTH + 4];
	cout << "Input original network topology matrix data file:" << endl;
	cout << "[Note: Must be a square matrix with integer costs]" << endl;
	scanf("%s", fileName);
	int lengthFN = strlen(fileName);
	fileName[lengthFN] = '.';
	fileName[lengthFN + 1] = 't';
	fileName[lengthFN + 2] = 'x';
	fileName[lengthFN + 3] = 't';
	fileName[lengthFN + 4] = '\0';
	//Opening topology matrix data file
	FILE* file = fopen(fileName, "r");
	/*txt file is not present.So checks if rtf file with same name is present or not. Project accepts only
	.txt files*/
	if (file == NULL)
	{
		fileName[lengthFN + 1] = 'r';
		fileName[lengthFN + 2] = 't';
		fileName[lengthFN + 3] = 'f';
		fileName[lengthFN + 4] = '\0';
		file = fopen(fileName, "r");
		if (file != NULL)
		{
			cout << "\nNote : Project does not accept rtf files, only accepts '.txt' file, Try again!" << endl;
			return NULL;
		}
	}
	int numElements = 0;
	if (file != NULL)
	{
		int testDigit = INVALID_NUMBER;
		int retVal = INVALID_NUMBER;
		while (!feof(file))
		{
			/*fscanf here returns 1 upon successfully reading one integer(%d), if not zero*/
			retVal = fscanf(file, "%d", &testDigit);
			/*On successful reading above expression returns 1. If there is a space or
			newline after a character then fscan tends to read it and if it is a last character,
			and would return a value of -1 for the same, for other characters, above expression
			returns 0(checked in above traversal)*/
			if (retVal > 0 && retVal != EOF)
				numElements++;
			else if (retVal == 0)
			{
				/*Above fscanf expression returns zero if there is a character/special character in the file. This
				would not move file pointer further so have to manually exit from the loop*/
				cout << "\nError : Character or symbol is present in " << fileName << ", Try again!" << endl;
				return NULL;
			}
		}
	}
	else
	{
		//Only shows the file name entered if fails
		fileName[lengthFN] = '\0';
		cout << "\nError : Fails to read the file with name " << fileName << ", Try again!" << endl;
		return NULL;
	}
	/*Topology matrix has to be a square matrix.
	If square numRouters of the number of elements of matrix is not an integer then it is not a
	square matrix eg: 4 elements => 2x2 matrix,9 elements => 3x3 matrix*/
	//Here numRouters is an integer <= sqrt(numElements)
	int numRouters = sqrt(numElements);
	if ((numRouters * numRouters) != numElements)
	{
		cout << "\nError : Given topology matrix (from file " << fileName << ") is not a square matrix, Try again!" << endl;
		return NULL;
	}
	/*To find shortest path between some vertices the minimum number of vertices have to be three*/
	if (numRouters < MIN_NUMBER_ROUTERS)
	{
		cout << "\nError : Minimum number of routers to run Link State Routing algorithm is 3, given topology "
			"has only " << numRouters <<", Try again!" << endl;
		return NULL;
	}
	numberOfRouters = numRouters;
	//Saving numRouters to the global variable, numberOfRouters 
	numberOfRouters = numRouters;
	/*Feeding the topology matrix entries to Topology matrix*/
	int** topologyMatrix = NULL;
	//Rows
	topologyMatrix = (int**)malloc(numRouters * sizeof(int*));
	if (topologyMatrix == NULL)
	{
		cout << "Error : Memory allocation failed, Try again!" << endl;
		return NULL;
	}
	int indx = 0, rowIndx = 0, colIndx = 0;
	//Allotting memory for columns
	for (; indx < numRouters; indx++)
	{
		//Columns, topologyMatrix[indx]  == *(topologyMatrix + indx)
		topologyMatrix[indx] = (int*)malloc(numRouters * sizeof(int));
		if (topologyMatrix[indx] == NULL)
		{
			cout << "Error : Memory allocation failed, Try again!" << endl;
			return NULL;
		}
	}
	/*Reading the file again to feed numbers to matrix*/
	rewind(file);
	int digit = INVALID_NUMBER;
	int returnVal = INVALID_NUMBER;
	while (!feof(file))
	{
		/*fscanf here returns 1 upon successfully reading one integer(%d), if not zero*/
		returnVal = fscanf(file, "%d", &digit);
		/*On successful reading above expression returns 1. If there is a space or
		newline after a character then fscan tends to read it and if it is a last character,
		and would return a value of -1 for the same, for other characters, above expression
		returns 0(checked in above traversal)*/
		if (returnVal > 0 && returnVal != EOF && digit != INVALID_NUMBER)
		{
			/*Cost of path to the router itself must be zero always, eventhough txt file gives weight for the 
			cost to path from router to itself as it is meaningless*/
			if (rowIndx == colIndx)
				topologyMatrix[rowIndx][colIndx] = 0;
			else
				//topologyMatrix[rowIndx][colIndx] is Same as *(*(topologyMatrix + rowIndx) + colIndx)
				topologyMatrix[rowIndx][colIndx] = digit;
			if (colIndx == (numRouters - 1))
			{
				colIndx = 0;
				rowIndx++;
			}
			else if (colIndx < (numRouters - 1))
				colIndx++;
			digit = INVALID_NUMBER;
		}
	}
	fclose(file);
	cout << "\nReview original topology matrix:" << endl;
	//Function which prints topologyMatrix
	printTopologyMatrix(topologyMatrix);
	/*Once a new init network topology happens then selectedSourceRouterIndex and selectedDestinationRouterIndex
	must reset to zeros*/
	selectedSourceRouterIndex = 0;
	selectedDestinationRouterIndex = 0;
	return topologyMatrix;
}

/************************************************************************************************************
	This file prompts the menu options required and directs the call to necessary sub functions
	Returns:
		Nothing
	Input:
		Nothing
************************************************************************************************************/
void initMenu()
{
	int inputVal = 0;
	int** topologyMatrix = NULL;
	//Used for updation of existing topology matrix
	int** updatedTopologyMatrix = NULL;
	//Used in initNetworkTopology function
	int** testDoublePointer = NULL;
	//Menu options initialization
	while (1)
	{
		inputVal = 0;
		cout << "\n-----------------------------------" << endl;
		cout << "CS542 LINK STATE ROUTING SIMULATOR\n" << endl;
		cout << "(1) Input a Network Topology" << endl;
		cout << "(2) Create a Forward Table" << endl;
		cout << "(3) Paths from Source to Destination" << endl;
		cout << "(4) Update Network Topology" << endl;
		cout << "(5) Best Router for Broadcast" << endl;
		cout << "(6) Print network topology" << endl;
		cout << "(7) Exit" << endl;
		cout << "\n-----------------------------------" << endl;
		cout << "Command:" << endl;
		cin >> inputVal;
		//Fails if input is not an integer
		if (cin.fail())
		{
			//Clears error flags
			cin.clear();
			//Either next numeric_limits<streamsize>::max() characters or the characters until '\n' shall be ignored, whichever comes first
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nError : Non integer input: Select an integer option from given options." << endl;
			continue;
		}
		//Exit option when inputVal is 7
		if (inputVal == 7)
		{
			cout << "\nExiting CS542 2019 Spring project. Good Bye!" << endl;
			//Freeing memory
			if (topologyMatrix != NULL)
				free(topologyMatrix);
			if (updatedTopologyMatrix != NULL)
				free(updatedTopologyMatrix);
			topologyMatrix = NULL;
			updatedTopologyMatrix = NULL;
			break;
		}
		//All individual functions are explained at respective function headers
		switch (inputVal)
		{
		case 1:
			testDoublePointer = initNetworkTopology();
			/*If by mistake selected option 1 and came back by entering invalid file name then it does not 
			delete currently loaded topology matrix*/
			if (testDoublePointer != NULL)
			{
				topologyMatrix = testDoublePointer;
				testDoublePointer = NULL;
			}
			break;
		case 2:
			createForwardTable(topologyMatrix);
			break;
		case 3:
			getPathsFromSourceToDestination(topologyMatrix);
			break;
		case 4:
			updatedTopologyMatrix = updateNetworkTopology(topologyMatrix);
			/*Below statement means that updateNetworkTopology function is succeeded.topologyMatrix pointer
			is updated always with updatedTopologyMatrix and updatedTopologyMatrix
			is set to NULL to facilitate further updation by calling the same function updateNetworkTopology.
			Here the location is not freed, but just changed. If update topology function is failed then old 
			network topology matrix is still available.*/
			if (updatedTopologyMatrix != NULL)
			{
				//Must do this
				topologyMatrix = NULL;
				topologyMatrix = updatedTopologyMatrix;
				updatedTopologyMatrix = NULL;
			}
			break;
		case 5:
			getBestRouterForBroadcast(topologyMatrix);
			break;
		case 6:
			printTopologyMatrix(topologyMatrix);
			break;
			//Entry of an option value <1 or >7
		default:
			cout << "\nError : Invalid option, try again!\n" << endl;
			break;
		}
	}
}
/************************************************************************************************************/
