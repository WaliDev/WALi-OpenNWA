#!/usr/bin/python
#
# toCSV.py
#   Produce a csv file summarizing the results of an experimental run.
#   Input is read from outputs/__result.out
#   (You run test_suite_compareFiveTools.sh to perform the experiments and generate __result.out)
#   Output is written to outputs/result.csv

import os, csv

csvPath = 'outputs/result.csv'
fin = open('outputs/__result.out', 'r')
csvFile = open(csvPath, 'w')
csvWriter = csv.writer(csvFile, quoting=csv.QUOTE_ALL)

firstElem = True
firstAssertion = True
flipResult = False
dirName=os.getcwd()

NO = 0
YES = 1
TIMEOUT = 2
EXCEPTION = 4

fileName = ""
dirName = ""
currentTool = ""
toolRawResult = "?"
toolSummaryResult = "?"
toolTime = "?"
desiredResult = "?"
veryFirstRow = True

lineNumber = 0

toolCountsPerBenchmark = dict()

def setTool(tool) :
    global currentTool, toolRawResult, toolSummaryResult, toolTime
    currentTool = tool
    toolRawResult = "?"
    toolSummaryResult = "?"
    toolTime = "?"

def setToolResult(tool, rawResult, summaryResult) :
    global currentTool, toolRawResult, toolSummaryResult, toolTime, lineNumber
    if tool != currentTool :
        print "ERROR on line " + str(lineNumber)
        print "currentTool = " + currentTool
        print "tool = " + tool
        raise Exception("toCSV.py failed because it found lines of its input in an unexpected order (1)")
    toolRawResult = rawResult
    toolSummaryResult = summaryResult

def setToolTime(tool, time) :
    global currentTool, toolRawResult, toolSummaryResult, toolTime
    if tool != currentTool :
        raise Exception("toCSV.py failed because it found lines of its input in an unexpected order (2)")
    toolTime = time

def writeRowIfComplete() :
    global fileName, dirName, currentTool, toolRawResult, toolSummaryResult, toolTime, veryFirstRow, toolCountsPerBenchmark
    if toolRawResult == "?" or toolTime == "?" : return
    if veryFirstRow :
        veryFirstRow = False
        csvWriter.writerow(["dir","filename","tool","desiredOutput","rawOutput","conclusion","time"])
    csvWriter.writerow([dirName,fileName,currentTool,desiredResult,toolRawResult,toolSummaryResult,toolTime])
    toolRawResult = "?"
    toolSummaryResult = "?"
    toolTime = "?"
    benchmark = os.path.join(dirName, fileName)
    if benchmark not in toolCountsPerBenchmark : toolCountsPerBenchmark[benchmark] = 0
    toolCountsPerBenchmark[benchmark] += 1

