import os

fin = open('outputs/__result_PH.out', 'r')
fout = open('print_hull.html', 'w')

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
fout.write('<th>Status</th>\n')
fout.write('<th>Run Time</th>\n')
fout.write('</tr>\n')


firstElem = True
dirName=os.getcwd()

bgColor = 0
normal = True
didWeGetATimeout = False

for line in fin:
        words = line.split()
        if (len(words) == 0):
                continue
                
        elif (words[0] == "__DIRECTORY"):       
                if firstElem:
                        firstElem = False
                else:
                        fout.write('</tr>\n')
                fout.write('<tr align=\"center\">\n')
                fout.write('<td colspan=\"7\"><font color=\"#00AAAA\">')
                fout.write(words[1])
                fout.write('</font></td>\n')

        elif (words[0] == "__FILENAME"):
                didWeGetATimeout = False
                fout.write('</tr>\n')
                path=words[1]
                fileName=os.path.basename(path)
                bgColor = 1 - bgColor
                fout.write('<tr align=\"center\" style="background-color:'+["white","#AAAAAA"][bgColor]+';">\n')
                fout.write('<td>')
                fout.write('<a href=\"inputs/' + fileName + '\">' + fileName + '</a>')
                fout.write('</td>\n')
                fout.write('<td>')
                fout.write('<a href=\"outputs/' + fileName + '.out\">Output</a>')
                fout.write('</td>\n')
                normal = True
                colCtr = 2
                
        elif (words[0] == "__ASSERTION"):
                if "__TIMEOUT" in words :
                    didWeGetATimeout = True

        elif (words[0] == "__NUMRNDS"):
                fout.write('<td>')
                fout.write(words[1])
                fout.write('</td>\n')
                colCtr += 1
        
        elif (words[0] == "__NTIME"):
                time = int(words[1]) / 1000000  
                while (colCtr < 3):
                        colCtr += 1
                        fout.write('<td></td>\n')
                if (didWeGetATimeout):
                        fout.write('<td><font color=\"#FF0000\">TIMEOUT</font></td>\n')
                elif (normal):
                        fout.write('<td><font color=\"#00FF00\">NORMAL</font></td>\n')
                else :
                        fout.write('<td><font color=\"#FF00FF\">UNKNOWN</font></td>\n')
                fout.write('<td>')
                fout.write(str(time / 1000.0))
                fout.write('</td>\n')
                colCtr += 1             
                
        elif (words[0] == "__TIMEOUT"): 
                fout.write('<td><font color=\"#800080\">TIMEOUT</font></td>\n')
                normal = False
                colCtr += 1     
                
        elif (words[0] == "__EXCEPTION"):
                fout.write('<td><font color=\"#800080\">EXCEPTION</font></td>\n')
                normal = False
                colCtr += 1
                
fout.write('</tr>\n')
fout.write('</table>\n')

fout.write('</body>\n')
fout.write('</html>\n')

fout.close()

