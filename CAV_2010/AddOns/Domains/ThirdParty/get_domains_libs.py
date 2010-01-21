#!/usr/bin/env python

## ################
## @author kidd

import platform
import urllib
import sys
import os
import shutil

def get_targz(base,libdir):
  wget = "curl -O"
  targz = base + '.tar.gz'
  if False == os.path.exists(base):
    os.system('%s http://pages.cs.wisc.edu/~kidd/dolby/%s' % (wget,targz))
    os.system('tar zxvf %s' % targz)
    cwd = os.getcwd()
    os.system('cd %s; ./configure --prefix=%s --libdir=%s && make -j2 && make install; cd -' % 
        (base,cwd,libdir))
  else:
    # Be sure that the libs are installed, but assume already compiled
    cwd = os.getcwd()
    os.system('cd %s; make install; cd -' % (base))

def get_buddy(libdir):
  get_targz('buddy-2.4',libdir)
  # Copy over the file for Visual Studio
  if platform.system() == 'Windows':
      shutil.copyfile('buddy-2.4-config.h', 'buddy-2.4/src/config.h')

def getlibs(libdir):
  get_buddy(libdir)

if __name__ in ['__main__']:
  (bits,linkage) = platform.architecture()
  Is64           = (False,True)[bits == '64bit']
  cwd            = os.getcwd()
  libdir         = os.path.join(cwd,('lib','lib64')[Is64])
  getlibs(libdir)

