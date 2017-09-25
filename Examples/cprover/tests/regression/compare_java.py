import os, git_info, subprocess

NEWTON_BELOW = 1
NEWTON_ABOVE = 3

class Result:
    def __init__( self ):
        self.name = ""
        self.runMode = 1
        self.directory = ""
        self.rnds = 9999
        self.time = -1.0
        self.e2eTime = -1
        self.asrt = []
        self.dasrt = []
        self.status = "normal"
        self.nameOfInput = ""

def parseRes( fin ):
    res = Result()
    resList = []
    currentDir = ""
    nextNameOfInput = ""
    nextNameOfOutput = ""
    for line in fin:
        words = line.split()
        if (words[0] == "__DIRECTORY"):
            #currentDir = os.path.basename(words[1])
            currentDir = words[1]
        if (words[0] == "__NAMEOFINPUT"): 
            nextNameOfInput = words[1]
            #print "Correctly read a past nameOfInput: " + res.nameOfInput
        if (words[0] == "__NAMEOFOUTPUT"): 
            nextNameOfOutput = words[1]
        if (words[0] == "__FILENAME"):
            if (res.name != ""):
                resList.append(res)
            res = Result()
            res.name = os.path.basename(words[1])
            if (len(words) > 2):
                res.runMode = int(words[2])
            res.directory = currentDir
            #
            res.nameOfInput = nextNameOfInput   # Note: __NAMEOFINPUT entries appear before __FILENAME entries
            res.nameOfOutput = nextNameOfOutput # Likewise for __NAMEOFOUTPUT
            nextNameOfInput = ""
            nextNameOfOutput = ""
            
        elif (words[0] == "__NUMRNDS"):
            res.rnds = int(words[1])
            
        elif (words[0] == "__TIME"):
            res.time = float(words[1])
        
        elif (words[0] == "__ASSERTION"):
            for i in range(len(words)-1):
                if (words[i+1] == "__TIMEOUT"):
                    res.status = "timeout"
                elif (words[i+1] == "__EXCEPTION"):
                    res.status = "exception"
                else:
                    res.asrt.append(words[i+1])
                    
        elif (words[0] == "__DUET"):
            for i in range(len(words)-1):
                res.dasrt.append(words[i+1])
                        
        elif (words[0] == "__TIMEOUT"):
            res.status = "timeout"
            
        elif (words[0] == "__EXCEPTION"):
            res.status = "exception"
            
        elif (words[0] == "__NTIME"):
            res.e2eTime = int(words[1]) / 1000000
            
    if (res.name != ""):
        resList.append(res)
    return resList

def findRes( resList, name, runMode ):
    for res in resList:
        if ((res.nameOfInput == name) and (res.runMode == runMode)):
            return res
    return None

def getTotalTime( resList, dirName, runMode ):
    time = 0
    for res in resList:
        if ((res.directory == dirName) and (res.runMode == runMode)):
            time += res.e2eTime if (res.e2eTime > 0) else 0
    return time

fin = open('__result_java.out.old', 'r')
oldResList = parseRes(fin)

fin = open('outputs/__result_java.out', 'r')
fout = open('compare_java.html', 'w')

fout.write('<!DOCTYPE html>\n')
fout.write('<html>\n')
fout.write('<body>\n')

