''' -------------------------------------
The main file of execution
How to run,
Accepting one (1) command line argument, i.e. so code could be executed with
python Main.py EPSILON TRAIN% SUCCESS_THRESHOLD_WIFI
Default values as below,
TRAIN_PERCENTAGE ([10, 50]) = 50
In discussion forum Prof had mentioned that the minimum value of Train percentage can be 10%
EPSILON ([0; 1]) = 0.3
SUCCESS_THRESHOLD_WIFI (for Wi-Fi data) = -90 
-------------------------------------'''
import random
import numpy as np
import pandas as pd
import sys
import matplotlib.pyplot as plt
import re
# To save dictionary as file
import pickle
# This is a seperate file which contains all supporting functions of current file
from Functions import *
''' ------------------------------------
Starting point of execution
-------------------------------------'''
# Default values
TRAIN_PERCENTAGE = 50
EPSILON = 0.3
SUCCESS_THRESHOLD_WIFI = -90
# Including Python file name its 5 commandline arguments
if len(sys.argv) == 5:
    # Input csv file - Second command line argument
    inputCSV = sys.argv[1]
    try:
        # Consdering header present, first row is column labels
        df = pd.read_csv(inputCSV)
        print("CSV file successfully loaded.")
    except FileNotFoundError:
        print("CSV File not found:", inputCSV)
        sys.exit()
    except pd.errors.ParserError:
        print("Error parsing input CSV file:", inputCSV)
        sys.exit()
    except Exception as e:
        print("Input CSV File: An error occurred:", e)  
        sys.exit() 
    # EPSILON - Third command line argument 
    try:
        eps = float(sys.argv[2])
        if eps < 0 or eps > 1:
            # Out of range EPSILON value, taking default value here
            print("Taking default EPSILON.")
        else:
            EPSILON = eps
    except ValueError:
        # Invalid EPSILON value, taking default value here
        pass  
    # Train percentage value - Fourth command line argument 
    try:
        trainPercent = int(sys.argv[3])
        # If train percentage is zero or above 50 then the default value of 
        # train percentage is taken
        if (trainPercent >= 10) and (trainPercent <= 50):
            TRAIN_PERCENTAGE = trainPercent    
        else: 
            # Out of range Train percentage value, taking default TRAIN_SIZE here
            # As per Professors recommendation in discussion forum, the safe 
            # train percentage range is [10, 50]
            pass    
    except ValueError:
        # Invalid Train percentage value, taking default value here
        pass
    # Success threshold value - Fifth and final command line argument 
    try:
        SUCCESS_THRESHOLD_WIFI = float(sys.argv[4])
    except ValueError:
        # Invalid successThreshold value, taking default value here
        # so no change
        pass
else:
    print("ERROR: Not enough or too many input arguments. Exiting.")
    sys.exit()

'''----------------------------------'''    
# Clears off screen for clean display of results alone
clearScreen()
print("----------------------------------------------------------------------------------")
print("Arangath, Unnikrishnan, A20348848 CS585 PA02 Solution:")
print("----------------------------------------------------------------------------------")
# For debugging purposes
np.random.seed(0) 
print("Epsilon:   ", EPSILON)  
print("Training data percentage:   ", TRAIN_PERCENTAGE)  
print("Success threshold:   ", SUCCESS_THRESHOLD_WIFI)  
print("----------------------------------------------------------------------------------")
print("First three rows of DataFrame:")
print(df.head(3))
print("----------------------------------------------------------------------------------")
# Below is the python dictionary whose key is the column name of the input csv, which is first row
# and value is a python list of three members, first one if the success visits of current 
# arm/ column/ channel here, second is total number of visits to current arm and third is the 
# success probability of the current arm
armStatDict = {}
# Taking initial best arm as first one/ first column/ first channel
currentBestArm = 1 
for column in df.columns:
    # We don't know anything about any of these channels, so initially all zero
    armStatDict[column] = [0, 0, 0.0]  

# Number of rows to select for training
trainSizeIteration = round((TRAIN_PERCENTAGE/ 100) * len(df))

