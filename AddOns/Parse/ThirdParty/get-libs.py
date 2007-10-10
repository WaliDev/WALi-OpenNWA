#!/usr/bin/python

import platform
import urllib
import sys
import os

xerces_master = 'http://www.apache.org/dist/xerces/c/binaries/'
xerces_mirror = 'http://mir2.ovh.net/ftp.apache.org/dist/xerces/c/binaries/'

xerces_win32   = 'xerces-c_2_8_0-x86-windows-vc_8_0'
xerces_win64   = 'xerces-c_2_8_0-x86_64-windows-vc_8_0'
xerces_linux32 = 'xerces-c_2_8_0-x86-linux-gcc_3_4'
xerces_linux64 = 'xerces-c_2_8_0-x86_64-linux-gcc_3_4'

def myrm(name):
    if os.path.isdir(name):
        os.system('rm -rf %s' % name)
    elif os.path.isfile(name):
        os.remove(name)

def xerces_setup(name):
    myrm(name)
    myrm(name+'.zip')
    myrm('xerces-c')

def get_xerces_win(name):
    name = xerces_win32 if bits == '32bit' else xerces_win64
    xerces_setup(name)
    target = name+'.zip'
    os.system('wget %s%s' % (xerces_mirror,target))
    os.system('unzip -o %s' % target)
    os.rename(name, 'xerces-c')

def get_xerces_linux(bits):
    name = (xerces_linux64 , xerces_linux32)[bits == '32bit']
    xerces_setup(name)
    target = name+'.tar.gz'
    os.system('wget %s%s' % (xerces_mirror,target))
    os.system('tar zxf %s' % target)
    os.rename(name, 'xerces-c')

def main():
    (bits,linkage) = platform.architecture()
    if bits != '64bit' and bits != '32bit':
        raise NameError, bits
    s = platform.system()
    if s == 'Linux':
        get_xerces_linux(bits)
    else:
        get_xerces_win(bits)


if __name__ == "__main__":
    main()

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

