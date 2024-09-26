''' -------------------------------------
The main file of execution
How to run,
Accepting one (1) command line argument, i.e. so code could be executed with
python PA02\Main.py TRAIN_SIZE
where:
TRAIN_SIZE is a number between 20 and 80 defining the size (in percentages) of 
the training set. For example: 60 would mean FIRST (as ordered in the dataset file) 
60% of samples. Note that your test set is always going to be the LAST (as ordered 
in the dataset file) 20% of samples.
Example:
python Main.py.py YES
If the number of arguments provided is NOT one (none, two or more) or the TRAIN_SIZE 
argument is out of the specified range, assume that the value for TRAIN_SIZE is 80.
-------------------------------------'''
import pandas as pd
import numpy as np
import sys
import os
from collections import defaultdict
# This is a seperate file which contains all supporting functions of current file
from Functions import *
''' ------------------------------------
Starting point of execution
-------------------------------------'''
# Default training and testing percentage of given data
TRAIN_SIZE = 80
TEST_SIZE = 20
# Check if the correct number of command-line arguments is provided
if len(sys.argv) == 2:
    try:
        # Attempt to convert the argument to an integer
        int_arg = int(sys.argv[1])
        # Next process if the given second argument is an integer
        if (int_arg >= 20) and (int_arg <= 80):
            TRAIN_SIZE = (int)(sys.argv[1])        
    except ValueError:
        # Take default TRAIN_SIZE here as given second argument is not an integer
        pass # Do nothing
'''----------------------------------'''    
# Clears off screen for clean display of results alone
clearScreen()
print("----------------------------------------------------------------------------------")
print("Arangath, Unnikrishnan, A20348848 CS585 PA02 Solution:")
print("----------------------------------------------------------------------------------")
print("Train size is first ",TRAIN_SIZE, "% of the data and TEST_SIZE is last 20% of the data", sep='')
''' ------------------------------------
Reading downloaded "Popular Movies of IMDb" of Kaggle
-------------------------------------'''  
dforiginal = pd.read_csv('PA02\TMDb_updated.csv')
# By setting the random seed to a fixed value, you ensure that every time you run your code, 
# you get the same sequence of random numbers. This is useful when you want to debug your code 
# or share your results with others
np.random.seed(0) 
print("First three rows of DataFrame:")
print(dforiginal.head(3))
''' ------------------------------------
Handling missing locations and fine tuning of data frame
-------------------------------------'''     
# Get total missing locations
totalMissingLocations = np.count_nonzero(dforiginal.isnull().values)

# Print total missing locations
print("Number of total missing locations:\n", totalMissingLocations)

# Check if any row is entirely missing
emptyRows = dforiginal.isnull().all(axis=1)
anyEmptyRows = any(emptyRows)

# Print if any row is entirely missing
if anyEmptyRows:
    print("At least one row is entirely missing.")

# Get exact missing cell locations
missingLocations = list(zip(*np.where(pd.isnull(dforiginal))))

# Print exact missing cell locations
print("Missing cell locations:")
locations = []
for row, col in missingLocations:
    locations.append((row, dforiginal.columns[col])) 
print(locations)

# As all of the 30 empty cells are in overview column and which is important for analysis, 
# so dropping those all rows which can not be used for either training or testing
dfProcessed = dforiginal.dropna()

# Dropping original_language and vote_count columns as they do not hold much signifcance for 
# analysis, Print the head of the DataFrame before dropping the columns
# Decided to drop 'title' column as it does not have significance if its not associated with 
# overview column
columns_to_drop = [dfProcessed.columns[0], 'original_language', 'vote_count', 'title']
dfProcessed = dfProcessed.drop(columns=columns_to_drop)

# Save the cleaned DataFrame back to a CSV file
dfProcessed.to_csv('PA02\TMDb_updated_1.csv', index=False)

print("Head of DataFrame before dropping the columns:")
print(dfProcessed.head(3))

# Print the head of the DataFrame after dropping the columns
print("\nHead of DataFrame after dropping the columns:")
print(dfProcessed.head(3))

