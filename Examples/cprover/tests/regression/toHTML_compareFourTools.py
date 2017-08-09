#!/usr/bin/python
import os

fin = open('outputs/__result_C4.out', 'r')
fout = open('result_C4.html', 'w')

fout.write('<!DOCTYPE html>\n')
fout.write('<html>\n')
fout.write('<body>\n')

fout.write('<table style="width:100%" border="1">\n')

fout.write('<colgroup>\n')
fout.write('<col span="1" style="width:20%;">\n')
fout.write('<col span="1" style="width:15%;">\n')
#fout.write('<col span="1" style="width:15%;">\n')
fout.write('<col span="1" style="width:13%;">\n')
fout.write('<col span="1" style="width:13%;">\n')
fout.write('<col span="1" style="width:13%;">\n')
fout.write('<col span="1" style="width:13%;">\n')
fout.write('</colgroup>\n')

fout.write('<tr>\n')
fout.write('<th>Test Name</th>\n')
fout.write('<th>Output</th>\n')
fout.write('<th>ICRA</th>\n')
#fout.write('<th>Duet</th>\n')
fout.write('<th>UAutomizer</th>\n')
fout.write('<th>CPAchecker</th>\n')
fout.write('<th>SeaHorn</th>\n')
fout.write('</tr>\n')


firstElem = True
firstAssertion = True
flipResult = False
dirName=os.getcwd()

NO = 0
YES = 1
TIMEOUT = 2
EXCEPTION = 4

bgColor = 0

newtonTime = 0
duetTime = 0
UATime = 0
CPATime = 0
#LPITime = 0
SEATime = 0
newtonAssertionsT = 0
duetAssertionsT = 0
UAAssertionsT = 0
CPAAssertionsT = 0
#LPIAssertionsT = 0
SEAAssertionsT = 0
newtonAssertionsF = 0
duetAssertionsF = 0
UAAssertionsF = 0
CPAAssertionsF = 0
#LPIAssertionsF = 0
SEAAssertionsF = 0
totAssertionsT = 0
totAssertionsF = 0