waliPath = "/bat0/stac/Code/WALi-Sandbox/WALi-OpenNWA"
fout.write('<details>')
fout.write('<summary><font color="blue">[Version Information]</font></summary><br>')
fout.write('Phase2 (IST-Java-ICRA) WALi-OpenNWA version: '+git_info.getMostRecentCommitHash(waliPath))
fout.write(' ('+git_info.getMostRecentCommitDate(waliPath)+')')
fout.write(' "'+git_info.getMostRecentCommitMessage(waliPath)+'"<br>\n')
duetPath = "/bat0/stac/Code/WALi-Sandbox/duet"
fout.write('Phase2 (IST-Java-ICRA) duet version: '+git_info.getMostRecentCommitHash(duetPath))
fout.write(' ('+git_info.getMostRecentCommitDate(duetPath)+')')
fout.write(' "'+git_info.getMostRecentCommitMessage(duetPath)+'"<br>\n')
fout.write('<br>')
fout.write('Phase3 WALi-OpenNWA version: '+git_info.getMostRecentCommitHash("."))
fout.write(' ('+git_info.getMostRecentCommitDate(".")+')')
fout.write(' "'+git_info.getMostRecentCommitMessage(".")+'"<br>\n')
duetPath = "../../../../../duet"
fout.write('Phase3 duet version: '+git_info.getMostRecentCommitHash(duetPath))
fout.write(' ('+git_info.getMostRecentCommitDate(duetPath)+')')
fout.write(' "'+git_info.getMostRecentCommitMessage(duetPath)+'"<br>\n')
fout.write('<pre>'+subprocess.check_output(['opam','list'])+'</pre><br>')
fout.write('</details>\n')

fout.write('<table style="width:100%" border="1">\n')

fout.write('<colgroup>\n')
fout.write('<col span="1" style="width:15%;">\n')
fout.write('<col span="1" style="width:15%;">\n')
fout.write('<col span="1" style="width:15%;">\n')
fout.write('<col span="1" style="width:15%;">\n')
fout.write('<col span="1" style="width:15%;">\n')
#fout.write('<col span="1" style="width:15%;">\n')
#fout.write('<col span="1" style="width:10%;">\n')
fout.write('</colgroup>\n')

fout.write('<tr>\n')
fout.write('<th>Test Name</th>\n')
fout.write('<th>Output</th>\n')
fout.write('<th>Duet Output</th>\n')
fout.write('<th>No. of Rounds</th>\n')
fout.write('<th>Result</th>\n')
#fout.write('<th>Run Time (Prev.)</th>\n')
#fout.write('<th>Duet Result</th>\n')
fout.write('</tr>\n')


firstElem = True
flipResult = False
res = None

colCtr = 6

belowTime = 0
aboveTime = 0
oldBelowTime = 0
oldAboveTime = 0
runMode = 1

nameOfInput = ""
nameOfOutput = ""

