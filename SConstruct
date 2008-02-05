# $Id$
# Build WALi library 

import os

WaliDir = os.getcwd()
Export('WaliDir')

LibInstallDir = WaliDir + '/lib'
Export('LibInstallDir')

def IsTarget(name):
    return name in COMMAND_LINE_TARGETS
Export('IsTarget')

## ##################
## libwali
SConscript('Source/SConscript', build_dir='_build',duplicate=0)

## ##################
## AddOns
#if IsTarget('addons'):
#    SConscript('AddOns/SConscript')
SConscript('AddOns/SConscript')

## ##################
## Examples
#if IsTarget('examples'):
#    SConscript('Examples/SConscript')
SConscript('Examples/SConscript')
