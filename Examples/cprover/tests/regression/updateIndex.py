import fileinput
import sys
import time

index=fileinput.input("index.html",inplace=True)

if len(sys.argv) < 3 :
    date_Ymd = time.strftime("%Y%m%d") 
    date_adbY = time.strftime("%a, %d %b %Y") 
else :
    date_Ymd = sys.argv[1]
    date_adbY = sys.argv[2]

for line in index:
	if "<!-- Start Here -->" in line:
		line=("<!-- Start Here -->\n\n"+
                      "<a href=\"" + date_Ymd + "/result.html\">" + date_adbY + "</a>"+
                      " ( <a href=\"" + date_Ymd + "/compare.html\">Comparison</a> )" +
                      " ( <a href=\"" + date_Ymd + "/print_hull.html\">Bounds Generation</a> )" +
                      " ( <a href=\"" + date_Ymd + "/result_java.html\">Java Results</a> )" + 
                      " ( <a href=\"" + date_Ymd + "/compare_java.html\">Java Comparison</a> )" +
                      #" ( <a href=\"" + date_Ymd + "/result_ocrs.html\">OCRS</a> )" +
                      #" ( <a href=\"" + date_Ymd + "/compare_ocrs.html\">OCRS Comparison</a> )" +
                      "<br>\n")
	sys.stdout.write(line)
	

