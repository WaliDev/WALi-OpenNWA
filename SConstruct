# Build WALi library 

## ####################################
## Third party users of WALi should 
## duplicate the setup code below.
import os,platform,platform

(bits,linkage) = platform.architecture()
## ####################
## To distinguish 64 bit build and lib dirs,
## enable this flag
#Is64           = (False,True)[bits == '64bit']
Is64           = False
WaliDir        = os.getcwd()
LibInstallDir  = os.path.join(WaliDir,('lib','lib64')[Is64])
BuildDir       = os.path.join(WaliDir,('_build','_build64')[Is64])
BaseEnv        = Environment()
MkStatic       = platform.system() != 'Linux' 

if 'gcc' == BaseEnv['CC']:
    BaseEnv['CXXFLAGS'] = '-g -ggdb -Wall -Wformat=2 -W'
elif 'cl' == BaseEnv['CC']:
    # Mostly copied from VS C++ 2005 Command line
    BaseEnv['CXXFLAGS'] = '/TP /errorReport:prompt /Wp64 /W4 /GR /MTd /EHsc'
BaseEnv['CPPPATH'] = [ os.path.join(WaliDir , 'Source') ]

Export('Is64')
Export('WaliDir')
Export('LibInstallDir')
Export('BuildDir')
Export('MkStatic')
Export('BaseEnv')

## Setup a default environment for building executables that use WALi
ProgEnv = BaseEnv.Clone()
 
if MkStatic:
  ProgEnv['StaticLibs'] = [os.path.join(LibInstallDir,'libwali.a')]
else:
  ProgEnv['StaticLibs'] = []
  ProgEnv['LIBS'] = ['wali']
  ProgEnv['LIBPATH'] = [ LibInstallDir ]

Export('ProgEnv')


## ####################################
## Calling of WALi SConscript files for
## building.
if 'help' not in COMMAND_LINE_TARGETS:
    ## ##################
    ## libwali
    built = SConscript('Source/SConscript', build_dir=BuildDir,duplicate=0)
    built += SConscript('Doc/tex/SConscript')

    ## ##################
    ## All
    if 'all' in COMMAND_LINE_TARGETS:
        for d in ['AddOns','Examples','Tests']:
            built += SConscript('%s/SConscript' % d)
        BaseEnv.Alias('all',built)

    ## AddOns
    else:
        if 'addons' in COMMAND_LINE_TARGETS:
            built += SConscript('AddOns/SConscript')
            for b in built:
              print b
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

