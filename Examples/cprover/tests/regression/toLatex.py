#!/usr/bin/python
#
# toLatex.py
#   Produce a LaTeX table in the manner of the PLDI17 submission.
#   Input is read from outputs/result.csv
#   (So, please run toCSV.py first.)
#   Output is written to outputs/experiments-table.tex

import os.path, csv, sys

outputDir = "outputs"
outputFilename = "experiments-table.tex"
inputFilename = "result.csv"
outputPath = os.path.join(outputDir, outputFilename)
inputPath = os.path.join(outputDir, inputFilename)
runID = "<unknown>"

excludedFiles = list()
if "--exclude" in sys.argv :
    exclusionFilePath = sys.argv[sys.argv.index("--exclude") + 1]
    with open(exclusionFilePath) as exclusionFile :
        for line in exclusionFile :
            line = line.strip()
            if line.startswith("./") : line = line[2:]
            if len(line) == 0 : continue
            excludedFiles.append(line.strip())

dataOrdering = [
  ("Rec. (tot.)", [ # grouping
    " \hhline{|b:============:b|}", # literal strings for line breaks
    " \hhline{|t:============:t|}",
    ("sv-benchmarks/recursive", "recursive"), # (directory, name in table)
    " \hhline{||------------||}",
    ("sv-benchmarks/recursive-simple", "rec.-simple"), 
    " \hhline{||============||}",
  ]),
  ("Loops (tot.)",[
    " \hhline{|b:============:b|}",
    " \hhline{|t:============:t|}",
    ("sv-benchmarks/loop-acceleration", "loop-accel."),
    " \hhline{||------------||}",
    ("sv-benchmarks/loop-invgen", "loop-invgen"),
    " \hhline{||------------||}",
    ("sv-benchmarks/loop-lit", "loop-lit"),
    " \hhline{||------------||}",
    ("sv-benchmarks/loops", "loops",),
    " \hhline{||------------||}",
    ("sv-benchmarks/loop-new", "loop-new"),
    " \hhline{||============||}",
  ]),
  ("C4B",[
    " \hhline{|b:============:b|}",
    " \hhline{|t:============:t|}",
    ("c4b", "C4B"),
  ]),
  ("Misc.-Rec.",[
    " \hhline{|b:============:b|}",
    " \hhline{|t:============:t|}",
    ("misc-recursive", "misc"),
    " \hhline{||------------||}",
    ("rec-loop-lit", "rec-loop-lit"),
    " \hhline{||------------||}",
    ("rec-loop-new", "rec-loop-new"),
    " \hhline{||============||}",
  ]),
]

toolOrdering = ["ICRA","UA","CPA","LPI","SEA"]

inputFile = open(inputPath, 'r')
inputCSV = csv.reader(inputFile)

header = inputCSV.next()
#Expect exactly: "dir","filename","tool","desiredOutput","rawOutput","conclusion","time"

dirDict = dict()
for directory, filename, tool, desiredOutput, rawOutput, conclusion, time in inputCSV :
    if os.path.join(directory,filename) in excludedFiles :
        print "Excluded file: " + os.path.join(directory, filename)
        continue
    if directory not in dirDict: dirDict[directory] = list()
    dirDict[directory].append([filename, tool, desiredOutput, rawOutput, conclusion, time])

inputFile.close()

F = open(outputPath,"w")

