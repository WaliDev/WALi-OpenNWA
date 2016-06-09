
csurf -nogui -l <path-to-script/script-name.stk $PROJ_NAME.prj

-------------------------------------------------------------------
bta_l.stk, bta_d.stk, and dump-sdg.stk are just examples obtained
from Venkatesh.

-------------------------------------------------------------------
How to have a script obtain input from the command line:

In bta_d.stk, consider the following snippet (way at the bottom):

        (process-main pdg (process-ea-args *userargv* '()) deps)

In the above line *userargv* is the list of command line
arguments. For this particular script, the command line argument is a
list of effective addresses.

The function process-ea-args in the same script iterates over the list
of (string) arguments, and converts it into a list of numbers.
