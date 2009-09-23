#!/usr/bin/python
# Author: Mads Chr. Olesen <mads@mchro.dk>
#Test creating a weight domain in Python, and using it

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

class PyReach(wali.PySemElem):
    numReaches = 0
    O = None
    Z = None
    myFalse = False
    myTrue = True

    def __init__(self, b=False):
        wali.PySemElem.__init__(self)

        #test setting of userdata
        self.isreached = b
        #test retrieval of userdata
        assert self.isreached == b

    @wali.PySemElem.printexc
    def pyone(self):
        if not PyReach.O:
            PyReach.O = PyReach(True)
        return wali.PySemElemPtr(PyReach.O)

    @wali.PySemElem.printexc
    def pyzero(self):
        if not PyReach.Z:
            PyReach.Z = PyReach(False)
        return wali.PySemElemPtr(PyReach.Z)

    @wali.PySemElem.printexc
    @wali.PySemElem.deref_ifneeded
    def pyextend(self, se):
        return wali.PySemElemPtr(PyReach(self.getuserdata()['isreached'] and se.getuserdata()['isreached']))
        
    @wali.PySemElem.printexc
    @wali.PySemElem.deref_ifneeded
    def pycombine(self, se):
        isreached = self.getuserdata()['isreached'] or se.getuserdata()['isreached']
        toret = PyReach(isreached)
        return wali.PySemElemPtr(toret)

    @wali.PySemElem.printexc
    def pyequal(self, se):
        return (self.getuserdata()['isreached'] == se.getuserdata()['isreached'])

    @wali.PySemElem.printexc
    def __str__(self):
        if self.getuserdata()["isreached"] == True:
            return "ONE"
        else:
            return "ZERO"

reachOne = PyReach(True)
reachOnePtr = wali.SemElemPtr(reachOne)

reachZero = PyReach(False)
reachZeroPtr = wali.PySemElemPtr(reachZero)

#Test domain
wali.test_semelem_impl(reachOnePtr)

print "One element: ", str(reachOne)
print "Zero element: ", str(reachZero)

myWpds = wali.FWPDS()
p = wali.getKey("p")
accept = wali.getKey("accept")

n = []
for i in range(0, 12):
    n += [wali.getKey("n" + str(i))]

#f intraprocedural
myWpds.add_rule(p, n[0], p, n[1], reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)

myWpds.add_rule( p, n[0], p, n[1], reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)
myWpds.add_rule( p, n[1], p, n[2], reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)
myWpds.add_rule( p, n[1], p, n[3], reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)
myWpds.add_rule( p, n[2], p, n[4], reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)
myWpds.add_rule( p, n[3], p, n[4], reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)

#g intraprocedural
myWpds.add_rule( p, n[6], p, n[7], reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)
myWpds.add_rule( p, n[7], p, n[8], reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)
myWpds.add_rule( p, n[8], p, n[9], reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)

#f call g
myWpds.add_rule( p, n[4], p, n[6], n[5], reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)
#f return
myWpds.add_rule( p, n[5] , p , reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)
#g return
myWpds.add_rule( p, n[9] , p , reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)

#disconnected stuff
myWpds.add_rule( p, n[10], p, n[11], reachOnePtr)
reachOnePtr = wali.SemElemPtr(reachOne)

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


#Test unref'ing
reachOne.unref()
import gc
gc.collect()
