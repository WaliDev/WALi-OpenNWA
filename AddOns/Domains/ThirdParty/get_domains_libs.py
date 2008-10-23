#!/usr/bin/python

import platform
import urllib
import sys
import os
import shutil
(bits,linkage) = platform.architecture()
Is64           = (False,True)[bits == '64bit']
WALiDir        = os.environ['WALiDir']
LibInstallDir  = os.path.join(WALiDir,('lib','lib64')[Is64])
BinInstallDir  = os.path.join(WALiDir,('bin','bin64')[Is64])

def get_targz(base):
  wget = "curl -O"
  targz = base + '.tar.gz'
  if not os.path.isfile(targz):
      os.system('%s http://pages.cs.wisc.edu/~kidd/dolby/%s' % (wget,targz))
      os.system('tar zxvf %s' % targz)
      cwd = os.getcwd()
      bindir = os.path.join(WALiDir,'bin')
      os.system('cd %s; ./configure --prefix=%s --libdir=%s --bindir=%s && make -j2 && make install; cd -' % 
          (base,cwd,LibInstallDir,BinInstallDir))

def get_buddy():
  get_targz('buddy-2.4')
  # Copy over the file for Visual Studio
  if platform.system() == 'Windows':
      shutil.copyfile('buddy-2.4-config.h', 'buddy-2.4/src/config.h')

def getlibs():
  get_buddy()

if __name__ in ['__main__']:
  getlibs()

