# Build WALi library 

## ####################################
## Third party users of WALi should 
## duplicate the setup code below.
## TODO: move setup code to an include file
import os, os.path, platform

Debug = True
DefaultEnvironment(MSVC_USE_SCRIPT=False) # for Windows

#(platform_bits,linkage) = platform.architecture()
if platform.machine() == 'i686':
   platform_bits = 32
elif platform.machine() == 'x86_64':
   platform_bits = 64
else:
    print "Warning: Check platform_bits for machine type", platform.machine()
    print "         Assuming 32"
    platform_bits = 32


Platform       = platform.system()
MkStatic       = (Platform == 'Windows')
WaliDir        = os.getcwd()
BaseEnv        = Environment(MSVC_USE_SCRIPT=False)
Is64           = (platform_bits == 64)

ThirtyTwoBitAliases=['32', 'x86', 'ia_32', 'ia32']
SixtyFourBitAliases=['64', 'x64', 'x86_64', 'amd64']

vars = Variables()
vars.Add(EnumVariable('arch', 'Architecture', 'default',
         allowed_values=ThirtyTwoBitAliases+SixtyFourBitAliases+['default']))
tempEnviron = Environment(tools=[], variables=vars)
arch = tempEnviron['arch']
tempEnviron = None
vars = None

if arch in ThirtyTwoBitAliases:
    Is64 = False
elif arch in SixtyFourBitAliases:
    Is64 = True


if Is64:
    LibInstallDir  = os.path.join(WaliDir,'lib64')
    BuildDir       = os.path.join(WaliDir,'_build64')
else:
    LibInstallDir  = os.path.join(WaliDir,'lib')
    BuildDir       = os.path.join(WaliDir,'_build')


if 'gcc' == BaseEnv['CC']:
    BaseEnv.Append(CCFLAGS='-g -ggdb -Wall -Wformat=2 -W -O2')
    if platform_bits == 64 and not Is64:
        # If we're on a 64-bit platform but want to compile for 32.
        # This will only happen if Is64 is changed from what it is
        # right now (Is64 = (platform_bits == 64))
        BaseEnv.Append(CCFLAGS='-m32')
        BaseEnv.Append(LINKFLAGS='-m32')
elif 'cl' == BaseEnv['CC']:
    # Mostly copied from VS C++ 2005 Command line
    BaseEnv.Append(CFLAGS='/TP /errorReport:prompt /Wp64 /W4 /GR /MTd /EHsc')
BaseEnv.Append(CPPPATH = [os.path.join(WaliDir , 'Source')])
BaseEnv.Append(CPPPATH = [os.path.join(WaliDir , 'Source')])

## Only supporting 32 bit on Darwin to not deal w/ Leopard/Snow Leopard diffs
if 'Darwin' == Platform and not MkStatic:
   Is64 = False
   BaseEnv.Append(CCFLAGS = '-m32')
   BaseEnv.Append(LINKFLAGS='-m32')
   BaseEnv.Append(SHLINKFLAGS='-Wl,-undefined,dynamic_lookup')
   BaseEnv.Append(SHLINKFLAGS='-Wl,-install_name,%s/${TARGET.file}'%LibInstallDir)

if Debug:
    print "\n+++ SConstruct setup"
    print "+ %20s : '%s'" % ('WaliDir',WaliDir)
    print "+ %20s : '%s'" % ('LibInstallDir',LibInstallDir)
    for f in ['CC','CXX','CFLAGS','CCFLAGS','CXXFLAGS','CPPPATH','SHLINKFLAGS']:
        print "+ %20s : '%s'" % (f,BaseEnv[f])   

Export('Debug')
Export('WaliDir')
Export('LibInstallDir')
Export('BuildDir')
Export('MkStatic')
Export('BaseEnv')
Export('Platform')
Export('Is64')

## Setup a default environment for building executables that use WALi
ProgEnv = BaseEnv.Clone()
 
if MkStatic:
  ProgEnv['StaticLibs'] = [os.path.join(LibInstallDir,'libwali.a')]
else:
  ProgEnv['StaticLibs'] = []
  ProgEnv.Append(LIBS = ['wali'])
  ProgEnv.Append(LIBPATH =  LibInstallDir )

Export('ProgEnv')


## ####################################
## Calling of WALi SConscript files for
## building.
if 'help' not in COMMAND_LINE_TARGETS:
    ## ##################
    ## libwali
    built = SConscript('Source/SConscript', variant_dir=BuildDir,duplicate=0)
    #built += SConscript('Doc/tex/SConscript')

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

