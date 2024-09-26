''' ------------------------------------
This file contains all supporting functions of the main file Main.py.
This file is imported in above mentioned main file.
-------------------------------------'''
import os
import sys
''' ------------------------------------
Function which cleares screen for ease 
of displaying output
-------------------------------------'''
def clearScreen():
    if os.name == 'nt':
        os.system('cls')
    else:
        os.system('clear')
''' ------------------------------------
Function which prints arm statistics dictionary
-------------------------------------'''
def printCurrentArmStatistics(armStatDict): 
    print("----------------------------------------------------------------------------------")
    print("Current Statistics:")
    for key, value in armStatDict.items():
        print(f"Arm: {key} =>  [Success: {value[0]}, Total Visits: {value[1]}, Success Probability: {value[2]}]")  
    print("----------------------------------------------------------------------------------")   
''' ------------------------------------
Function which returns the Arm which is current best,
or having the highest success porobability so far
-------------------------------------'''
def getCurrentBestArm(armStatDict): 
    currentBestArm = None
    maxSuccessProbability = 0.0 
    # If some keys have same max success probability then it selects the first 
    # key with this specific maximum probability value
    for key, value in armStatDict.items():
        if value[2] > maxSuccessProbability:
            maxSuccessProbability = value[2]
            currentBestArm = key
    # If there are no Arms with a non zero success probability, occurs during initial case.
    # Returns the first key in such case
    if currentBestArm is None:
        return next(iter(armStatDict))
    return currentBestArm
''' ------------------------------------
Function to redirect prints to a file
-------------------------------------'''
def redirectPrintsToFile(filename):
    sys.stdout = open(filename, 'w')
''' ------------------------------------
Restore stdout to default/ console
-------------------------------------'''
def restoreStdout():
    sys.stdout.close()
    sys.stdout = sys.__stdout__