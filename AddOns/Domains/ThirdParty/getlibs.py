#!/usr/bin/python

import platform
import urllib
import sys
import os
import shutil

def get_targz(base):
    wget = "wget"
    if 'Darwin' == platform.system():
        wget = "curl -O"
    targz = base + '.tar.gz'
    if not os.path.isfile(targz):
        os.system('%s http://pages.cs.wisc.edu/~kidd/dolby/%s' % (wget,targz))
        os.system('tar zxvf %s' % targz)
        cwd = os.getcwd()
        # lib dir is CPDS/lib
        libdir = os.path.join(cwd,'..','lib')
        # bin dir is CPDS/bin
        bindir = os.path.join(cwd,'..','bin')
        os.system('cd %s; ./configure --prefix=%s && make -j2 && make install; cd -' % 
            (base,cwd))

def get_buddy():
    get_targz('buddy-2.4')
    # Copy over the file for Visual Studio
    if platform.system() == 'Windows':
        shutil.copyfile('buddy-2.4-config.h', 'buddy-2.4/src/config.h')

def main():
    get_buddy()

if __name__ in ['__main__','getlibs']:
    main()

