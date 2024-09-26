''' ------------------------------------
This file is a file which generates Single merged plot of Cumulative Success Probability. 
There must be input pk files/ saved dictionary files for generation of plots, three files named as
csp350.pk, csp550.pk and csp850.pk, for Epsilon and TRAIN% parameter pairings: 0.3/50, 0.5/50, 0.8/50 
respectively with same Threshold of -90. File must be generated separately running main file like, 
1. python Main.py input.csv 0.3 50 -90 for csp350.pkl
2. python Main.py input.csv 0.5 50 -90 for csp550.pkl
3. python Main.py input.csv 0.8 50 -90 for csp850.pkl
pkl files must be manually renamed
-------------------------------------'''
import matplotlib.pyplot as plt
# Reading saved dictionary file
import pickle
''' ------------------------------------
Generating merged plot
-------------------------------------'''
# Reading three saved dictionary files of Cumulative Success Probabilities 
# for Epsilon and TRAIN% parameter pairings: 0.3/50, 0.5/50, 0.8/50
with open('csp350.pkl', 'rb') as file:
    csp350 = pickle.load(file)

with open('csp550.pkl', 'rb') as file:
    csp550 = pickle.load(file)

with open('csp850.pkl', 'rb') as file:
    csp850 = pickle.load(file)    

# Three plots in single figure
plt.figure(figsize=(10, 6))  # Adjust the figure size as needed

plt.plot(csp350['TimeStamp'], csp350['Cumulative Success Probability'], marker='o', markersize=10, linestyle='-', label='Epsilon/ TRAIN%: 0.3/ 50')
plt.plot(csp550['TimeStamp'], csp550['Cumulative Success Probability'], marker='s', markersize=5, linestyle='-', label='Epsilon/ TRAIN%: 0.5/ 50')
plt.plot(csp850['TimeStamp'], csp850['Cumulative Success Probability'], marker='^', markersize=1, linestyle='-', label='Epsilon/ TRAIN%: 0.8/ 50')

plt.xlabel('TimeStamp')
plt.ylabel('Cumulative Success Probability')
plt.title('Comparison of Cumulative Success Probability \nfor Epsilon and TRAIN% parameter pairings: 0.3/50, 0.5/50, 0.8/50')
plt.legend()
plt.grid(True)
plt.savefig('SuccessProbabilityComparisonPlot.png')
plt.show()