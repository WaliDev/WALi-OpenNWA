import os

fin = open('outputs/__result.out', 'r')
fout = open('result.html', 'w')

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
fout.write('<th>Run Time</th>\n')
fout.write('<th>Duet Result</th>\n')
fout.write('</tr>\n')


firstElem = True
flipResult = False
dirName=os.getcwd()

# these constants are declared in NewtonCompare.cpp
NEWTON_BELOW = 1
NEWTON_ABOVE = 3

bgColor = 0
colCtr = 6

belowTime = 0.0
aboveTime = 0.0

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
			fout.write('Below Time = ' + str(belowTime / 1000.0) + '<br>')
			fout.write('Above Time = ' + str(aboveTime / 1000.0) + '</font></td>\n')
			fout.write('</tr>\n')
			belowTime = 0.0
			aboveTime = 0.0
			
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
		if (int(words[2]) == NEWTON_ABOVE):
			runMode = 'above'
		else:
			runMode = 'below'
			bgColor = 1 - bgColor
			
		flipResult = (('unsafe' in fileName) or ('false-unreach-call' in fileName))	
		
		fout.write('<tr align=\"center\" style="background-color:'+["white","#AAAAAA"][bgColor]+';">\n')
		
		if (runMode == 'below'):
			fout.write('<td rowspan="2">')
			fout.write('<a href=\"inputs/' + fileName + '\">' + fileName + '</a>')
			fout.write('</td>\n')
		
		fout.write('<td>')
		fout.write('<a href=\"outputs/' + fileName + '.' + runMode + '.out\">' + runMode + '</a>')
		fout.write('</td>\n')
		
		if (runMode == 'below'):
			fout.write('<td rowspan="2">')
			fout.write('<a href=\"outputs/' + fileName + '.duet.out\">duet</a>')
			fout.write('</td>\n')
		
		colCtr = 3
		
	elif (words[0] == "__NUMRNDS"):
		fout.write('<td>')
		fout.write(words[1])
		fout.write('</td>\n')
		colCtr += 1
		
	elif (words[0] == "__NTIME"):
		time = int(words[1]) / 1000000
		if (runMode == 'below'):
			belowTime += time
		elif (runMode == 'above'):
			aboveTime += time
			
		while (colCtr < 5):
			colCtr += 1
			fout.write('<td></td>\n')
		fout.write('<td>')
		fout.write(str(time / 1000.0))
		fout.write('</td>\n')
		colCtr += 1
	
	elif (words[0] == "__ASSERTION"):
		fout.write('<td>')
		for i in range(len(words)-1):
			if (flipResult):
				if (words[i+1] == "FAIL"):
					fout.write('<font color=\"#00AA00\">OKAY</font><br>')
				elif (words[i+1] == "PASS"):
					fout.write('<b><font color=\"#FF0000\">UNSOUND</font></b><br>')
				elif (words[i+1] == "__TIMEOUT"):
					fout.write('<font color=\"#800080\">TIMEOUT</font><br>')
				elif (words[i+1] == "__EXCEPTION"):
					fout.write('<font color=\"#800080\">EXCEPTION</font><br>')
			else:
				if (words[i+1] == "PASS"):
					fout.write('<font color=\"#00AA00\">PASS</font><br>')
				elif (words[i+1] == "FAIL"):
					fout.write('<font color=\"#FF0000\">FAIL</font><br>')
				elif (words[i+1] == "__TIMEOUT"):
					fout.write('<font color=\"#800080\">TIMEOUT</font><br>')
				elif (words[i+1] == "__EXCEPTION"):
					fout.write('<font color=\"#800080\">EXCEPTION</font><br>')
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
					fout.write('<font color=\"#00AA00\">OKAY</font><br>')
				elif (words[i+1] == "PASS"):
					fout.write('<b><font color=\"#FF0000\">UNSOUND</font></b><br>')
				elif (words[i+1] == "TIMEOUT"):
					fout.write('<font color=\"#800080\">TIMEOUT</font><br>')
			else:
				if (words[i+1] == "PASS"):
					fout.write('<font color=\"#00AA00\">PASS</font><br>')
				elif (words[i+1] == "FAIL"):
					fout.write('<font color=\"#FF0000\">FAIL</font><br>')
				elif (words[i+1] == "TIMEOUT"):
					fout.write('<font color=\"#800080\">TIMEOUT</font><br>')
		fout.write('</td>\n')
		colCtr += 1
		
	elif (words[0] == "__TIMEOUT"):	
		fout.write('<td><font color=\"#800080\">TIMEOUT</font></td>\n')
		colCtr += 1
		
	elif (words[0] == "__EXCEPTION"):
		fout.write('<td><font color=\"#800080\">EXCEPTION</font></td>\n')
		colCtr += 1
		
fout.write('</tr>\n')
fout.write('<tr align=\"center\" style="background-color:#000000;">\n')
fout.write('<td colspan=\"4\"><font color=\"#FFFFFF\"><b>Total</b></font></td>\n')
fout.write('<td colspan=\"3\"><font color=\"#FFFFFF\">')
fout.write('Below Time = ' + str(belowTime / 1000.0) + '<br>')
fout.write('Above Time = ' + str(aboveTime / 1000.0) + '</font></td>\n')

fout.write('</tr>\n')
fout.write('</table>\n')

fout.write('</body>\n')
fout.write('</html>\n')

fout.close()

