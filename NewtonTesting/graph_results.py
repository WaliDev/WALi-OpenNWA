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


class TimeRes():
  def __init__(self, test, toplvl):
    self.test = test
    self.toplvl = toplvl

  def getTimes(self):
    results = []
    outPath = join(self.test + '.times')
    lineNum1 = 1
    lineNum2 = 7
    outFile = open(outPath, 'w')
    for root, dirs, files in os.walk(self.toplvl):
      bps = fnmatch.filter(files, "*.bp")
      for bp in bps:
        try:
          fullpath = join(root, bp + '.' + self.test + '.res.txt')
          fout = open(fullpath, 'r')
          outText = fout.read();
          if 'NonRec' in outText:
            Type = 'NonRec'
          elif 'Linear' in outText:
            Type = 'Linear'
          elif 'NonLin' in outText:
            Type = 'NonLin'
          else:
            Type = 'NotFound'
          line1 = linecache.getline(fullpath, lineNum1)
          line1N = line1.rstrip('\n')
          line2 = linecache.getline(fullpath, lineNum2)
          line2N = line2.rstrip('\n')
          line3 = linecache.getline(fullpath, 8)
          Lin = False
          print line3;
          if line3 == 'NumRnds\n':
            print 'YAY'
            Lin = True
            line4 = linecache.getline(fullpath, 9)
            line4N = line4.rstrip('\n')
            if line2N != '':
              results.append((line1N,line2N,Type,line4N))
            elif line1N != '':
              print line1N
              line3N = '300'
              results.append((line1N,line3N,Type,line4N))
          else:
            if line2N != '':
              results.append((line1N,line2N,Type))
            elif line1N != '':
              print line1N
              line3N = '300'
              results.append((line1N,line3N,Type))
        except:
          print 'Error opening file: ' + fullpath

    for res in results:
      for i in range(len(res)):
        outFile.write(res[i])
        outFile.write('\t')
      outFile.write('\n')
    outFile.close()

if __name__ == '__main__':
  parser = argparse.ArgumentParser(description="collect results from runs of wpds variants")
  parser.add_argument('--dir', dest='toplvl', default='.')
  parser.add_argument('--testExt', dest='Test', default='-')

  args = parser.parse_args()

  TimeRes(args.Test, args.toplvl).getTimes()