# Prints size of the data file before and after cleaning
print(f"Size before cleaning:\n {dforiginal.shape}")
print(f"Size after cleaning:\n {dfProcessed.shape}")

# Round values of a specific column to the nearest integer
column_to_round = 'vote_average'
dfProcessed[column_to_round] = dfProcessed[column_to_round].round()

# Change data type of the column to integer
dfProcessed[column_to_round] = dfProcessed[column_to_round].astype(int)

print("First 3 entries of the DataFrame post rounding off vote_average column:")
print(dfProcessed.head(3))

# Save the DataFrame back to a CSV file
dfProcessed.to_csv('PA02\TMDb_updated_2.csv', index=False)
''' ------------------------------------
Exploration of vote_average/ result column
-------------------------------------'''     
# Find the maximum and minimum values of vote_average column to form labels
columnName = 'vote_average'
maxVoteAverage = dfProcessed[columnName].max()
minVoteAverage = dfProcessed[columnName].min()

# Print the maximum and minimum values
print(f"Maximum value in column '{columnName}': {maxVoteAverage}")
print(f"Minimum value in column '{columnName}': {minVoteAverage}")

# Find the number of unique values in the vote_average column
numOfUniqueValues = dfProcessed[columnName].nunique()

# Print the number of unique values
print(f"Number of unique values in column '{columnName}': {numOfUniqueValues}")

# Get the unique values and their counts
uniqueValueCounts_dfProcessed = dfProcessed[columnName].value_counts()

# Print unique values and their counts
print(f"Main dataframe: Unique values in the column '{columnName}':")
for value, count in uniqueValueCounts_dfProcessed.items():
    print(f"{value}: {count}")
''' ------------------------------------
Seperating training and testing data frame
-------------------------------------''' 
# Calculate the number of rows for the training set
trainRows = int((len(dfProcessed) * TRAIN_SIZE)/100)

# Split the dataset into training set and remaining data
trainingSet = dfProcessed.iloc[:trainRows]

# Calculate the number of rows for the test set (always last 20% of the dataset)
testRows = int((len(dfProcessed) * TEST_SIZE)/100)

# Take the LAST 20% of the dataframe as the test set
testingSet = dfProcessed.iloc[-testRows:]

# Resetting the indices of the resulting DataFrames
trainingSet.reset_index(drop=True, inplace=True)
testingSet.reset_index(drop=True, inplace=True)

# Saving above training and testing data frames as csv files
# index=False is kept not to retain any old row numbers
trainingSet.to_csv('PA02\TMDb_updated_2_trainingSet.csv', index=False) 
testingSet.to_csv('PA02\TMDb_updated_2_testingSet.csv', index=False)  


# Print the shapes of the training and test sets
print("Shape of data frame:\n", dfProcessed.shape)
print("Shape of training set:\n", trainingSet.shape)
print("Shape of testing set:\n", testingSet.shape)

# Plots votes distribution across different samples in main dataframe, training dataframe
# and testing dataframe
plotLabelDistribution(dfProcessed, trainingSet, testingSet)
''' ------------------------------------
Stage: Training classifier … 
Processing trainingSet dataframe 
Gets the probabilities of 11 labels of vote_average column (voteAverageProbabilitiesDict) 
Gets the total word counts of overview columns correspondings to these 11 labels of vote_average column (voteAverageTotalWordsCountDict)
-------------------------------------''' 
print("----------------------------------------------------------------------------------")
print("Training classifier …")
print("----------------------------------------------------------------------------------")

