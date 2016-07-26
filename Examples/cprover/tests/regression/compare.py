import os

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

def parseRes( fin ):
	res = Result()
	resList = []
	currentDir = ""
	for line in fin:
		words = line.split()
		if (words[0] == "__DIRECTORY"):
			currentDir = os.path.basename(words[1])
		if (words[0] == "__FILENAME"):
			if (res.name != ""):
				resList.append(res)
			res = Result()
			res.name = os.path.basename(words[1])
			if (len(words) > 2):
				res.runMode = int(words[2])
			res.directory = currentDir
			
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
		if ((res.name == name) and (res.runMode == runMode)):
			return res
	return None

def getTotalTime( resList, dirName, runMode ):
	time = 0
	for res in resList:
		if ((res.directory == dirName) and (res.runMode == runMode)):
			time += res.e2eTime if (res.e2eTime > 0) else 0
	return time

fin = open('__result.out.old', 'r')
oldResList = parseRes(fin)

fin = open('outputs/__result.out', 'r')
fout = open('compare.html', 'w')

fout.write('<!DOCTYPE html>\n')
fout.write('<html>\n')
fout.write('<body>\n')

fout.write('<table style="width:100%" border="1">\n')

fout.write('<colgroup>\n')
fout.write('<col span="1" style="width:15%;">\n')
fout.write('<col span="1" style="width:15%;">\n')
fout.write('<col span="1" style="width:15%;">\n')
fout.write('<col span="1" style="width:15%;">\n')
fout.write('<col span="1" style="width:15%;">\n')
fout.write('<col span="1" style="width:15%;">\n')
fout.write('<col span="1" style="width:10%;">\n')
fout.write('</colgroup>\n')

fout.write('<tr>\n')
fout.write('<th>Test Name</th>\n')
fout.write('<th>Output</th>\n')
fout.write('<th>Duet Output</th>\n')
fout.write('<th>No. of Rounds</th>\n')
fout.write('<th>Result</th>\n')
fout.write('<th>Run Time (Prev.)</th>\n')
fout.write('<th>Duet Result</th>\n')
fout.write('</tr>\n')


firstElem = True
flipResult = False
res = None

colCtr = 6

belowTime = 0
aboveTime = 0
oldBelowTime = 0
oldAboveTime = 0

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
			fout.write('Above Time = ' + str(aboveTime / 1000.0))
			fout.write(' (was ' + str(oldAboveTime / 1000.0) +  ')</font></td>\n')
			fout.write('</tr>\n')
			belowTime = 0
			aboveTime = 0

		oldBelowTime = getTotalTime(oldResList, os.path.basename(words[1]), NEWTON_BELOW)
		oldAboveTime = getTotalTime(oldResList, os.path.basename(words[1]), NEWTON_ABOVE)
		
		fout.write('<tr align=\"center\">\n')
		fout.write('<td colspan=\"7\"><font color=\"#00AAAA\">')
		fout.write(words[1])
		fout.write('</font></td>\n')

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
		res = findRes(oldResList, fileName, runMode)
		normalFlag = not((res is None) or (res.status != "normal"))
		
		fout.write('<tr align=\"center\">\n')
		
		if (runMode == NEWTON_BELOW):
			fout.write('<td rowspan="2">')
			fout.write('<a href=\"inputs/' + fileName + '\">' + fileName + '</a>')
			fout.write('</td>\n')
		
		fout.write('<td>')
		mode = 'below' if (runMode == NEWTON_BELOW) else 'above'
		fout.write('<a href=\"outputs/' + fileName + '.' + mode + '.out\">' + mode + '</a>')
		fout.write('</td>\n')
		
		if (runMode == NEWTON_BELOW):
			fout.write('<td rowspan="2">')
			fout.write('<a href=\"outputs/' + fileName + '.duet.out\">duet</a>')
			fout.write('</td>\n')
		
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
					fout.write('<font color=\"#00AA00\">PASS</font><br>')
				elif (words[i+1] == "FAIL"):
					fout.write('PASS<br>')
				elif ((words[i+1] == "PASS") and (res != None) and (asrtCtr < len(res.asrt)) and (res.asrt[asrtCtr] == "FAIL")):
					fout.write('<font color=\"#FF0000\">FAIL</font><br>')
				elif ((words[i+1] == "PASS") and (res != None) and (asrtCtr >= len(res.asrt))):
					fout.write('<font color=\"#00AA00\">FAIL</font><br>')
				elif (words[i+1] == "PASS"):
					fout.write('FAIL<br>')
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
		fout.write('<td rowspan="2">')
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
fout.write('Above Time = ' + str(aboveTime / 1000.0))
fout.write(' (was ' + str(oldAboveTime / 1000.0) +  ')</font></td>\n')


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
