#!/usr/bin/env python

## ###############
## @author kidd

import os
import platform
import shutil
import sys
import xerces_consts

name = xerces_consts.Name
bits = xerces_consts.bits
IsLinux = xerces_consts.IsLinux
xrcs3_mirror  = 'http://apache.cs.utah.edu/xerces/c/3/binaries/'

def myrm(name):
  if os.path.isdir(name):
    os.system('rm -rf %s' % name)
  elif os.path.isfile(name):
    os.remove(name)

def getlibs(LibInstallDir):
  built = []
  if False == os.path.exists(name):
    target = name+('.zip','.tar.gz')[IsLinux]
    myrm(target)
    os.system('curl -O %s%s' % (xrcs3_mirror,target))
    os.system('%s %s' % (('unzip -o','tar zxf')[IsLinux],target))
  cwd = os.getcwd()
  if False == os.path.exists(LibInstallDir):
    os.mkdir(LibInstallDir)
  for lib in os.listdir(os.path.join(name,'lib')):
    src = os.path.join(cwd,name,'lib',lib)
    dst = os.path.join(LibInstallDir,lib)
    print 'Src = %s' % src
    print 'Dst = %s' % dst
    shutil.copy2(src,dst)
    built += [dst]
  return (name,built)


# Work from command line or SConscript
if __name__ in ['__main__']:
  if bits != '64bit' and bits != '32bit':
    raise NameError, bits
  Is64           = (False,True)[bits == '64bit']
  getlibs(os.path.join(os.getcwd(),'lib'))