# Getting the python dictionary which contains the probability of each vote among all votes
voteAverageProbabilitiesDict = getVoteAverageProbabilities(trainingSet)
# Getting the python dictionary which has the total word count per each vote in 
# overview column
voteAverageTotalWordsCountDict = getTotalWordCountOfVotes(trainingSet)
''' ------------------------------------
Stage: Testing classifier …
-------------------------------------''' 
''' ------------------------------------
Testing user input sentence
-------------------------------------''' 
print("----------------------------------------------------------------------------------")
print("Testing user input sentence …")
print("----------------------------------------------------------------------------------")
while True:
    # Prompt the user to enter a sentence
    S = input("Enter your sentence (enter 'N' or 'n' or 'press Enter' to exit): ")

    # Check if the user entered 'N' or 'n' to exit
    if S.lower() == 'n' or not S.strip():
        print("Exiting the program...")
        break

    # Check if the user pressed Enter to exit
    if not S.strip():
        print("Exiting the program...")
        break    

    # Print the entered sentence
    print("You entered S:", S)

    # Getting the vote for input sentence S
    voteCalculated, voteClassProbabilitiesForText = getVoteForText(trainingSet, S, voteAverageProbabilitiesDict, voteAverageTotalWordsCountDict)

    print("was classified as ", voteCalculated)
    # Traverse through voteClassProbabilitiesForText and prints the probabilities of other votes
    for vote, yMAP in enumerate(voteClassProbabilitiesForText):
        print("P({} | S) = {}".format(vote, round(yMAP, 10)))   
print("----------------------------------------------------------------------------------")
print("Testing classifier …")
print("----------------------------------------------------------------------------------")
# Variables for testing classifier
truePositives = 0
trueNegatives = 0
falsePositives = 0
falseNegatives = 0

# Positive class - 6 or 7, Negative class - others all
positive = {6, 7}
negative = {0, 1, 2, 3, 4, 5, 8, 9, 10}

# Traversing through each rows of testing dataframe
for index, row in testingSet.iterrows():
    # Get the 'overview' and 'vote_average' values for the current row
    overviewText = row['overview']
    voteActual = row['vote_average']
    # Getting the vote for input overviewText
    voteCalculated, voteClassProbabilitiesForText = getVoteForText(trainingSet, overviewText, voteAverageProbabilitiesDict, voteAverageTotalWordsCountDict)
    # Calculating TP, TN. FP, FN accordingly
    # True positive: voteActual is in positive class and voteCalculated is in positive class
    if voteActual in positive and voteCalculated in positive:
        truePositives += 1
    # True negative: voteActual is in negative class and voteCalculated is in negative class
    elif voteActual in negative and voteCalculated in negative:
        trueNegatives += 1
    # False positive: voteActual is in negative class but voteCalculated is in positive class
    elif voteActual in negative and voteCalculated in positive:
        falsePositives += 1
    # False negative: voteActual is in positive class but voteCalculated is in negative class
    elif voteActual in positive and voteCalculated in negative:
        falseNegatives += 1

# Calculating the metrics
if truePositives + falseNegatives == 0:
    sensitivity = 0
else:
    sensitivity = truePositives / (truePositives + falseNegatives)

if trueNegatives + falsePositives == 0:
    specificity = 0
else:
    specificity = trueNegatives / (trueNegatives + falsePositives)

if truePositives + falsePositives == 0:
    precision = 0
else:
    precision = truePositives / (truePositives + falsePositives)

if trueNegatives + falseNegatives == 0:
    negativePredictiveValue = 0
else:
    negativePredictiveValue = trueNegatives / (trueNegatives + falseNegatives)

total_samples = truePositives + trueNegatives + falsePositives + falseNegatives
if total_samples == 0:
    accuracy = 0
else:
    accuracy = (truePositives + trueNegatives) / total_samples

if precision + sensitivity == 0:
    fscore = 0
else:
    fscore = 2 * (precision * sensitivity) / (precision + sensitivity)

print("True Positives:", truePositives)
print("True Negatives:", trueNegatives)
print("False Positives:", falsePositives)
print("False Negatives:", falseNegatives)
print("Sensitivity (recall):", sensitivity)
print("Specificity:", specificity)
print("Precision:", precision)
print("Negative Predictive Value:", negativePredictiveValue)
print("Accuracy:", accuracy)
print("F-Score:", fscore) 

# Plots ROC and Confusion Matrix for above model
plotROCandConfusionMatrix(truePositives, falsePositives, trueNegatives, falseNegatives)



