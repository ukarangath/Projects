''' ------------------------------------
This file contains all supporting functions of the main file Main.py.
This file is imported in above mentioned main file.
-------------------------------------'''
import os
import pandas as pd
import re
import matplotlib.pyplot as plt
from sklearn.metrics import confusion_matrix
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
Function which plots votes distribution across different samples in main dataframe, 
training dataframe and testing dataframe
Note: I took support from internet for plots 
-------------------------------------'''       
def plotLabelDistribution(df, trainingSet, testingSet):
    complete_label_distribution = df['vote_average'].value_counts(normalize=True) * 100
    training_label_distribution = trainingSet['vote_average'].value_counts(normalize=True) * 100
    test_label_distribution = testingSet['vote_average'].value_counts(normalize=True) * 100

    fig, axs = plt.subplots(1, 2, figsize=(15, 5))

    axs[0].bar(complete_label_distribution.index, complete_label_distribution.values, color='skyblue')
    axs[0].set_title('Complete Dataset Label Breakdown')
    axs[0].set_xlabel('Label')
    axs[0].set_ylabel('% of Samples')

    axs[1].bar(training_label_distribution.index - 0.2, training_label_distribution.values, width=0.4, label='Training Set', color='orange')
    axs[1].bar(test_label_distribution.index + 0.2, test_label_distribution.values, width=0.4, label='Test Set', color='green')
    axs[1].set_title('Training and Test Set Label Breakdown')
    axs[1].set_xlabel('Label')
    axs[1].set_ylabel('% of Samples')
    axs[1].legend()

    plt.tight_layout()
    # Saves the plot
    plt.savefig('PA02\LabelDistribution.png')    
    plt.show()    
''' ------------------------------------
Function which plots ROC curve and Confusion matrix of the model
Note: I took help from internet for making plots
-------------------------------------'''      
def plotROCandConfusionMatrix(truePositives, falsePositives, trueNegatives, falseNegatives):
    # Calculate true positive rate (TPR) and false positive rate (FPR)
    tpr = truePositives / (truePositives + falseNegatives)
    fpr = falsePositives / (falsePositives + trueNegatives)

    # Plot ROC curve
    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)
    plt.plot(fpr, tpr, color='blue', lw=2)
    plt.plot([0, 1], [0, 1], color='red', linestyle='--')
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title('ROC Curve')
    plt.grid(True)

    # Calculate confusion matrix
    cm = confusion_matrix([1, 0, 1, 0], [1, 1, 0, 0])  # Replace with your actual values
    plt.subplot(1, 2, 2)
    plt.imshow(cm, interpolation='nearest', cmap=plt.cm.Blues)
    plt.title('Confusion Matrix')
    plt.colorbar()
    plt.xticks([0, 1], ['Negative', 'Positive'])
    plt.yticks([0, 1], ['Negative', 'Positive'])
    plt.xlabel('Predicted Label')
    plt.ylabel('True Label')

    # Display values in each cell
    thresh = cm.max() / 2.
    for i in range(cm.shape[0]):
        for j in range(cm.shape[1]):
            plt.text(j, i, format(cm[i, j], 'd'), horizontalalignment="center", color="white" if cm[i, j] > thresh else "black")

    plt.tight_layout()
    # Saves the plot
    plt.savefig('PA02\ROCandConfusionMatrix.png')
    plt.show()
''' ------------------------------------
Function to count total words in a column
-------------------------------------'''
def countWords(column):
    return column.str.split().str.len().sum()  
''' ------------------------------------
Function which returns a python dictionary of total word counts for each vote in 
overview column in the given dataframe
-------------------------------------'''
def getTotalWordCountOfVotes(df): 
    # Group by 'vote_average' and apply the count_words function to the 'overview' 
    # column, then convert to dictionary
    voteAverageTotalWordsCountDict = (df.groupby('vote_average')['overview'].apply(countWords)).to_dict()
    print("Dictionary with total words count for all of the vote_average labels in overview:\n", voteAverageTotalWordsCountDict)
    return voteAverageTotalWordsCountDict
''' ------------------------------------
Function which returns a python dictionary of each vote probability among all votes in the given dataframe
-------------------------------------'''
def getVoteAverageProbabilities(df):
    # Get unique values of vote_average column and their occurrence counts
    uniqueValueCounts_trainingSet = df['vote_average'].value_counts()
    # Print unique values and their counts
    print(f"Training data frame: Unique values in the column '{'vote_average'}':")
    for value, count in uniqueValueCounts_trainingSet.items():
        print(f"{value}: {count}")
    # Calculate probabilities of vote_average labels and rounding to 3 digits
    labelProbabilities = uniqueValueCounts_trainingSet / len(df)
    labelProbabilities = round(labelProbabilities, 5)
    # Create a dictionary to store vote_average labels and their probabilities
    voteAverageProbabilitiesDict = {} 
    # Populate the dictionary with vote_average labels and corresponding probabilities
    for value, probability in labelProbabilities.items():
        voteAverageProbabilitiesDict[value] = probability
    print("Dictionary with 11 unique values of vote_average column and their probabilities:\n", voteAverageProbabilitiesDict)
    print("Sum of probabilities just for verification:\n", sum(voteAverageProbabilitiesDict.values()))
    return voteAverageProbabilitiesDict
''' ------------------------------------
Function which returns the dictionary which contains the number of 
occurrences of the input word in overview rows for each labels of 
vote_average column
-------------------------------------'''
def wordOccurrencesPerVote(df, word):
    wordOccurrencesPerVoteDict = {}
    for index, row in df.iterrows():
        label = row['vote_average']
        overview = row['overview']
        wordOccurrences = overview.lower().count(word.lower())
        if label not in wordOccurrencesPerVoteDict:
            wordOccurrencesPerVoteDict[label] = wordOccurrences
        else:
            wordOccurrencesPerVoteDict[label] += wordOccurrences
    return wordOccurrencesPerVoteDict     
''' ------------------------------------
Function to assign vote for a text 
-------------------------------------'''
def getVoteForText(trainingdf, text, voteAverageProbabilitiesDict, voteAverageTotalWordsCountDict):
    #  Getting bag of words representation of the given text wrt to all 11 labels in the training 
    # dataframe, this is without add-1 smoothing, which will be incorporated in later stage
    bagOfWordsForVotes = {}
    # Split the text using regular expressions to ignore certain characters
    words = re.split(r'\W+', text)
    for label in range(11):
        bagOfWordsForVotes[label] = {}
        for word in words:
            bagOfWordsForVotes[label][word] = sum(trainingdf.loc[trainingdf['vote_average'] == label, 'overview'].str.contains(word))

    # Finding the probabilities of the above text with respect to each label as explained in the lecture
    # but here there are 11 such labels, from 0 to 10, including both 0 and 10
    voteClassProbabilitiesForText = []
    for vote in range(11):
        textVocabularyDictOfVote = bagOfWordsForVotes[vote]
        try:
            voteProbability = voteAverageProbabilitiesDict[vote]
        except KeyError:
            # This is an important error of that vote is not present in the training dataframe,
            # so zeroing the results separately. It does not cause any issues as we are 
            # employing add-1 smoothing
            voteProbability = 0
        try:
            voteWordsCount = voteAverageTotalWordsCountDict[vote]
        except KeyError:
            # This is an important error of that vote is not present in the training dataframe,
            # so zeroing the results separately. It does not cause any issues as we are 
            # employing add-1 smoothing
            voteWordsCount = 0            
        yMAP = voteProbability
        # Traverse through the dictionary and multiply the values
        for value in textVocabularyDictOfVote.values():
            # add-1 smoothing done by default
            yMAP *= (value + 1)/ (voteWordsCount + len(textVocabularyDictOfVote))
        voteClassProbabilitiesForText.append(yMAP)

    # The position which has maximum probability is the predicted vote average
    vote = voteClassProbabilitiesForText.index(max(voteClassProbabilitiesForText))
    # Returning voteClassProbabilitiesForText for displaying probabilities for a 
    # user input sentence
    return vote, voteClassProbabilitiesForText
