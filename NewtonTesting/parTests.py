#!/usr/bin/python
from Queue import Queue
from threading import Thread
import threading
import time
import fnmatch
import os
from os.path import join
import subprocess
import argparse
import win32api
import signal

cmd = ''
pattern = '*.*'
timeout = 30 * 60 #seconds
outext = ''

class RunCmd(threading.Thread):
  def __init__(self, root, filename, timeout, base):
    threading.Thread.__init__(self)
    self.root = root
    self.base = base
    self.filename = filename
    self.timeout = timeout
    self.timedOut = False

  def run(self):
    cmdtorun = cmd.format(os.path.join(self.root,self.filename))
    print(cmdtorun)
    if outext != '-':
      foutW = open(join(self.root, self.filename + '.' + outext + '.out'), 'w')
      ferrW = open(join(self.root, self.filename + '.' + outext + '.err'), 'w')
      self.proc = subprocess.Popen(cmdtorun, shell=True, stdout = foutW, stderr = ferrW)
      foutW.close()
      ferrW.close()
      fout = open(join(self.root, self.filename + '.' + outext + '.out'), 'r')
      ferr = open(join(self.root, self.filename + '.' + outext + '.err'), 'r')
      self.proc.wait()
      partialName = self.filename.split('\\')
      print partialName
      pName = partialName[0]
      totalName = pName.split('.')
      bName = totalName[0]
      print totalName
      print bName
      bRes = open(self.base)
      fout2 = open(join(self.root, self.filename + '.' + outext + '.out'), 'r')
      Finished = False
      Reachable1 = False
      AError1 = False
      Reachable2 = False
      AError2 = False
      Reachable3 = False
      AError3 = False
      RoundCount = False
      for line in bRes:
        if bName in line:
          tName = line.split(';')
          if (bName == tName[0]):
            result = tName[1]
            #print result
            if 'Pass' in result:
              BebopReachable = False
            else:
              BebopReachable = True
      if self.timedOut:
        Finished = False
      else:
        outText = fout.read()
        if 'Newton Time' in outText:
          Finished = True
          Reachable = True
          for line in fout2:
            if 'EWPDS TIME' in line:
              parts = line.split(':')
              print parts
              t1 = parts[1].strip()
              parts2 = t1.split(' ')
              DTime0 = float(parts2[0].strip())
            if 'FWPDS TIME' in line:
              parts = line.split(':')
              print parts
              t1 = parts[1].strip()
              parts2 = t1.split(' ')
              DTime1 = float(parts2[0].strip())
            if 'Newton Time' in line:
              parts = line.split(':')
              print parts
              t1 = parts[1].strip()
              parts2 = t1.split(' ')
              DTime2 = float(parts2[0].strip())
        elif 'Newton:' in outText:
          print "YAY"
          Finished = True
          Reachable = True
          for line in fout2:
            if 'NumRnds:' in line:
              parts = line.split(':')
              print parts
              t1 = parts[1].strip()
              NRnds = int(t1.strip())
              RoundCount = True
              print NRnds
            if 'Time taken' in line:
              parts = line.split(':')
              print parts
              t1 = parts[2].strip()
              parts2 = t1.split(' ')
              DTime = float(parts2[0].strip())
        if 'NonRec' in outText:
          NonRec = True
          Lin = False
          NonLin = False
        elif 'linAfterGLP' in outText:
          Lin = True
          NonRec = False
          NonLin = False
        else:
          NonLin = True
          Lin = False
          NonRec = False
        if 'error reachable' in outText:
          Finished = True
          Reachable = True
          for line in fout2:
            if 'Time taken' in line:
              parts = line.split(':')
              print parts
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
      #os.remove(join(self.root, self.filename + '.' + outext + '.out'))
      #os.remove(join(self.root, self.filename + '.' + outext + '.err'))
      fres = open(join(self.root, self.filename + '.' + outext + '.res.txt'), 'w')
      fres.write(join(self.root, self.filename))
      fres.write('\n')
      i = 0
      with open(join(self.root, self.filename)) as f:
        for i, l in enumerate(f):
          pass
      lC = i + 1
      fres.write(str(lC))
      fres.write('\n')
      print self.filename
      if BebopReachable:
        fres.write('BebopReachable\n')
      else:
        fres.write('BebopNotReachable\n')
      
      if self.timedOut:
        fres.write('Timeout\n')
      else:
        if Finished:
          fres.write('Completed\n')
          if Reachable:
            fres.write('Reachable\n')
          else:
            fres.write('NotReachable\n')
          fres.write('Time\n')
          fres.write(str(DTime))
          #fres.write('\n')
          #fres.write('FWPDS Time\n')
          #fres.write(str(DTime1))
          #fres.write('\n')
          #fres.write('Newton Time\n')
          #fres.write(str(DTime2))
          fres.write('\n')
          if RoundCount:
            fres.write('NumRnds\n')
            fres.write(str(NRnds))
            fres.write('\n')
          if NonRec:
            fres.write('NonRec\n')
          elif Lin:
            fres.write('Linear\n')
          else:
            fres.write('NonLin\n')
        else:
          if AError:
            fres.write('AssertionError\n')
            fres.write(AVal)
          else:
            fres.write('SpaceOut')
      fres.close()

    else:
      self.proc = subprocess.Popen(cmdtorun, shell=True)
      self.proc.wait()

  def Run(self):
    self.start()
    self.join(timeout)
    PROCESS_TERMINATE = 1
    if self.is_alive():
      print "TIMEOUT: " + os.path.join(self.root, self.filename) 
      self.timedOut = True
      try:
        subprocess.call(['taskkill','/F','/T','/PID',str(self.proc.pid)])
        #self.proc.kill()
      except Exception, e:
        print "Could not terminate: " + str(e)
      self.join()


