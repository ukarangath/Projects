//Ensures current file included only once
#pragma once
/*
	Link State Routing Simulator
	C++ header
	Author : Unnikrishnan
*/

//MACROS
/*To find shortest path between some vertices, there has to be minimum of three vertices. If only one vertex is present then
no shortest path exists, if two vertices are there then the only connection between them is the shortest path.*/
#define MIN_NUMBER_ROUTERS 3
//Maximum number of letters allowed in topology matrix text data file name except ".txt"
#define FILE_NAME_LENGTH 30
/*Just a invalid number to be used as an initialization value in some cases eg: predessor node's index of a node which 
does not have a predessor node, here large negative number is very much invalid to be an index. Which means there is no
predessor node for the node under consideration.*/
#define INVALID_NUMBER -100000
/*A very large positive number is used, this is used in Dijkstra's algorithm to find best router for broadcast. This can not be 
INT_MAX, because if INT_MAX + 1 happens then it flips sign to become -INT_MAX. This can be treated as maximum positive weight or cost
allowed between any two nodes in any network topology considered in this project.*/
#define LARGEST_POSITIVE_WEIGHT 100000
/*If no router is selected as source router and user selects destination router then 1 is used as source router index*/
#define DEFAULT_SOURCE_ROUTER_INDEX 1

/*FUNCTION DECLARATIONS RELATED TO LINK STATE ROUTING ALGORITHM*/
void displayAllPaths(int sourceRouterIndex, int destinationRouterIndex, int** topologyMatrix);
void displayAllPathsHelper(int sRI, int dRI, int* visited, int* path, int** tM, int& pathIndx);
void getShortestPath(int sourceRouterIndex, int destinationRouterIndex, int** topologyMatrix, int mode);
int getShortestPathHelper(int countOfVisitedNodes, int* nodeVisitedInfo, int* cumDist);

/*FUNCTION DECLARATIONS FOR PROJECT FRAMEWORK*/
void printTopologyMatrix(int** topologyMatrix);
void getBestRouterForBroadcast(int** topologyMatrix);
int** updateNetworkTopology(int** topologyMatrix);
void getPathsFromSourceToDestination(int** topologyMatrix);
void createForwardTable(int** topologyMatrix);
int** initNetworkTopology();
void initMenu();

