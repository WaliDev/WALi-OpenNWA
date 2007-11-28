# Build WALi library 

import os

WaliDir = os.getcwd()
Export('WaliDir')

LibInstallDir = WaliDir + '/lib'
Export('LibInstallDir')

SConscript('Source/SConscript', build_dir='_build',duplicate=0)
SConscript('AddOns/SConscript')
SConscript('Examples/SConscript')