for line in fin:
	words = line.split()
	if len(line.strip()) == 0 or line.strip().startswith("#") : continue
	if (words[0] == "__DIRECTORY"):		
		if firstElem:
			firstElem = False
		else:
			fout.write('</tr>\n')
			fout.write('<tr align=\"center\" style="background-color:#AAAAAA;">\n')
			fout.write('<td colspan=\"2\"><b>Total</b></td>\n\n')
			fout.write('<td>True: ' + str(newtonAssertionsT) + '/' + str(totAssertionsT))
			#fout.write('<br>False: ' + str(newtonAssertionsF) + '/' + str(totAssertionsF))
			fout.write('<br>Time: ' + str(newtonTime / 1000.0) + '</td>\n')
			#fout.write('<td>True: ' + str(duetAssertionsT) + '/' + str(totAssertionsT))
			#fout.write('<br>False: ' + str(duetAssertionsF) + '/' + str(totAssertionsF))
			#fout.write('<br>Time: ' + str(duetTime / 1000.0) + '</td>\n')
			fout.write('<td>True: ' + str(UAAssertionsT) + '/' + str(totAssertionsT))
			#fout.write('<br>False: ' + str(UAAssertionsF) + '/' + str(totAssertionsF))
			fout.write('<br>Time: ' + str(UATime / 1000.0) + '</td>\n')
			fout.write('<td>True: ' + str(CPAAssertionsT) + '/' + str(totAssertionsT))
			#fout.write('<br>False: ' + str(CPAAssertionsF) + '/' + str(totAssertionsF))
			fout.write('<br>Time: ' + str(CPATime / 1000.0) + '</td>\n')
			#fout.write('<td>True: ' + str(LPIAssertionsT) + '/' + str(totAssertionsT))
			#fout.write('<br>False: ' + str(LPIAssertionsF) + '/' + str(totAssertionsF))
			#fout.write('<br>Time: ' + str(LPITime / 1000.0) + '</td>\n')
			fout.write('<td>True: ' + str(SEAAssertionsT) + '/' + str(totAssertionsT))
			#fout.write('<br>False: ' + str(SEAAssertionsF) + '/' + str(totAssertionsF))
			fout.write('<br>Time: ' + str(SEATime / 1000.0) + '</td>\n')
			fout.write('</tr>\n')
			newtonTime = 0
			duetTime = 0
			UATime = 0
			CPATime = 0
			#LPITime = 0
			SEATime = 0
			newtonAssertionsT = 0
			duetAssertionsT = 0
			UAAssertionsT = 0
			CPAAssertionsT = 0
			#LPIAssertionsT = 0
			SEAAssertionsT = 0
			newtonAssertionsF = 0
			duetAssertionsF = 0
			UAAssertionsF = 0
			CPAAssertionsF = 0
			#LPIAssertionsF = 0
			SEAAssertionsF = 0
			totAssertionsT = 0
			totAssertionsF = 0
			
		fout.write('<tr align=\"center\">\n')
		fout.write('<td colspan=\"7\"><font color=\"#00AAAA\">')
		fout.write(words[1].split("Examples/cprover/tests/")[-1])
		#fout.write(words[1])
		fout.write('</font></td>\n')

	if (words[0] == "__FILENAME"):
		if firstElem:
			firstElem = False
		else:
			fout.write('</tr>\n')
		
		path=words[1]
		fileName=os.path.basename(path)
		bgColor = 1 - bgColor
	
		flipResult = (('unsafe' in fileName) or ('false-unreach-call' in fileName))
		if flipResult:
			totAssertionsF += 1
		else:
			totAssertionsT += 1
		
		
		fout.write('<tr align=\"center\">\n')
		#fout.write('<tr align=\"center\" style="background-color:'+["white","#BBBBBB"][bgColor]+';">\n')
		
		fout.write('<td>')
		fout.write('<a href=\"inputs/' + fileName + '\">' + fileName + '</a>')
		dotIFile = fileName + ".i"
		fout.write('&nbsp;<a href=\"inputs/' + dotIFile + '\">[.i]</a>')
		fout.write('</td>\n')
		
		fout.write('<td>')
		fout.write('<a href=\"outputs/' + fileName + '.below.out\">ICRA</a> ')
		#fout.write('<a href=\"outputs/' + fileName + '.duet.out\">duet</a> ')
		fout.write('<a href=\"outputs/' + fileName + '.ua.out\">UAutomizer</a> ')
		fout.write('<a href=\"outputs/' + fileName + '.cpa.out\">CPAchecker</a> ')
		#fout.write('<a href=\"outputs/' + fileName + '.lpi.out\">LPI</a> ')
		fout.write('<a href=\"outputs/' + fileName + '.sea.out\">SeaHorn</a>')
		fout.write('</td>\n')
	
	elif (words[0] == "__ASSERTION"):
		fout.write('<td>')
		errorReachable = NO
		for i in range(len(words)-1):
			if (words[i+1] == "FAIL"):
				errorReachable = YES
			elif (words[i+1] == "__TIMEOUT"):
				errorReachable = TIMEOUT
				break
				
		if (flipResult):
			if (errorReachable == YES): #ONE FAIL
				#fout.write('<font color=\"#00AA00\">PASS</font><br>')
				fout.write('<font color=\"#00AA00\">OKAY')
				newtonAssertionsF += 1
			elif (errorReachable == NO): #ALL PASS
				#fout.write('<font color=\"#FF0000\">FAIL</font><br>')
				fout.write('<b><font color=\"#FF0000\">UNSOUND</b>')
			elif (errorReachable == TIMEOUT): 
				fout.write('<font color=\"#800080\">TIMEOUT')
		else:
			if (errorReachable == NO): #ALL PASS
				fout.write('<font color=\"#00AA00\">PASS')
				newtonAssertionsT += 1
			elif (errorReachable == YES): #ONE FAIL
				fout.write('<font color=\"#FF0000\">FAIL')
			elif (errorReachable == TIMEOUT):
				fout.write('<font color=\"#800080\">TIMEOUT')
		
				
	elif (words[0] == "__TIMEOUT"):
		fout.write('<td><font color=\"#800080\">TIMEOUT\n')
		
	elif (words[0] == "__EXCEPTION"):
		fout.write('<td><font color=\"#800080\">EXCEPTION\n')
			
	elif (words[0] == "__DUET"):
                pass
		#fout.write('<td>')
		#errorReachable = NO
		#for i in range(len(words)-1):
		#	if (words[i+1] == "FAIL"):
		#		errorReachable = YES
		#	elif (words[i+1] == "TIMEOUT"):
		#		errorReachable = TIMEOUT
		#		break	
		#if (flipResult):
		#	if (errorReachable == YES): #ONE FAIL
		#		#fout.write('<font color=\"#00AA00\">PASS</font><br>')
		#		fout.write('<font color=\"#00AA00\">OKAY')
		#		duetAssertionsF += 1
		#	elif (errorReachable == NO): #ALL PASS
		#		#fout.write('<font color=\"#FF0000\">FAIL</font><br>')
		#		fout.write('<b><font color=\"#FF0000\">UNSOUND</b>')
		#	elif (errorReachable == TIMEOUT): 
		#		fout.write('<font color=\"#800080\">TIMEOUT')
		#else:
		#	if (errorReachable == NO): #ALL PASS
		#		fout.write('<font color=\"#00AA00\">PASS')
		#		duetAssertionsT += 1
		#	elif (errorReachable == YES): #ONE FAIL
		#		fout.write('<font color=\"#FF0000\">FAIL')
		#	elif (errorReachable == TIMEOUT):
		#		fout.write('<font color=\"#800080\">TIMEOUT')
		
	elif (words[0] == "__UA"):
		fout.write('<td>')
		if (flipResult):
			if (words[1] == "FAIL"): 
				#fout.write('<font color=\"#00AA00\">PASS</font><br>')
				fout.write('<font color=\"#00AA00\">OKAY')
				UAAssertionsF += 1
			elif (words[1] == "PASS"): 
				#fout.write('<font color=\"#FF0000\">FAIL</font><br>')
				fout.write('<b><font color=\"#FF0000\">UNSOUND</b>')
			elif (words[1] == "TIMEOUT"): 
				fout.write('<font color=\"#800080\">TIMEOUT')
			elif (words[1] == "UNKNOWN"): 
				fout.write('<font color=\"#D2691E\">UNKNOWN')
			else:
				fout.write('<font color=\"#800080\">ERROR')
		else:
			if (words[1] == "PASS"): 
				fout.write('<font color=\"#00AA00\">PASS')
				UAAssertionsT += 1
			elif (words[1] == "FAIL"): 
				fout.write('<font color=\"#FF0000\">FAIL')
			elif (words[1] == "TIMEOUT"):
				fout.write('<font color=\"#800080\">TIMEOUT')
			elif (words[1] == "UNKNOWN"): 
				fout.write('<font color=\"#D2691E\">UNKNOWN')
			else:
				fout.write('<font color=\"#800080\">ERROR')
		
	elif (words[0] == "__CPA"):
		fout.write('<td>')
		if (flipResult):
			if (words[1] == "FAIL"): 
				#fout.write('<font color=\"#00AA00\">PASS</font><br>')
				fout.write('<font color=\"#00AA00\">OKAY')
				CPAAssertionsF += 1
			elif (words[1] == "PASS"): 
				#fout.write('<font color=\"#FF0000\">FAIL</font><br>')
				fout.write('<b><font color=\"#FF0000\">UNSOUND</b>')
			elif (words[1] == "TIMEOUT"): 
				fout.write('<font color=\"#800080\">TIMEOUT')
			elif (words[1] == "UNKNOWN"): 
				fout.write('<font color=\"#D2691E\">UNKNOWN')
			else:
				fout.write('<font color=\"#800080\">ERROR')
		else:
			if (words[1] == "PASS"):
				fout.write('<font color=\"#00AA00\">PASS')
				CPAAssertionsT += 1
			elif (words[1] == "FAIL"): 
				fout.write('<font color=\"#FF0000\">FAIL')
			elif (words[1] == "TIMEOUT"):
				fout.write('<font color=\"#800080\">TIMEOUT')
			elif (words[1] == "UNKNOWN"): 
				fout.write('<font color=\"#D2691E\">UNKNOWN')
			else:
				fout.write('<font color=\"#800080\">ERROR')
	
	elif (words[0] == "__LPI"):
		pass
		#fout.write('<td>')
		#if (flipResult):
		#	if (words[1] == "FAIL"): 
		#		#fout.write('<font color=\"#00AA00\">PASS</font><br>')
		#		fout.write('<font color=\"#00AA00\">OKAY')
		#		LPIAssertionsF += 1
		#	elif (words[1] == "PASS"): 
		#		#fout.write('<font color=\"#FF0000\">FAIL</font><br>')
		#		fout.write('<b><font color=\"#FF0000\">UNSOUND</b>')
		#	elif (words[1] == "TIMEOUT"): 
		#		fout.write('<font color=\"#800080\">TIMEOUT')
		#	elif (words[1] == "UNKNOWN"): 
		#		fout.write('<font color=\"#D2691E\">UNKNOWN')
		#	else:
		#		fout.write('<font color=\"#800080\">ERROR')
		#else:
		#	if (words[1] == "PASS"):
		#		fout.write('<font color=\"#00AA00\">PASS')
		#		LPIAssertionsT += 1
		#	elif (words[1] == "FAIL"): 
		#		fout.write('<font color=\"#FF0000\">FAIL')
		#	elif (words[1] == "TIMEOUT"):
		#		fout.write('<font color=\"#800080\">TIMEOUT')
		#	elif (words[1] == "UNKNOWN"): 
		#		fout.write('<font color=\"#D2691E\">UNKNOWN')
		#	else:
		#		fout.write('<font color=\"#800080\">ERROR')
	
	elif (words[0] == "__SEA"):
		fout.write('<td>')
		if (flipResult):
			if (words[1] == "FAIL"): 
				#fout.write('<font color=\"#00AA00\">PASS</font><br>')
				fout.write('<font color=\"#00AA00\">OKAY')
				SEAAssertionsF += 1
			elif (words[1] == "PASS"): 
				#fout.write('<font color=\"#FF0000\">FAIL</font><br>')
				fout.write('<b><font color=\"#FF0000\">UNSOUND</b>')
			elif (words[1] == "TIMEOUT"): 
				fout.write('<font color=\"#800080\">TIMEOUT')
			elif (words[1] == "UNKNOWN"): 
				fout.write('<font color=\"#D2691E\">UNKNOWN')
			else:
				fout.write('<font color=\"#800080\">ERROR')
		else:
			if (words[1] == "PASS"):
				fout.write('<font color=\"#00AA00\">PASS')
				SEAAssertionsT += 1
			elif (words[1] == "FAIL"): 
				fout.write('<font color=\"#FF0000\">FAIL')
			elif (words[1] == "TIMEOUT"):
				fout.write('<font color=\"#800080\">TIMEOUT')
			elif (words[1] == "UNKNOWN"): 
				fout.write('<font color=\"#D2691E\">UNKNOWN')
			else:
				fout.write('<font color=\"#800080\">ERROR')
	
	elif (words[0] == "__NTIME"):
		time = int(words[1]) / 1000000
		fout.write(' (' + str(time / 1000.0) + ') </font></td>\n')
		newtonTime += time
		
	elif (words[0] == "__DTIME"):
                pass
		#time = int(words[1]) / 1000000
		#fout.write(' (' + str(time / 1000.0) + ') </font></td>\n')
		#duetTime += time
		
	elif (words[0] == "__UATIME"):
		time = int(words[1]) / 1000000
		fout.write(' (' + str(time / 1000.0) + ') </font></td>\n')
		UATime += time
		
	elif (words[0] == "__CPATIME"):
		time = int(words[1]) / 1000000
		fout.write(' (' + str(time / 1000.0) + ') </font></td>\n')
		CPATime += time
		
	elif (words[0] == "__LPITIME"): pass
		#time = int(words[1]) / 1000000
		#fout.write(' (' + str(time / 1000.0) + ') </font></td>\n')
		#LPITime += time
		
	elif (words[0] == "__SEATIME"):
		time = int(words[1]) / 1000000
		fout.write(' (' + str(time / 1000.0) + ') </font></td>\n')
		SEATime += time
		
	