for line in fin:
    lineNumber += 1
    if len(line.strip()) == 0 or line.strip().startswith("#") : continue
    words = line.split()
    if (words[0] == "__DIRECTORY"):     
        if firstElem:
            firstElem = False
        else:
            pass
            
    if (words[0] == "__FILENAME"):
        setTool("")
        if firstElem:
            firstElem = False
        else:
            pass
        
        path=words[1]
        fileName=os.path.basename(path)
        dirName=os.path.dirname(path.split("/cprover/tests/")[-1])
    
        flipResult = (('unsafe' in fileName) or ('false-unreach-call' in fileName))
        if flipResult :
            desiredResult = "shouldFail"
        else :
            desiredResult = "shouldPass"
        
    elif (words[0] == "__ASSERTION"):
        setTool("ICRA")
        errorReachable = NO
        for i in range(len(words)-1):
            if (words[i+1] == "FAIL"):
                errorReachable = YES
            elif (words[i+1] == "__TIMEOUT"):
                errorReachable = TIMEOUT
                break
                
        if (flipResult):
            if (errorReachable == YES): #ONE FAIL
                setToolResult("ICRA", "FAIL", "OKAY")
            elif (errorReachable == NO): #ALL PASS
                setToolResult("ICRA", "PASS", "UNSOUND")
            elif (errorReachable == TIMEOUT): 
                setToolResult("ICRA", "TIMEOUT", "TIMEOUT")
        else:
            if (errorReachable == NO): #ALL PASS
                setToolResult("ICRA", "PASS", "PASS")
            elif (errorReachable == YES): #ONE FAIL
                setToolResult("ICRA", "FAIL", "FAIL")
            elif (errorReachable == TIMEOUT):
                setToolResult("ICRA", "TIMEOUT", "TIMEOUT")
                
    elif (words[0] == "__TIMEOUT"):
        setToolResult("ICRA", "TIMEOUT", "TIMEOUT")
        
    elif (words[0] == "__EXCEPTION"):
        setTool("ICRA")
        setToolResult("ICRA", "ERROR", "ERROR")
            
    #elif (words[0] == "__DUET"):
    #    pass
        #fout.write('<td>')
        #errorReachable = NO
        #for i in range(len(words)-1):
        #   if (words[i+1] == "FAIL"):
        #       errorReachable = YES
        #   elif (words[i+1] == "TIMEOUT"):
        #       errorReachable = TIMEOUT
        #       break   
        #if (flipResult):
        #   if (errorReachable == YES): #ONE FAIL
        #       #fout.write('<font color=\"#00AA00\">PASS</font><br>')
        #       fout.write('<font color=\"#00AA00\">OKAY')
        #       duetAssertionsF += 1
        #   elif (errorReachable == NO): #ALL PASS
        #       #fout.write('<font color=\"#FF0000\">FAIL</font><br>')
        #       fout.write('<b><font color=\"#FF0000\">UNSOUND</b>')
        #   elif (errorReachable == TIMEOUT): 
        #       fout.write('<font color=\"#800080\">TIMEOUT')
        #else:
        #   if (errorReachable == NO): #ALL PASS
        #       fout.write('<font color=\"#00AA00\">PASS')
        #       duetAssertionsT += 1
        #   elif (errorReachable == YES): #ONE FAIL
        #       fout.write('<font color=\"#FF0000\">FAIL')
        #   elif (errorReachable == TIMEOUT):
        #       fout.write('<font color=\"#800080\">TIMEOUT')
    
    elif (words[0] in ["__UA", "__CPA", "__LPI", "__SEA"]) :
        toolHere = words[0][2:]
        setTool(toolHere)
        if (flipResult):
            if (words[1] == "FAIL"): 
                setToolResult(toolHere, "FAIL", "OKAY")
            elif (words[1] == "PASS"): 
                setToolResult(toolHere, "PASS", "UNSOUND")
            elif (words[1] == "TIMEOUT"): 
                setToolResult(toolHere, "TIMEOUT", "TIMEOUT")
            elif (words[1] == "UNKNOWN"): 
                setToolResult(toolHere, "UNKNOWN", "UNKNOWN")
            else: 
                setToolResult(toolHere, "ERROR", "ERROR")
        else:
            if (words[1] == "PASS"): 
                setToolResult(toolHere, "PASS", "PASS")
            elif (words[1] == "FAIL"): 
                setToolResult(toolHere, "FAIL", "FAIL")
            elif (words[1] == "TIMEOUT"):
                setToolResult(toolHere, "TIMEOUT", "TIMEOUT")
            elif (words[1] == "UNKNOWN"): 
                setToolResult(toolHere, "UNKNOWN", "UNKNOWN")
            else:
                setToolResult(toolHere, "ERROR", "ERROR")

    elif (words[0] == "__NTIME"):
        time = int(words[1]) / 1000000
        setToolTime("ICRA", str(time / 1000.0))
  
    elif (words[0] in ["__UATIME","__CPATIME","__LPITIME","__SEATIME"]) :
        time = int(words[1]) / 1000000
        toolHere = words[0][2:-4]
        setToolTime(toolHere, str(time / 1000.0))
        
    #elif (words[0] == "__DTIME"): pass
        #time = int(words[1]) / 1000000
        #fout.write(' (' + str(time / 1000.0) + ') </font></td>\n')
        #duetTime += time
        
    writeRowIfComplete()

csvFile.close()

expectedNumber = toolCountsPerBenchmark.values()[0]
if not all([V == expectedNumber for V in toolCountsPerBenchmark.values()]) :
    os.unlink(csvPath)
    raise Exception("Sanity check failure in toCSV.py: some tool didn't report an output for some benchmark!")
#print expectedNumber