print >>F, r"""
\begin{tabular}{@{\hspace{0ex}}||@{\hspace{.35ex}}l@{\hspace{.35ex}}||@{\hspace{.35ex}}c@{\hspace{.35ex}}||@{\hspace{.35ex}}r@{\hspace{.35ex}}|@{\hspace{.35ex}}c@{\hspace{.35ex}}||@{\hspace{.35ex}}r@{\hspace{.35ex}}|@{\hspace{.35ex}}c@{\hspace{.35ex}}||@{\hspace{.35ex}}r@{\hspace{.35ex}}|@{\hspace{.35ex}}c@{\hspace{.35ex}}||@{\hspace{.35ex}}r@{\hspace{.35ex}}|@{\hspace{.35ex}}c@{\hspace{.35ex}}||@{\hspace{.35ex}}r@{\hspace{.35ex}}|@{\hspace{.35ex}}c@{\hspace{.35ex}}||@{\hspace{0ex}}}
\hhline{|t:============:t|}
Benchmark          & Total & \multicolumn{2}{@{\hspace{0ex}}c@{\hspace{0ex}}||@{\hspace{.4ex}}}{ICRA} & \multicolumn{2}{@{\hspace{0ex}}c@{\hspace{0ex}}||@{\hspace{.4ex}}}{UAut.} & \multicolumn{2}{@{\hspace{0ex}}c@{\hspace{0ex}}||@{\hspace{.4ex}}}{CPA} & \multicolumn{2}{@{\hspace{0ex}}c@{\hspace{0ex}}||@{\hspace{.4ex}}}{LPI} & \multicolumn{2}{@{\hspace{0ex}}c@{\hspace{0ex}}||@{\hspace{0ex}}}{SEA} \\
\hhline{||~||-----------||}
Suite              & \#A & \multicolumn{1}{c|}{Time} & \#A & \multicolumn{1}{c|}{Time} & \#A & \multicolumn{1}{c|}{Time} & \#A & \multicolumn{1}{c|}{Time} & \#A & \multicolumn{1}{c|}{Time} & \#A \\"""

def format(f) : return "%.1f" % f
#def format(f) : return "%.2f" % f # till Tuesday

def createLatexRow(name, inputDataRows) :
    toolTimes = dict()
    toolTruePositives = dict()
    toolTrueNegatives = dict()
    maxTruePositives = 0
    maxTrueNegatives = 0
    minTime = None
    totalPositives = 0
    totalNegatives = 0
    for tool in toolOrdering :
        toolTimes[tool] = toolTruePositives[tool] = toolTrueNegatives[tool] = 0
    for filename, tool, desiredOutput, rawOutput, conclusion, time in inputDataRows :
        if conclusion == "PASS" :
            toolTruePositives[tool] += 1 # We wanted the assertion to pass, and the tool said "pass"
            maxTruePositives = max(maxTruePositives, toolTruePositives[tool])
        if conclusion == "OKAY" :
            toolTrueNegatives[tool] += 1 # We wanted the assertion to fail, and the tool said "fail"
            maxTrueNegatives = max(maxTrueNegatives, toolTrueNegatives[tool])
        toolTimes[tool] += float(time)
        if tool == toolOrdering[0] :
            if desiredOutput == "shouldPass" : totalPositives += 1
            if desiredOutput == "shouldFail" : totalNegatives += 1
    for tool in toolOrdering :
        if minTime is None : minTime = toolTimes[tool]
        minTime = min(minTime, toolTimes[tool])
    output = " " + name + "&" + str(totalPositives) + "/" + str(totalNegatives)
    for tool in toolOrdering :
        time = toolTimes[tool]
        if time == minTime :
            output += r"& \textbf{" + format(time) + "} &"
        else :
            output += "& " + format(time) + " &"
        tp = toolTruePositives[tool]
        if tp == maxTruePositives :
            output += r"\textbf{" + str(tp) + "}/"
        else :
            output += str(tp) + "/"
        tn = toolTrueNegatives[tool]
        if tn == maxTrueNegatives and maxTrueNegatives > 0 :
            output += r"\textbf{" + str(tn) + "}"
        else :
            output += str(tn)
    output += r" \\"
    return output 

for groupingName, commands in dataOrdering :
    numDirectoriesInGrouping = len([1 for C in commands if not isinstance(C, basestring)])
    allGroupingDataRows = list()
    for command in commands :
        if isinstance(command, basestring) :
            print >>F, command
            continue
        directory, abbreviation = command
        allGroupingDataRows.extend(dirDict[directory])
        if numDirectoriesInGrouping == 1 : abbreviation = r"\textbf{" + abbreviation + "}"
        print >>F, createLatexRow(abbreviation, dirDict[directory])
    if numDirectoriesInGrouping > 1 :
        print >>F, createLatexRow(r"\textbf{"+groupingName+"}", allGroupingDataRows)

print >>F, r"""
 \hhline{|b:============:b|}
\end{tabular}"""

F.close()