for line in fin:
    words = line.split()
    if (words[0] == "__DIRECTORY"): 
        if firstElem:
            firstElem = False
        else:
            fout.write('</tr>\n')
            fout.write('<tr align=\"center\" style="background-color:#000000;">\n')
            fout.write('<td colspan=\"4\"><font color=\"#FFFFFF\"><b>Total</b></font></td>\n')
            fout.write('<td colspan=\"3\"><font color=\"#FFFFFF\">')
            fout.write('Below Time = ' + str(belowTime / 1000.0))
            fout.write( ' (was ' + str(oldBelowTime / 1000.0) + ')<br>\n')
            #fout.write('Above Time = ' + str(aboveTime / 1000.0))
            #fout.write(' (was ' + str(oldAboveTime / 1000.0) +  ')</font></td>\n')
            fout.write('</tr>\n')
            belowTime = 0
            aboveTime = 0

        #oldBelowTime = getTotalTime(oldResList, os.path.basename(words[1]), NEWTON_BELOW)
        oldBelowTime = getTotalTime(oldResList, words[1], NEWTON_BELOW)
        #oldAboveTime = getTotalTime(oldResList, os.path.basename(words[1]), NEWTON_ABOVE)
        oldAboveTime = 0
        
        fout.write('<tr align=\"center\">\n')
        fout.write('<td colspan=\"7\"><font color=\"#00AAAA\">')
        fout.write(words[1])
        fout.write('</font></td>\n')

    if (words[0] == "__NAMEOFINPUT"): nameOfInput = words[1]
    if (words[0] == "__NAMEOFOUTPUT"): nameOfOutput = words[1]
    if (words[0] == "__FILENAME"):
        while (colCtr < 6):
            colCtr += 1
            fout.write('<td></td>\n')
        fout.write('</tr>\n')
        
        path=words[1]
        fileName=os.path.basename(path)
        if (len(words) > 2):
            runMode = int(words[2])
        
        flipResult = ('unsafe' in fileName) or ('false-unreach-call' in fileName)   
        res = findRes(oldResList, nameOfInput, runMode)
        normalFlag = not((res is None) or (res.status != "normal"))
        
        fout.write('<tr align=\"center\">\n')
        
        if (runMode == NEWTON_BELOW):
            #fout.write('<td rowspan="2">')
            fout.write('<td>')
            fout.write('<a href=\"inputs/' + nameOfInput + '\">' + nameOfInput + '</a>')
            fout.write('</td>\n')
        
        fout.write('<td>')
        #mode = 'below' if (runMode == NEWTON_BELOW) else 'above'
        mode = 'below'
        fout.write('<a href=\"outputs/' + nameOfOutput + '\">' + 'output' + '</a>')
        fout.write('</td>\n')
        
        #if (runMode == NEWTON_BELOW):
        #    #fout.write('<td rowspan="2">')
        #    fout.write('<td>')
        #    fout.write('<a href=\"outputs/' + fileName + '.duet.out\">duet</a>')
        #    fout.write('</td>\n')
        
        colCtr = 3
        
        
    elif (words[0] == "__NUMRNDS"):
        fout.write('<td>')
        if ((res != None) and (int(words[1]) < res.rnds)):
            fout.write('<font color=\"#00AA00\">')
        elif ((res != None) and (int(words[1]) > res.rnds)):
            fout.write('<font color=\"#FF0000\">')
        else:
            fout.write('<font>')
        fout.write(words[1])
        fout.write('</font></td>\n')
        colCtr += 1
        
        
    elif (words[0] == "__NTIME"):
        time = int(words[1]) / 1000000
        if (runMode == NEWTON_BELOW):
            belowTime += time
        elif (runMode == NEWTON_ABOVE):
            aboveTime += time

        while (colCtr < 5):
            colCtr += 1
            fout.write('<td></td>\n')
        fout.write('<td>')
        showOld = False
        if (res != None):
            diff = float(time) / res.e2eTime
            if (diff <= 0.75):
                fout.write('<font color=\"#008000\">')
                showOld = True
            elif (diff <= 0.9):
                fout.write('<font color=\"#00FF00\">')
                showOld = True
            elif (diff <= 1.1):
                fout.write('<font>')
            elif (diff <= 1.25):
                fout.write('<font color=\"#FF5733\">')
                showOld = True
            else:
                fout.write('<font color=\"#FF0000\">')
                showOld = True
        else:
            fout.write('<font>')
        fout.write(str(time / 1000.0) + '</font>')
        if (showOld):
            fout.write(' (' + str(res.e2eTime / 1000.0) + ')')
        fout.write('</td>\n')
        colCtr += 1
        
    
    
    elif (words[0] == "__ASSERTION"):
        fout.write('<td>')
        asrtCtr = 0
        for i in range(len(words)-1):
            if (words[i+1] == "__TIMEOUT" or words[i+1] == "__EXCEPTION"):
                if ((res != None) and (res.status == "normal")):
                    fout.write('<font color=\"#FF0000\">' + words[i+1][2:] + '</font><br>')
                else:
                    fout.write(words[i+1][2:] + '<br>')
            elif (flipResult):
                if ((words[i+1] == "FAIL") and (res != None) and (((asrtCtr < len(res.asrt)) and (res.asrt[asrtCtr] == "PASS")) or (asrtCtr >= len(res.asrt)))):
                    fout.write('<font color=\"#00AA00\">OKAY</font><br>')
                elif (words[i+1] == "FAIL"):
                    fout.write('OKAY<br>')
                elif ((words[i+1] == "PASS") and (res != None) and (asrtCtr < len(res.asrt)) and (res.asrt[asrtCtr] == "FAIL")):
                    fout.write('<font color=\"#FF0000\">UNSOUND</font><br>')
                elif ((words[i+1] == "PASS") and (res != None) and (asrtCtr >= len(res.asrt))):
                    fout.write('<font color=\"#00AA00\">UNSOUND</font><br>')
                elif (words[i+1] == "PASS"):
                    fout.write('UNSOUND<br>')
            else:
                if ((words[i+1] == "PASS") and (res != None) and (((asrtCtr < len(res.asrt)) and (res.asrt[asrtCtr] == "FAIL")) or (asrtCtr >= len(res.asrt)))):
                    fout.write('<font color=\"#00AA00\">PASS</font><br>')
                elif (words[i+1] == "PASS"):
                    fout.write('PASS<br>')
                elif ((words[i+1] == "FAIL") and (res != None) and (asrtCtr < len(res.asrt)) and (res.asrt[asrtCtr] == "PASS")):
                    fout.write('<font color=\"#FF0000\">FAIL</font><br>')
                elif ((words[i+1] == "FAIL") and (res != None) and (asrtCtr >= len(res.asrt))):
                    fout.write('<font color=\"#00AA00\">FAIL</font><br>')
                elif (words[i+1] == "FAIL"):
                    fout.write('FAIL<br>')
            asrtCtr += 1
        fout.write('</td>\n')
        colCtr += 1
    
            
    elif (words[0] == "__DUET"):
        while (colCtr < 6):
            colCtr += 1
            fout.write('<td></td>\n')
        #fout.write('<td rowspan="2">')
        fout.write('<td>')
        for i in range(len(words)-1):
            if (flipResult):
                if (words[i+1] == "FAIL"):
                    fout.write('PASS<br>')
                elif (words[i+1] == "PASS"):
                    fout.write('FAIL<br>')
                elif (words[i+1] == "TIMEOUT"):
                    fout.write('TIMEOUT<br>')
            else:
                if (words[i+1] == "PASS"):
                    fout.write('PASS<br>')
                elif (words[i+1] == "FAIL"):
                    fout.write('FAIL<br>')
                elif (words[i+1] == "TIMEOUT"):
                    fout.write('TIMEOUT<br>')
        fout.write('</td>\n')
        colCtr += 1
        
        
    elif (words[0] == "__TIMEOUT"):
        fout.write('<td>')  
        if ((res != None) and (res.status != "timeout")):
            fout.write('<font color=\"#FF0000\">TIMEOUT</font><br>')
        else:
            fout.write('TIMEOUT<br>')
        fout.write('</td>\n')
        colCtr += 1
        
        
    elif (words[0] == "__EXCEPTION"):
        fout.write('<td>')
        if ((res != None) and (res.status != "exception")):
            fout.write('<font color=\"#FF0000\">EXCEPTION</font><br>')
        else:
            fout.write('EXCEPTION<br>')
        fout.write('</td>\n')
        colCtr += 1

fout.write('</tr>\n')
fout.write('<tr align=\"center\" style="background-color:#000000;">\n')
fout.write('<td colspan=\"4\"><font color=\"#FFFFFF\"><b>Total</b></font></td>\n')
fout.write('<td colspan=\"3\"><font color=\"#FFFFFF\">')
fout.write('Below Time = ' + str(belowTime / 1000.0))
fout.write( ' (was ' + str(oldBelowTime / 1000.0) + ')<br>\n')
#fout.write('Above Time = ' + str(aboveTime / 1000.0))
#fout.write(' (was ' + str(oldAboveTime / 1000.0) +  ')</font></td>\n')


fout.write('</tr>\n')
fout.write('</table>\n')

fout.write('<br><font color=\"#008000\">>25% better</font>')
fout.write('<br><font color=\"#00FF00\">25%-10% better</font>')
fout.write('<br><font>No significant change</font>')
fout.write('<br><font color=\"#FF5733\">10%-25% worse</font>')
fout.write('<br><font color=\"#FF0000\">>25% worse</font>')

fout.write('</body>\n')
fout.write('</html>\n')

fout.close()
