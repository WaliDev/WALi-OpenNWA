# $Id$
# Build WALi library 

## ####################################
## Third party users of WALi should 
## duplicate the setup code below.
import os,platform,SCons,glob,re

WaliDir       = os.getcwd()
LibInstallDir = os.path.join(WaliDir,'lib')
BaseEnv       = Environment()
MkStatic      = platform.system() != 'Linux' 

if 'gcc' == BaseEnv['CC']:
    BaseEnv['CXXFLAGS'] = '-g -ggdb -Wall -Wformat=2 -W'
elif 'cl' == BaseEnv['CC']:
    # Mostly copied from VS C++ 2005 Command line
    BaseEnv['CXXFLAGS'] = '/TP /errorReport:prompt /Wp64 /W4 /GR /MTd /EHsc'

Export('WaliDir')
Export('LibInstallDir')
Export('MkStatic')
Export('BaseEnv')


## ####################################
## Calling of WALi SConscript files for
## building.
if 'help' not in COMMAND_LINE_TARGETS:
    ## ##################
    ## libwali
    objs = []
    objs.append(SConscript('Source/SConscript', build_dir='_build',duplicate=0))
    
    ## ##################
    ## All
    if 'all' in COMMAND_LINE_TARGETS:
        for d in ['AddOns','Examples']:
            o = SConscript('%s/SConscript' % d)
            objs.append(o)
        BaseEnv.Alias('all',objs)

    ## AddOns
    else:
        if 'addons' in COMMAND_LINE_TARGETS:
            SConscript('AddOns/SConscript')
        if 'examples' in COMMAND_LINE_TARGETS:
            SConscript('Examples/SConscript')
else:
    BaseEnv.Alias('help',[])
    print """
    scons [addons examples all]
    """
