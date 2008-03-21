# $Id$
# Build WALi library 

## ####################################
## Third party users of WALi should 
## duplicate the setup code below.
import os,platform

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
    built = []
    built = built + SConscript('Source/SConscript', build_dir='_build',duplicate=0)
    
    ## ##################
    ## All
    if 'all' in COMMAND_LINE_TARGETS:
        for d in ['AddOns','Examples','Tests']:
            o = SConscript('%s/SConscript' % d)
            built = built + o
        ### TODO : better printing
        #for b in built:
        #    print b
        BaseEnv.Alias('all',built)

    ## AddOns
    else:
        if 'addons' in COMMAND_LINE_TARGETS:
            built += SConscript('AddOns/SConscript')
            BaseEnv.Alias('addons',built)
        if 'examples' in COMMAND_LINE_TARGETS:
            built += SConscript('Examples/SConscript')
            BaseEnv.Alias('examples',built)
        if 'tests' in COMMAND_LINE_TARGETS:
            built += SConscript('Tests/SConscript')
            BaseEnv.Alias('tests',built)
else:
    BaseEnv.Alias('help',[])
    print """
    scons [all addons examples tests]
    """
