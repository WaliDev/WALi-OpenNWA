#!/usr/bin/python

import platform
import urllib
import sys
import os
(bits,linkage) = platform.architecture()
Is64           = (False,True)[bits == '64bit']
WALiDir        = os.environ['WALiDir']
LibInstallDir  = os.path.join(WALiDir,('lib','lib64')[Is64])
BinInstallDir  = os.path.join(WALiDir,('bin','bin64')[Is64])
XercesTgt      = ('xerces-c','xerces-c64')[Is64]

xerces_master = 'http://www.apache.org/dist/xerces/c/2/binaries/'
xerces_win32   = 'xerces-c_2_8_0-x86-windows-vc_8_0'
xerces_win64   = 'xerces-c_2_8_0-x86_64-windows-vc_8_0'
xerces_linux32 = 'xerces-c_2_8_0-x86-linux-gcc_3_4'
xerces_linux64 = 'xerces-c_2_8_0-x86_64-linux-gcc_3_4'

xerces3_mirror = 'http://apache.cs.utah.edu/xerces/c/3/binaries/'
xerces3_linux32 = 'xerces-c-3.0.0-x86-linux-gcc-3.4'
xerces3_linux64 = 'xerces-c-3.0.0-x86_64-linux-gcc-3.4'

def myrm(name):
    if os.path.isdir(name):
        os.system('rm -rf %s' % name)
    elif os.path.isfile(name):
        os.remove(name)

def xerces_setup(name):
    myrm(name)
    myrm(XercesTgt)

def get_xerces_win(bits):
    name =  (xerces3_win64,xerces3_win32)[bits == '32bit'] 
    target = name+'.zip'
    xerces_setup(target)
    os.system('curl -O %s%s' % (xerces3_mirror,target))
    os.system('unzip -o %s' % target)
    os.rename(name, XercesTgt)

def get_xerces_linux(bits):
    name = (xerces3_linux64 , xerces3_linux32)[bits == '32bit']
    target = name+'.tar.gz'
    xerces_setup(target)
    os.system('curl -O %s%s' % (xerces3_mirror,target))
    os.system('tar zxf %s' % target)
    os.rename(name, XercesTgt)

def getlibs():
    if bits != '64bit' and bits != '32bit':
        raise NameError, bits
    s = platform.system()
    if False == os.path.exists(XercesTgt):
        if s == 'Linux':
            get_xerces_linux(bits)
        else:
            get_xerces_win(bits)


# Work from command line or SConscript
if __name__ in ['__main__']:
    getlibs()

#def get_xerces_py(name):
#    xerces_setup(name)
#    print '  Fetching : %s' % link
#    tick = [0]
#    def reporthook(blocks, block_size, file_size): 
#        xfer[0] += 1
#        tmp = tick[0] + 1
#        if tmp > 10:
#            print '#',
#            sys.stdout.flush()
#            tick[0] = 0
#        else:
#            tick[0] = tmp
#    urllib.urlretrieve( link , targz , reporthook );