fout.write('</tr>\n')
fout.write('<tr align=\"center\" style="background-color:#AAAAAA;">\n')
fout.write('<td colspan=\"2\"><b>Total</b></td>\n\n')
fout.write('<td>True: ' + str(newtonAssertionsT) + '/' + str(totAssertionsT))
#fout.write('<br>False: ' + str(newtonAssertionsF) + '/' + str(totAssertionsF))
fout.write('<br>Time: ' + str(newtonTime / 1000.0) + '</td>\n')
#fout.write('<td>True: ' + str(duetAssertionsT) + '/' + str(totAssertionsT))
#fout.write('<br>False: ' + str(duetAssertionsF) + '/' + str(totAssertionsF))
#fout.write('<br>Time: ' + str(duetTime / 1000.0) + '</td>\n')
fout.write('<td>True: ' + str(UAAssertionsT) + '/' + str(totAssertionsT))
#fout.write('<br>False: ' + str(UAAssertionsF) + '/' + str(totAssertionsF))
fout.write('<br>Time: ' + str(UATime / 1000.0) + '</td>\n')
fout.write('<td>True: ' + str(CPAAssertionsT) + '/' + str(totAssertionsT))
#fout.write('<br>False: ' + str(CPAAssertionsF) + '/' + str(totAssertionsF))
fout.write('<br>Time: ' + str(CPATime / 1000.0) + '</td>\n')
#fout.write('<td>True: ' + str(LPIAssertionsT) + '/' + str(totAssertionsT))
#fout.write('<br>False: ' + str(LPIAssertionsF) + '/' + str(totAssertionsF))
#fout.write('<br>Time: ' + str(LPITime / 1000.0) + '</td>\n')
fout.write('<td>True: ' + str(SEAAssertionsT) + '/' + str(totAssertionsT))
#fout.write('<br>False: ' + str(SEAAssertionsF) + '/' + str(totAssertionsF))
fout.write('<br>Time: ' + str(SEATime / 1000.0) + '</td>\n')
fout.write('</tr>\n')

fout.write('</table>\n')

fout.write('</body>\n')
fout.write('</html>\n')

fout.close()

