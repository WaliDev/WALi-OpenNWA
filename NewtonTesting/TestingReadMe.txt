Python Readme:

In this folder are python scripts to run on the slam boolean programs

parTests - runs the tests
		 - Most of the arguments .h is self explanatory
			- outext is the extension that other python scripts will need to run this specific test
			- If --fromDir = 0, then --dir is expecting a text file
			- bebop use base.txt in this folder
		 - Depending on the path to the program (if using a text file) line 25: pName = partialName[0] might need to have its number changed
		 
compile_results - Does a comparison with bebop
				- the test extension is the one from when parTests was run
				- bebop = 1 does a comparison with the slam programs in regards to error reachability
				- bebop = 0, search = 1 with return a list of either unsafe = 1 (we disagreed with bebop and said an error was unreachable) unsafe = 2 (unfinished results) unsafe = 3 (we said an error was reachable where bebop said it wasn't)
				-- sorted will sort the list returns by search from smallest program to largest (based on number of lines)
				
graph_results - (Poorly Named) Returns a list of programs, the time it took them to run in seconds (with 300 being the default if unfinished) and if
				newton with tensor was run, the linearity of the equation set (non-recursive, linear, non linear).  If either Newton method is run, graph_results will also return the rounds it took to converges (note that newton with tensor checks for linearity, so it will sometimes complete in one round whereas newton without tensoring will complete in two
				
scatter - graphs the results - Graphs two np.arrays of the same size, point by point.  You will need to go in and set the axis names and the data 	sets to graph

compile_condor_results - Does correctness checking of the condor runs



Condor Files:

The files named "h1, h2, etc." are condor submit files.

Results:

The excel graph contains the final correct results of the various binrel Newton comparison runs

collection.xip - The folder containing the bp files
			 Also contains files of previous runs - if hitting space issues, start deleting *.out, *.err, and *.txt
			 
InterestingTests.txt - A file naming the tests run for the paper
TestText.txt - A file containing names of a small subset of tests to run


png files (The graphs of the final results)
