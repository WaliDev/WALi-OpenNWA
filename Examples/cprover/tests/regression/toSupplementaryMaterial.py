#!/usr/bin/python
#
# toSupplementaryMaterial.py
#   Produce a subdirectory "supplementary" containing files for
#   the supplementary-materials submission to PLDI17.
#   This includes:
#      
#   Inputs: 
#      inputs/*.c, inputs/*.i, outputs/*.out, outputs/*.png
#      (except outputs/__result.out and outputs/__result_PH.out)
#      print_hull.html, results.html
#      (So, please run toHTML.py and toHTML_printHull.py and toPlotsAndSummaryText.py first.)
#
#   Output is written to "supplementary"
#
#   Text files have all occurrences of the name "turetsky" replaced with "icrauser",
#   as appropriate for double-blind submission.

import os.path, shutil, re

destDir = "supplementary"
sourceDir = "."

excludeList = ["__result.out", "__result_PH.out"]
binaryCopy = [".*[.]png"]

inDir = os.path.join(destDir, "inputs")
outDir = os.path.join(destDir, "outputs")

def copyFileCleaned(relative=None,sourcePath=None,destPath=None) :
    if relative is None and (sourcePath is None or destPath is None) :
        raise Exception("Please supply an argument to copyFileCleaned")
    if sourcePath is None : sourcePath = os.path.join(sourceDir,relative)
    if destPath is None : destPath = os.path.join(destDir, relative)
    if any(re.match(R, sourcePath) for R in binaryCopy) :
        # copy binary files (.png) unchanged
        shutil.copyfile(sourcePath, destPath)
        return
    with open(sourcePath, 'r') as S :
        with open(destPath, 'w') as D :
            for line in S :
                line = line.replace("turetsky","icrauser")
                line = line.replace("Turetsky","Icrauser")
                print >> D, line.rstrip()

def copyDirCleaned(relative,regex=None) :
    for base, dirs, files in os.walk(relative) :
        for filename in files :
            path = os.path.join(sourceDir, base, filename)
            if filename not in excludeList and (regex is None or re.search(regex,path)) :
                copyFileCleaned(path)

if os.path.exists(destDir) :
    I = 1 
    while True :
        displaced = "OLD_"+destDir+"_" + str(I)
        if not os.path.exists(displaced) : break
        I += 1
    shutil.move(destDir, displaced)

fileExtensions = r"\.(c|i|out|png)$"
os.mkdir(destDir)
copyFileCleaned("result.html")
copyFileCleaned("tool-comparison-plots.html")
os.mkdir(inDir)
copyDirCleaned("inputs", regex=fileExtensions)
os.mkdir(outDir)
copyDirCleaned("outputs", regex=fileExtensions)
os.rename(os.path.join(destDir,"result.html"),os.path.join(destDir,"assertion-checking-results.html"))
copyFileCleaned(sourcePath=os.path.join(sourceDir,"supp_template","README.txt"),destPath=os.path.join(destDir,"README.txt"))
# NEED TO COPY ALL PRINT-HULL-RELEVANT FILES? (No, as long as test_suite_print_hull has been run recently, they'll be there.)
copyFileCleaned("print_hull.html")
os.rename(os.path.join(destDir,"print_hull.html"),os.path.join(destDir,"bounds-generation-results.html"))
# Zip final output...
# Need to filter the data so we don't get things other than .c,.i,.out,.png copied accidentally...
