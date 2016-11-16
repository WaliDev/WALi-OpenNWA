#!/usr/bin/python
#
# toPlotsAndSummaryText.py
#   Produce scatter plots comparing performance of ICRA to other tools, and
#   also a little bit of summary text: geometric means of ratios of other
#   tools' running times divided by ICRA's running times.
#
#   Inputs is read from outputs/result.csv
#   (So, please run toCSV.py first)
#
#   Output is written to outputs/plot-ICRA-vs-*.png and tool-comparison-plots.html and outputs/automatic-summary.txt
#
#   This script uses matplotlib to make its plots and scipy to compute geometric means, so you may need to install those packages.

import os.path, csv

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import scipy.stats

outputDir = "outputs"
plotFilenameTemplate = "plot-ICRA-vs-%s.%s"
inputFilename = "result.csv"
htmlFilename = "tool-comparison-plots.html"
summaryFilename = "automatic-summary.txt"

def nicePrint(aList) :
    n = 5
    for i,v in enumerate(aList) :
        print v,
        if i % n == (n - 1) : print ""
    print ""

class GeometricMean :
    def __init__(self, tag=None) :
        self.tag = tag
        self.values = list()
    def register(self, value) :
        self.values.append(value)
    def read(self) :
        resultSP = scipy.stats.mstats.gmean(self.values)
        #if "SEA" in self.tag and "all" in self.tag :
        #    print "Here are the values for: " + self.tag
        #    nicePrint(sorted(self.values))
        #    #print sorted(self.values)
        #    print "Their arithmetic mean is: " + str(sum(self.values) / float(len(self.values)))
        #    print "Their geometric mean is: " + str(resultSP) 
        return resultSP

def numFormat(v) :  return "%.3f" % v

plotPathTemplate     = os.path.join(outputDir, plotFilenameTemplate)
inputPath            = os.path.join(outputDir, inputFilename)
summaryPath          = os.path.join(outputDir, summaryFilename)

#minValue = 0.04; maxValue = 300
minValue = 0.01; maxValue = 900

summaryFile = open(summaryPath, 'w')

otherTools = ["UA","CPA","LPI","SEA"]
fullNames = {"UA":"Ultimate Automizer", "CPA":"CPAchecker", "LPI":"LPI", "SEA":"SeaHorn"}
for tool in otherTools :
    inputFile = open(inputPath, 'r')
    inputCSV = csv.reader(inputFile)
    header = inputCSV.next()
    #Expect exactly: "dir","filename","tool","desiredOutput","rawOutput","conclusion","time"
    icraValues = dict()
    otherToolValues = dict()
    keys = list()
    selectedKeysICRA = set()
    selectedKeysOther = set()
    for directory, filename, toolOnThisRow, desiredOutput, rawOutput, conclusion, time in inputCSV :
        key = directory+"@@@@"+filename
        if True : # could put other criteria here...
            if key not in keys : keys.append(key)
            if toolOnThisRow == tool :   otherToolValues[key] = time
            if toolOnThisRow == "ICRA" : icraValues[key] = time
        if conclusion in ["PASS", "OKAY", "FAIL", "UNSOUND"] :
            if toolOnThisRow == tool : selectedKeysOther.add(key)
            if toolOnThisRow == "ICRA" : selectedKeysICRA.add(key)
    xValues = list()
    yValues = list()
    allKeysGeometricMean = GeometricMean("allKeys"+tool)
    for key in keys :
        if key in otherToolValues and key in icraValues :
            xValues.append( otherToolValues[key] )
            yValues.append( icraValues[key] )
            allKeysGeometricMean.register(float(otherToolValues[key])/float(icraValues[key]))
    fig, ax = plt.subplots()
    ax.scatter(xValues, yValues, marker="+")
    ax.set_yscale('log')
    ax.set_xscale('log')
    ax.set_xlabel(fullNames[tool] + " (s)")
    ax.set_ylabel('ICRA (s)')
    ax.set_title('Running time of ICRA versus ' + fullNames[tool] + '\n')
    ax.plot([minValue, maxValue], [minValue, maxValue], color='k', linestyle='-', linewidth=1)
    for format in ['ps', 'png'] :
        fig.savefig(plotPathTemplate % (tool, format))
    #plt.show() # display plots in a GUI
    inputFile.close()
    selectedKeysGeometricMean = GeometricMean("selectedKeys"+tool)
    for key in keys :
        if key in selectedKeysOther and key in selectedKeysICRA :
            selectedKeysGeometricMean.register(float(otherToolValues[key])/float(icraValues[key]))
    print >>summaryFile, (tool + " all:" + numFormat(allKeysGeometricMean.read())
                   + " -> bothfinished:" + numFormat(selectedKeysGeometricMean.read()) )
summaryFile.close()

with open(htmlFilename, 'w') as F:
    print >>F, "<html><body>"
    print >>F, "<hr>"
    for tool in otherTools :
        print >>F, '<img src="%s"><br>' % ( plotPathTemplate % (tool, 'png') )
        print >>F, "<hr>"
    print >>F, "</html></body>"

