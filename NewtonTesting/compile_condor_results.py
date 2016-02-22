#!/usr/bin/python

import fnmatch
import os
from os.path import join
import argparse


if __name__ == '__main__':

  parser = argparse.ArgumentParser(description='Run NewtonCompare FWPDS only mode on the slam tests cases')
  # executable name, required
  parser.add_argument('--inext', dest='inext', default='bp', help='Extension used to identify candidate input files')
  parser.add_argument('--outext', dest='outext', default='', help='For input file in.inext, stdout > in.inext.outext.out and stderr > in.inext.outext.err. Use - to turn off redirection')
  parser.add_argument('--dir', dest='toplvl', default=".", help='Run in the specified diretory tree')
  parser.add_argument('--bebop', dest='base', default='', help='The list of results from bebop runs')

  args = parser.parse_args()

  pattern = '*.' + args.inext
  for root, dirs, files in os.walk(args.toplvl):
    for filename in fnmatch.filter(files, pattern):
      if args.outext != '-':
        try:
	  fout = open(join(root, filename + '.' + args.outext + '.out'), 'r')
          ferr = open(join(root, filename + '.' + args.outext + '.err'), 'r')
          totalName = filename.split('.')
          bName = totalName[0]
          bRes = open(args.base)
          fout2 = open(join(root, filename + '.' + args.outext + '.out'), 'r')
          Finished = False
          Reachable = False
          AError = False
          for line in bRes:
            if bName in line:
              tName = line.split(';')
              if (bName == tName[0]):
                result = tName[1]
                if 'Pass' in result:
                  BebopReachable = False
                else:
                  BebopReachable = True
          outText = fout.read()
          if 'error reachable' in outText:
            Finished = True
            Reachable = True
            for line in fout2:
              if 'Time taken' in line:
                parts = line.split(':')
                t1 = parts[2].strip()
                parts2 = t1.split(' ')
                DTime = float(parts2[0].strip())
          elif 'error not reachable' in outText:
            Finished = True
            Reachable = False
            for line in fout2:
              if 'Time taken' in line:
                parts = line.split(':')
                t1 = parts[2].strip()
                parts2 = t1.split(' ')
                DTime = float(parts2[0].strip())
          else:
            errText = ferr.read()
            for line in errText:
              if 'Assertion' in line:
                AError = True
                AVal = line
              else:
                AError = False
          fout.close()
          fout2.close()
          ferr.close()
          #os.remove(join(root, filename + '.' + outext + '.out'))
          #os.remove(join(root, filename + '.' + outext + '.err'))
          fres = open(join(root, filename + '.' + args.outext + '.res'), 'w')
          fres.write(join(root, filename))
          fres.write('\n')
          i = 0
          with open(join(root, filename)) as f:
            for i, l in enumerate(f):
              pass
          lC = i + 1
          fres.write(str(lC))
          fres.write('\n')
          if BebopReachable:
            fres.write('BebopReachable\n')
          else:
            fres.write('BebopNotReachable\n')

          if Finished:
            fres.write('Completed\n')
            if Reachable:
              fres.write('Reachable\n')
            else:
              fres.write('NotReachable\n')
            fres.write(str(DTime))
          else:
            if AError:
              fres.write('AssertionError\n')
              fres.write(AVal)
            else:
              fres.write('TimeOut/SpaceOut')
	  fres.close()

	except:
	  print 'BP file never finished.\n'