# TRAINING
print("TRAINING Start ... ")
print("Note: Debug prints during training are saved in trainingOutput.txt file at same location due to the size of the same.")
# File name to save the prints during training is below
redirectPrintsToFile("trainingOutput.txt")
print("EPSILON: ", EPSILON)
print("Success Threshold: ", SUCCESS_THRESHOLD_WIFI)
for index in range(trainSizeIteration):
    print("----------------------------------------------------------------------------------")
    # Generating random number pValue in [0, 1], 0 and 1 inclusive
    pValue = random.uniform(0, 1)
    print("Timestamp/ Row Index: ", index)
    print("Generated random number from [0, 1]: ", pValue)    
    if pValue < EPSILON: # EXPLORE
        print("EXPLORING ...")
        # Select one random column
        randomArmOrChannel = random.choice(df.columns)
        print("Random Arm/ Channel: ",randomArmOrChannel)
        # Get the arm value for current time stamp, index
        # means the value at [index, randomArmOrChannel]
        armValue = df[randomArmOrChannel].iloc[index]
        print("Rnadom Arm/ Channel Value at current timestamp/ row: ",armValue)
        # Success
        if armValue <= SUCCESS_THRESHOLD_WIFI:
            print("Success")
            # Updating success counts
            armStatDict[randomArmOrChannel][0] += 1
            # Updating total visit counts
            armStatDict[randomArmOrChannel][1] += 1
            # Updating success probability associated with selected arm
            armStatDict[randomArmOrChannel][2] = armStatDict[randomArmOrChannel][0]/ armStatDict[randomArmOrChannel][1]
        # Failure
        else: 
            print("Failure")
            # Updating total visit counts
            armStatDict[randomArmOrChannel][1] += 1
            # Updating success probability associated with selected arm
            armStatDict[randomArmOrChannel][2] = armStatDict[randomArmOrChannel][0]/ armStatDict[randomArmOrChannel][1]            
    else: # EXPLOITING
        print("EXPLOITING Current Best Arm ...")        
        # Getting current best arm to exploit
        currentBestArm = getCurrentBestArm(armStatDict)
        print("current Best Arm: ", currentBestArm)
        # Getting the timeseries value associated with current best arm 
        # at current time stamp, which is the value at [index, currentBestArm],
        # which decides success or failure now
        armValue = df[currentBestArm].iloc[index]
        print("Current Best Arm value at current time stamp: ", armValue)
        # Success
        if armValue <= SUCCESS_THRESHOLD_WIFI:
            print("Success")
            # Updating success counts
            armStatDict[currentBestArm][0] += 1
            # Updating total visit counts
            armStatDict[currentBestArm][1] += 1
            # Updating success probability associated with current best arm
            armStatDict[currentBestArm][2] = armStatDict[currentBestArm][0]/ armStatDict[currentBestArm][1]
        # Failure
        else: 
            print("Failure")
            # Updating total visit counts
            armStatDict[currentBestArm][1] += 1
            # Updating success probability associated with current best arm
            armStatDict[currentBestArm][2] = armStatDict[currentBestArm][0]/ armStatDict[currentBestArm][1] 
    # To verify updations
    printCurrentArmStatistics(armStatDict)
# Restore prints back to console
restoreStdout()
print("TRAINING Ends.")           
printCurrentArmStatistics(armStatDict)
currentBestArm = getCurrentBestArm(armStatDict)
print("Success probability of Best Arm", currentBestArm, "post TRAINING is", armStatDict[currentBestArm][2])

# TESTING ON CURRRENT BEST ARM RECEIVED FROM TRAINING 
# Boundary check if testing index is iterable to length of df
if trainSizeIteration >= len(df):
    print("No testing data, used full data for training alone. Try with a different training percentage. Exits ...")
    sys.exit()

print("\nTESTING Start ... ")
print("Testing starts from the next index of training until the end of the dataframe, on the Current Best Arm. \nEXPLOITING Curent Best Arm for the rest of the dataframe.")
print("Note: Debug prints during testing are saved in testingOutput.txt file at same location due to the size of the same.")
# File name to save the prints during training is below
redirectPrintsToFile("testingOutput.txt")
print("EPSILON: ", EPSILON)
print("Success Threshold: ", SUCCESS_THRESHOLD_WIFI)
# Getting Best arm from training results
bestArm = getCurrentBestArm(armStatDict)
print("Post training, Best Arm is", bestArm, "on which testing will be performed.")
# successVisitsOfBestArm is the number of successful visits of best arm during testing phase
successVisitsOfBestArm = 0
# For plots, saving the Cumulative Success Probability, I use a dictionary
cumulativeSuccessProbabilityAndTimeStampStat = {'TimeStamp': [], 'Cumulative Success Probability': []}
cumulativeSuccessProbability = 0
# Testing starts from the next index of training until the end of the dataframe
for index in range(trainSizeIteration, len(df)):
    print("----------------------------------------------------------------------------------")
    print("Timestamp/ Row Index: ", index)
    # For plots
    cumulativeSuccessProbabilityAndTimeStampStat['TimeStamp'].append(index)
    # Getting the timeseries value associated with best arm 
    # at current time stamp, which is the value at [index, bestArm],
    # which decides success or failure now
    armValue = df[bestArm].iloc[index]
    print("Value of Best Arm at current time stamp: ", armValue)
    # Success
    if armValue <= SUCCESS_THRESHOLD_WIFI:
        print("Success")
        successVisitsOfBestArm += 1
    # Failure
    else: 
        print("Failure")
    # For plots
    print("Success Probability at current time stamp:", (successVisitsOfBestArm/ (index - trainSizeIteration + 1)))
    cumulativeSuccessProbability += (successVisitsOfBestArm/ (index - trainSizeIteration + 1))
    print("Cumulative Success Probability at current time stamp:", cumulativeSuccessProbability)
    cumulativeSuccessProbabilityAndTimeStampStat['Cumulative Success Probability'].append(cumulativeSuccessProbability)

# Save cumulativeSuccessProbabilityAndTimeStampStat dictionary to a file, for merging the plots
# Rename csp_rename_this_file_manually.pkl files as csp350.pkl, csp550.pkl and csp850.pkl for
# for Epsilon and TRAIN% parameter pairings: 0.3/50, 0.5/50, 0.8/50 with same Threshold of -90
with open('csp_rename_this_file_manually.pkl', 'wb') as file:
    pickle.dump(cumulativeSuccessProbabilityAndTimeStampStat, file)

# Restore prints back to console
restoreStdout()        
print("TESTING Ends.")
print("Success probability of Best Arm", bestArm, "post TESTING is", (successVisitsOfBestArm/ (len(df) - trainSizeIteration)))