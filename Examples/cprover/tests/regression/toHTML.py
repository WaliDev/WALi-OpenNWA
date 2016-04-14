import os

fin = open('outputs/result.out', 'r')
fout = open('result.html', 'w')

fout.write('<!DOCTYPE html>\n')
fout.write('<html>\n')
fout.write('<body>\n')

fout.write('<table style="width:100%" border="1">\n')

fout.write('<colgroup>\n')
fout.write('<col span="1" style="width:20%;">\n')
fout.write('<col span="1" style="width:20%;">\n')
fout.write('<col span="1" style="width:20%;">\n')
fout.write('<col span="1" style="width:20%;">\n')
fout.write('<col span="1" style="width:20%;">\n')
fout.write('</colgroup>\n')

fout.write('<tr>\n')
fout.write('<th>Test Name</th>\n')
fout.write('<th>Output</th>\n')
fout.write('<th>No. of Rounds</th>\n')
fout.write('<th>Run Time</th>\n')
fout.write('<th>Result</th>\n')
fout.write('</tr>\n')


firstElem = True
firstAssertion = True
dirName=os.getcwd()

for line in fin:
	words = line.split()
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
		
		fout.write('<tr align="center">\n')
		fout.write('<td>')
		fout.write('<a href=\"' + path + '\">' + fileName + '</a>')
		fout.write('</td>\n')
		
		fout.write('<td>')
		fout.write('<a href=\"' + dirName + '/outputs/' + fileName + '.out\">output</a>')
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
			
		if (words[1] == "PASS"):
			fout.write('<font color=\"#00AA00\">PASS</font><br>')
		elif (words[1] == "FAIL"):
			fout.write('<font color=\"#FF0000\">FAIL</font><br>')
		
	elif (words[0] == "__TIMEOUT"):
		if firstAssertion:
			fout.write('<td>')
			firstAssertion = False
		
		fout.write('<font color=\"#00AAAA\">TIMEOUT</font><br>')
		
	

fout.write('</table>\n')

fout.write('</body>\n')
fout.write('</html>\n')

fout.close()