class Worker(Thread):
  """Thread executing tasks from a given tasks queue"""
  def __init__(self, tasks):
    Thread.__init__(self)
    self.tasks = tasks
    self.daemon = True
    self.start()

  def run(self):
    while True:
      c, root, filename = self.tasks.get()
      try: 
        print("Task #" + str(c))
        RunCmd(root, filename, timeout, args.base).Run()
      except Exception, e: 
        print e
      self.tasks.task_done()

class ThreadPool:
  """Pool of threads consuming tasks from a queue"""
  def __init__(self, num_threads):
    self.tasks = Queue(num_threads)
    for _ in range(num_threads): Worker(self.tasks)

  def add_task(self, c, root, filename):
    """Add a task to the queue"""
    self.tasks.put((c, root, filename))

  def wait_completion(self):
    """Wait for completion of all the tasks in the queue"""
    self.tasks.join()

if __name__ == '__main__':

  parser = argparse.ArgumentParser(description='Run NewtonCompare FWPDS only mode on the slam tests cases')
  # executable name, required
  parser.add_argument(dest='executable', default="", help='Executable to run')
  parser.add_argument(dest='extra_args', default="", nargs="*", help='Any extra arguments to be passed to the executable, appended after the input file name')
  parser.add_argument('--nthreads', dest='nthreads', type=int, default=1, help='Number of threads to spawn for the experiment')
  parser.add_argument('--timeout', dest='timeout', type=int, default=30, help='Timeout for each run, in minutes')  
  parser.add_argument('--inext', dest='inext', default='bp', help='Extension used to identify candidate input files')
  parser.add_argument('--outext', dest='outext', default='', help='For input file in.inext, stdout > in.inext.outext.out and stderr > in.inext.outext.err. Use - to turn off redirection')
  parser.add_argument('--fromDir', dest='src', type=int, default=1, help='Do the bebop files come from a directory or a list of filenames')
  parser.add_argument('--dir', dest='toplvl', default=".", help='Run in the specified diretory tree or from list of files')
  parser.add_argument('--bebop', dest='base', default='', help='The list of results from bebop runs')

  args = parser.parse_args()

  print ('####################################################')
  cmd = args.executable
  cmd = cmd + ' {0} '
  for i in args.extra_args:
    cmd = cmd + ' ' + i
  pattern = '*.' + args.inext
  print ('Command run will be ' + cmd.format('INFILE'))
  print ('  where INFILE is of matches ' + pattern)

  timeout = args.timeout*60  #seconds
  outext = args.outext
  print ('Timeout for each run is {0} seconds.'.format(timeout))
  print ('Number of threads spawned is {0}.'.format(args.nthreads))
  print ('####################################################')

  # 1) Init a Thread pool with the desired number of threads
  pool = ThreadPool(args.nthreads)
  
  # 2) Run Newton on all programs
  filesbysize = {}
  c = 0
  timeStart = time.strftime("%H:%M:%S")
  if args.src == 1:
    for root, dirs, files in os.walk(args.toplvl):
      for filename in fnmatch.filter(files, pattern):
        pool.add_task(c, root, filename)
        c = c + 1
  else:
    f = open(args.toplvl,'r')
    for line in f:
      print line
      nLine = line.rstrip('\n')
      print nLine
      pool.add_task(c, "", nLine)
      c = c + 1
  # 3) Wait for completion
  pool.wait_completion()
  timeEnd = time.strftime("%H:%M:%S")
  print ('\nstarttime: ' + timeStart)
  print ('\nendtime: ' + timeEnd)