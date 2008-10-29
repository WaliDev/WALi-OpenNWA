#!/usr/bin/python

import platform
import urllib
import sys
import os
import shutil

def get_targz(base,libdir,bindir):
  wget = "curl -O"
  targz = base + '.tar.gz'
  if not os.path.isfile(targz):
      os.system('%s http://pages.cs.wisc.edu/~kidd/dolby/%s' % (wget,targz))
      os.system('tar zxvf %s' % targz)
      cwd = os.getcwd()
      os.system('cd %s; ./configure --prefix=%s --libdir=%s --bindir=%s && make -j2 && make install; cd -' % 
          (base,cwd,libdir,bindir))

def get_buddy(libdir,bindir):
  get_targz('buddy-2.4',libdir,bindir)
  # Copy over the file for Visual Studio
  if platform.system() == 'Windows':
      shutil.copyfile('buddy-2.4-config.h', 'buddy-2.4/src/config.h')

def getlibs(libdir,bindir):
  get_buddy(libdir,bindir)

if __name__ in ['__main__']:
  (bits,linkage) = platform.architecture()
  Is64           = (False,True)[bits == '64bit']
  cwd            = os.getcwd()
  LibInstallDir  = os.path.join(cwd,('lib','lib64')[Is64])
  BinInstallDir  = os.path.join(cwd,('bin','bin64')[Is64])
  getlibs(LibInstallDir,BinInstallDir)

