#!/usr/bin/python
# Author: Mads Chr. Olesen <mads@mchro.dk>
#Test using a C++ weight domain in Python

try:
    import wali
except ImportError, e:
    #Trying to run uninstalled
    import os
    import os.path
    root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    lib_dir = os.path.join(root_dir, 'lib')
    print "Could not load the 'wali' module, maybe you need to:"
    print "export LD_LIBRARY_PATH=" + lib_dir
    raise e


reachOne = wali.Reach(True)
reachOnePtr = wali.SemElemPtr(reachOne)
#reachOne = wali.SemElemPtr()

reachZero = wali.Reach(False)
reachZeroPtr = wali.SemElemPtr(reachZero)

#Test domain
wali.test_semelem_impl(reachOnePtr)

myWpds = wali.FWPDS()
p = wali.getKey("p")
accept = wali.getKey("accept")

n = []
for i in range(0, 12):
    n += [wali.getKey("n" + str(i))]

#f intraprocedural
myWpds.add_rule(p, n[0], p, n[1], reachOnePtr)
myWpds.add_rule( p, n[0], p, n[1], reachOnePtr)
myWpds.add_rule( p, n[1], p, n[2], reachOnePtr)
myWpds.add_rule( p, n[1], p, n[3], reachOnePtr)
myWpds.add_rule( p, n[2], p, n[4], reachOnePtr)
myWpds.add_rule( p, n[3], p, n[4], reachOnePtr)

#g intraprocedural
myWpds.add_rule( p, n[6], p, n[7], reachOnePtr)
myWpds.add_rule( p, n[7], p, n[8], reachOnePtr)
myWpds.add_rule( p, n[8], p, n[9], reachOnePtr)

#f call g
myWpds.add_rule( p, n[4], p, n[6], n[5], reachOnePtr)
#f return
myWpds.add_rule( p, n[5] , p , reachOnePtr)
#g return
myWpds.add_rule( p, n[9] , p , reachOnePtr)

#disconnected stuff
myWpds.add_rule( p, n[10], p, n[11], reachOnePtr)

print str(myWpds)

# Perform poststar query
query = wali.WFA()
query.addTrans( p, n[0], accept, reachOnePtr);
query.set_initial_state( p )
query.add_final_state( accept )
print "BEFORE poststar"
print str(query)

answer = wali.WFA()
myWpds.poststar(query, answer)
print "AFTER poststar"
print str(answer)

#Perform prestar query
prequery = wali.WFA()
prequery.addTrans( p, n[4], accept, reachOnePtr )
prequery.set_initial_state( p )
prequery.add_final_state( accept )
print "BEFORE prestar"
print str(prequery)

answer = wali.WFA()
myWpds.prestar(prequery,answer);
print "AFTER poststar"
print str(answer)

