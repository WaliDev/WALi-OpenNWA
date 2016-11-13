import fileinput
import sys
import time

index=fileinput.input("index.html",inplace=True)

for line in index:
	if "<!-- Start Here -->" in line:
		line="<!-- Start Here -->\n\n<a href=\"" + time.strftime("%Y%m%d") + "/result.html\">" + time.strftime("%a, %d %b %Y") + "</a> ( <a href=\"" + time.strftime("%Y%m%d") + "/compare.html\">Comparison</a> ) ( <a href=\"" + time.strftime("%Y%m%d") + "/print_hull.html\">Bounds Generation</a> )<br>\n"
	sys.stdout.write(line)
	

