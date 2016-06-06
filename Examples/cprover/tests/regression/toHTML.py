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
fout.write('<th>Run Time</th>\n')
fout.write('<th>Result</th>\n')
fout.write('<th>Duet Result</th>\n')
fout.write('</tr>\n')


firstElem = True
firstAssertion = True
flipResult = False
dirName=os.getcwd()
# these constants are declared in NewtonCompare.cpp
NEWTON_BELOW = 1
NEWTON_ABOVE = 3

for line in fin:
	words = line.split()
	if (words[0] == "__DIRECTORY"):	
		if not(firstAssertion):
			firstAssertion = True
			fout.write('</td>\n')
		
		if firstElem:
			firstElem = False
		else:
			fout.write('</tr>\n')

		fout.write('<tr align=\"center\">\n')
		fout.write('<td colspan=\"7\"><font color=\"#00AAAA\">')
		fout.write(words[1])
		fout.write('</font></td>\n')

	if (words[0] == "__FILENAME"):
		if not(firstAssertion):
			firstAssertion = True
			fout.write('</td>\n')
		
		if firstElem:
			firstElem = False
		else:
			fout.write('</tr>\n')
		
		path=words[1]
		fileName=os.path.basename(path)
		if (int(words[2]) == NEWTON_ABOVE):
			runMode = 'above'
		else:
			runMode = 'below'
			
		flipResult = (('unsafe' in fileName) or ('false' in fileName))	
		
		fout.write('<tr align=\"center\">\n')
		
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
		
	elif (words[0] == "__NUMRNDS"):
		fout.write('<td>')
		fout.write(words[1])
		fout.write('</td>\n')
		
	elif (words[0] == "__TIME"):
		fout.write('<td>')
		fout.write(words[1])
		fout.write('</td>\n')
	
	elif (words[0] == "__ASSERTION"):
		if firstAssertion:
			fout.write('<td>')
			firstAssertion = False
		if (flipResult):
			if (words[1] == "FAIL"):
				fout.write('<font color=\"#00AA00\">PASS</font><br>')
			elif (words[1] == "PASS"):
				fout.write('<font color=\"#FF0000\">FAIL</font><br>')
		else:
			if (words[1] == "PASS"):
				fout.write('<font color=\"#00AA00\">PASS</font><br>')
			elif (words[1] == "FAIL"):
				fout.write('<font color=\"#FF0000\">FAIL</font><br>')
			
	elif (words[0] == "__DUET"):
		if not(firstAssertion):
			firstAssertion = True
			fout.write('</td>\n')
			
		fout.write('<td rowspan="2">')
		for i in range(len(words)-1):
			if (flipResult):
				if (words[i+1] == "FAIL"):
					fout.write('<font color=\"#00AA00\">PASS</font><br>')
				elif (words[i+1] == "PASS"):
					fout.write('<font color=\"#FF0000\">FAIL</font><br>')
				elif (words[i+1] == "TIMEOUT"):
					fout.write('<font color=\"#00AAAA\">TIMEOUT</font><br>')
			else:
				if (words[i+1] == "PASS"):
					fout.write('<font color=\"#00AA00\">PASS</font><br>')
				elif (words[i+1] == "FAIL"):
					fout.write('<font color=\"#FF0000\">FAIL</font><br>')
				elif (words[i+1] == "TIMEOUT"):
					fout.write('<font color=\"#00AAAA\">TIMEOUT</font><br>')
		fout.write('</td>')
		
	elif (words[0] == "__TIMEOUT"):
		if firstAssertion:
			fout.write('<td>')
			firstAssertion = False
		
		fout.write('<font color=\"#00AAAA\">TIMEOUT</font><br>')
		
	elif (words[0] == "__EXCEPTION"):
		if firstAssertion:
			fout.write('<td>')
			firstAssertion = False
		
		fout.write('<font color=\"#FF0000\">EXCEPTION</font><br>')
		
if not(firstAssertion):
	fout.write('</td>\n')
fout.write('</tr>\n')
fout.write('</table>\n')

fout.write('</body>\n')
fout.write('</html>\n')

fout.close()

