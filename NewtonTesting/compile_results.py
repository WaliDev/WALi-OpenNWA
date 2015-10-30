#!/usr/bin/python
from os.path import join
import os
import fnmatch
import argparse
import linecache
class bcolors:
  WARNING = '\033[93m'
  ERROR = '\033[91m'
  ENDC = '\033[0m'


class Collation():
  def __init__(self, test, type, toplvl,sort):
    self.test = test
    self.type = type
    self.toplvl = toplvl
    self.sort = sort

  def searchList(self):
    results = []
    if self.type == 1:
      outPath = join(self.test + '.unsafeRes')
      lineNum1 = 3
      lineNum2 = 5
      lineVal1 = 'BebopReachable\n'
      lineVal2 = 'NotReachable\n'
    elif self.type == 3:
      outPath = join(self.test + '.safeRes')
      lineNum1 = 3
      lineNum2 = 5
      lineVal1 = 'BebopNotReachable\n'
      lineVal2 = 'Reachable\n'
    elif self.type == 2:
      outPath = join(self.test + '.unfinishedRes.txt')
      lineVal = 'Completed\n'
    elif self.type == 4:
      outPath = join(self.test + '.RoundAverage.txt')
      lineVal = 'NumRnds\n'
      lineNum = 9
      numCom = 0
      numRoundTot = 0
    outFile = open(outPath, 'w')
    for root, dirs, files in os.walk(self.toplvl):
      bps = fnmatch.filter(files, "*.bp")
      for bp in bps:
        try:
          fullpath = join(root, bp + '.' + self.test + '.res.txt')
          if ((self.type == 1) or (self.type == 3)):
    	    line1 = linecache.getline(fullpath, lineNum1)
    	    if lineVal1 == line1:
    	      line2 = linecache.getline(fullpath, lineNum2)
    	      if line2 == lineVal2:
    	        size = linecache.getline(fullpath, 2)
    	        bpPath = join(root,bp)
    	        numLines = int(size)
    	        results.append((bpPath,numLines))
    	  elif (self.type == 2):
    	    line1 = linecache.getline(fullpath, 4)
    	    isBDD = True
    	    if line1 != lineVal:
    	      if line1 == 'SpaceOut\n':
                size = linecache.getline(fullpath, 2)
                #errpath = join(root, bp + '.' + self.test + '.err')
                #errFind = open(errpath, 'r')
                #for line6 in errFind:
                #  if 'BDD error' in line6:
                #    isBDD = False
                #if isBDD:
                bpPath = join(root,bp)
                numLines = int(size)
                results.append((bpPath,numLines))
                #errFind.close()
          else:
            lineT = linecache.getline(fullpath, 8)
            if lineT == lineVal:
              numRounds = linecache.getline(fullpath, 9)
              numCom = numCom + 1;
              numRoundTot = int(numRounds) + numRoundTot
    	except:
          bErr = True
    	  #print 'Error opening file: ' + fullpath

    if (self.type != 4):
      sorted_res = sorted(results, key=lambda element : element[1])
      for res in sorted_res:
        outFile.write(res[0])
        outFile.write('\n')
    else:
      outFile.write(str(numCom))
      outFile.write('\n')
      outFile.write(str(numRoundTot))
    outFile.close()

  def bebopCompare(self):
    timedOut = 0
    spacedOut = 0
    Aborted = 0
    bebopReachable = 0
    BRReachable = 0
    BRNon = 0
    bebopNon = 0
    BNReach = 0
    BNNon = 0
    Other = 0
    
    print 'Starting!'

    for root, dirs, files in os.walk(self.toplvl):
      bps = fnmatch.filter(files, "*.bp")
      for bp in bps:
        try:
          fullpath = join(root, bp + '.' + self.test + '.res.txt')
          foo = open(fullpath)
          text = foo.read()
          if 'Timeout' in text:
            timedOut = timedOut + 1
          elif 'SpaceOut' in text:
            spacedOut = spacedOut + 1
          elif 'Assertion' in text:
            Aborted = Aborted + 1
          elif 'Completed' in text:
            if 'BebopNotReachable' in text:
    	      bebopNon = bebopNon + 1
              line = linecache.getline(fullpath, 5)
    	      if 'NotReachable\n' == line:
    	        BNNon = BNNon + 1
    	      else:
    	        BNReach = BNReach + 1
            else:
    	      bebopReachable = bebopReachable + 1
    	      line = linecache.getline(fullpath, 5)
              if 'NotReachable\n' == line:
                BRNon = BRNon + 1
              else:
                BRReachable = BRReachable + 1
          else:
    	    print fullpath
            Other = Other + 1

          foo.close()
        except:
          Done = False
	  #Other = Other + 1

    bar = open(self.test + '.output','w')
    bar.write('Number of timeouts: ')
    bar.write(str(timedOut))
    bar.write('\nNumber of spaceouts: ')
    bar.write(str(spacedOut))
    bar.write('\nNumber of AssertErrors: ')
    bar.write(str(Aborted))
    bar.write('\nNumber Bebop and ' + self.test + ' said were reachable: ')
    bar.write(str(BRReachable))
    bar.write('\nNumber Bebop said were Reachable and ' + self.test + ' said were not reachable: ')
    bar.write(str(BRNon))
    bar.write('\nNumber Bebop and ' + self.test + ' said were not reachable: ')
    bar.write(str(BNNon))
    bar.write('\nNumber Bebop said were Not Reachable and ' + self.test + ' said were reachable: ')
    bar.write(str(BNReach))
    bar.write('\nNumber uncompleted for other reasons: ')
    bar.write(str(Other))
    bar.close()

if __name__ == '__main__':
  parser = argparse.ArgumentParser(description="collect results from runs of wpds variants")
  parser.add_argument('--dir', dest='toplvl', default='.')
  parser.add_argument('--bebopComp', dest='bComp', type=int, default=0)
  parser.add_argument('--testExt', dest='Test', default='-')
  parser.add_argument('--search', dest='search', type=int, default=0)
  parser.add_argument('--unsafe', dest='type', type=int, default=0)
  parser.add_argument('--sorted', dest='sort', type=int, default=0)

  args = parser.parse_args()

  if args.bComp == 1:
    Collation(args.Test, args.type, args.toplvl,args.sort).bebopCompare()
  elif args.search == 1:
    Collation(args.Test, args.type, args.toplvl,args.sort).searchList()
