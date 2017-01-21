#!/usr/bin/python
import sys, os.path, re, subprocess

summary_start_key = "; Procedure summary for "
smtlib_file = os.path.join(os.path.dirname(os.path.realpath(__file__)),"smtlib_output.smt2")

def gatherProcedureNames() :
    procedures = list()
    with open(smtlib_file,"r") as F :
        for line in F :
            if line.startswith(";") :
                matches = re.match(summary_start_key + "'(.*)'", line)
                if matches :
                    procedures.append(matches.group(1))
                else :
                    if line.startswith(summary_start_key) :
                        print "Warning: NewtonOcaml was unable to determine the procedure name for some procedures in this program."
    if len(procedures) == 0 : print "Warning: no procedure summaries were found in the smtlib file."
    return procedures

def extractProcedureSummary(exactName) :
    summary = ""
    inSummary = False
    with open(smtlib_file,"r") as F :
        for line in F :
            if inSummary :
                if line.startswith(";") : break
                summary += line
            else :
                if line.startswith(summary_start_key + "'" + exactName + "'") :
                    inSummary = True
    return summary

def pickProcedureName(proc_descr) :
    procedureNames = gatherProcedureNames()
    if proc_descr in procedureNames : return proc_descr
    superstrings = list()
    for procedureName in procedureNames :
        if proc_descr in procedureName : # is a substring of
            superstrings.append(procedureName)
    if len(superstrings) == 0 :
        print "Error: no procedure names are superstrings of '" + proc_descr + "'"
        sys.exit(1)
    if len(superstrings) > 1 :
        print "Error: more than one procedure name is a superstring of: '" + proc_descr + "'"
        print "  Candidates are: "
        for procedureName in superstrings :
            print "    '" + procedureName + "'"
        sys.exit(1)
    return superstrings[0]

def usage() :
    print ""
    print "USAGE: ask_z3.py --list-procedures"
    print "       ask_z3.py --print-summary <proc_descr>"
    print "       ask_z3.py --check-implication <proc_descr> <SMTLIB formula>"
    print "       ask_z3.py --check-implication-debug <proc_descr> <SMTLIB formula>"
    print ""
    print " Read an SMTLIB2 file from the same directory as this script,"
    print "   having name 'smtlib_output.smt2' and query z3 with it."
    print ""
    print "   --list-procedures lists all procedures described by the SMTLIB2 file."
    print ""
    print "   --print-summary prints the procedure summary for one procedure."
    print ""
    print "   --check-implication will create a z3 input file consisting of a"
    print "     procedure summary and the negation of the provided formula."
    print "     It then sends that file to z3.  If z3 reports UNSAT, then the"
    print "     procedure summary implies the provided formula."
    print ""
    print "   --check-implication-debug is the same, except it only prints the"
    print "     resulting file to the terminal and then exits.  It sends nothing"
    print "     to z3."
    print ""
    print "   Here, <proc_descr> is either an exact mangled name of a procedure,"
    print "     or else it is a string that is a substring of exactly one of the"
    print "     procedures' mangled names."
    print "     (Hint: the procedure name, e.g., doSomeAction, might work.)"
    print ""
    print "   Note: Because mangled names contain spaces and SMTLIB formulas"
    print "     do also, it is strongly suggested that you put both of these"
    print "     inside (single?) quotes on your command line."
    print ""
    print "   Example:  ./ask_z3.py --check-implication 'main' '(< k!10 k!13)'"
    print ""
    sys.exit(0)

def handleArguments() :
    if len(sys.argv) > 1 and sys.argv[1] == "--list-procedures" :
        print "Procedure names (with quotes added for convenience) are: "
        for p in gatherProcedureNames() : print "  '" + p + "'"
        return
    if len(sys.argv) == 3 and sys.argv[1] == "--print-summary" :
        procedureName = pickProcedureName(sys.argv[2])
        print summary_start_key + "'" + procedureName + "'"
        summary = extractProcedureSummary(procedureName)
        print summary
        return
    if len(sys.argv) == 4 and sys.argv[1] in ["--check-implication","--check-implication-debug"] :
        procedureName = pickProcedureName(sys.argv[2])
        query = summary_start_key + "'" + procedureName + "'\n"
        query += extractProcedureSummary(procedureName)
        query += "; Negation of given formula\n"
        query += "(assert (not " + sys.argv[3] + " ))\n"
        query += "(check-sat)\n"
        query += "(get-model)\n"
        query += ""
        if sys.argv[1] == "--check-implication" :
            print "Querying z3 for implication between '" + procedureName + "' and your formula."
            print ""
            child=subprocess.Popen(["z3","-in"],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
            outdata,errdata = child.communicate(query)
            if errdata is None and outdata.strip().startswith('sat\n') : 
                print "The implication does not hold."
                print outdata
                return
            if errdata is None and outdata.strip().startswith('unsat\n') : 
                print "The implication holds."
                return
            print "z3 returned unrecognized output:"
            if outdata : print outdata
            if errdata : print errdata
            return
        else :
            print query
            return
    usage()

if __name__ == '__main__' :
    handleArguments()
