/*
	Link State Routing Simulator
 	Main file
	Author : Unnikrishnan
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

/************************************************************************************************************
	Main function
	Returns:
		
	Input:
		
************************************************************************************************************/
int main()
{
	//FOR TESTING - Generate a topology matrix of higher size
	// Select either nt1.txt or nt2.txt from the main folder as input network topology files
	FILE* file = fopen("nt1.txt", "w+");
	
	if (file != NULL)
	{
		for (int i = 0; i < 150; i++)
		{
			for (int j = 0; j < 150; j++)
			{
				if (i == j)
				{
					fprintf(file, "%d", 0);
					fprintf(file, " ");
				}
				else
				{
					//rand()/100 is to make it a small number, not to cross INT_MAX
					fprintf(file, "%d", rand()/100);
					fprintf(file, " ");
				}
			}
			fprintf(file, "\n");
		}
	}
	fclose(file);*/
   /*Menu options are inilialized here, further function calls happen through this function*/
   initMenu();
}
