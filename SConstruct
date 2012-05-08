# Build WALi library 

## ####################################
## Third party users of WALi should 
## duplicate the setup code below.
## TODO: move setup code to an include file
import os, os.path, platform

Debug = True
#DefaultEnvironment(MSVC_USE_SCRIPT=False) # for Windows

#(platform_bits,linkage) = platform.architecture()
if platform.machine() == 'i686':
   platform_bits = 32
elif platform.machine() == 'x86_64':
   platform_bits = 64
elif platform.machine() == 'AMD64':
   platform_bits = 64
else:
    print "Warning: Check platform_bits for machine type", platform.machine()
    print "         Assuming 32"
    platform_bits = 32


Platform       = platform.system()
MkStatic       = (Platform == 'Windows')
WaliDir        = os.getcwd()
ThirdPartyDir     = os.path.join(WaliDir,'ThirdParty')
BaseEnv        = Environment() #MSVC_USE_SCRIPT=False)
Is64           = (platform_bits == 64)

ThirtyTwoBitAliases=['32', 'x86', 'ia_32', 'ia32']
SixtyFourBitAliases=['64', 'x64', 'x86_64', 'amd64']

if platform.system() == 'Windows':
   BaseEnv['ENV']['PATH'] += ';c:\\cygwin\\bin'

vars = Variables()
vars.Add(EnumVariable('arch', 'Architecture', 'default',
         allowed_values=ThirtyTwoBitAliases+SixtyFourBitAliases+['default']))
vars.Add(PathVariable('CXX', 'Path to compiler', BaseEnv['CXX'], PathVariable.PathAccept))
vars.Add(BoolVariable('strong_warnings', 'Enable (on by default) to get strong warning flags', True))
vars.Add(BoolVariable('optimize', 'Turn on optimization', True))
vars.Add(EnumVariable('checking', "Level of checking. 'slow' gives full checking, e.g. checked iterators. 'fast' gives only quick checks. 'none' removes all assertions. NOTE: On Windows, this also controls whether the library builds with /MTd (under 'slow') or /MT (under 'fast' and 'none').", None, allowed_values=('slow', 'fast', 'none')))

tempEnviron = Environment(tools=[], variables=vars)
arch = tempEnviron['arch']
BaseEnv['CXX'] = tempEnviron['CXX']
strong_warnings = tempEnviron['strong_warnings']
optimize = tempEnviron['optimize']
if 'checking' not in tempEnviron:
   if optimize:
      CheckedLevel = 'fast'
   else:
      CheckedLevel = 'slow'
else:
   CheckedLevel = tempEnviron['checking']
tempEnviron = None
vars = None

if arch in ThirtyTwoBitAliases:
    Is64 = False
elif arch in SixtyFourBitAliases:
    Is64 = True


def unquoting_WhereIs(possibly_relative_path_with_arguments):
    no_variables = BaseEnv.subst(possibly_relative_path_with_arguments)
    just_the_program = shlex.split(no_variables)[0]
    absolute_path = str(BaseEnv.WhereIs(just_the_program))
    return absolute_path

import shlex
def unquoting_requoting_WhereIs(possibly_relative_path_with_arguments):
    absolute_path = unquoting_WhereIs(possibly_relative_path_with_arguments)
    if ' ' in absolute_path:
       return '"%s"' % absolute_path
    else:
       return absolute_path

BaseEnv['CC']  = unquoting_requoting_WhereIs(BaseEnv['CC']) 
BaseEnv['CXX'] = unquoting_requoting_WhereIs(BaseEnv['CXX']) 


(dummy, BaseEnv['compiler']) = os.path.split(BaseEnv['CC'])
if BaseEnv['compiler'][0] == '"':
   BaseEnv['compiler'] = BaseEnv['compiler'][1:]
if BaseEnv['compiler'][-1] == '"':
   BaseEnv['compiler'] = BaseEnv['compiler'][0:-1]

if BaseEnv['compiler'] == 'cl.EXE':
   BaseEnv['compiler'] = 'cl'


if Is64:
    LibInstallDir  = os.path.join(WaliDir,'lib64')
    BinInstallDir  = os.path.join(WaliDir,'bin64')
    BuildDir       = os.path.join(WaliDir,'_build64')
else:
    LibInstallDir  = os.path.join(WaliDir,'lib')
    BinInstallDir  = os.path.join(WaliDir,'bin')
    BuildDir       = os.path.join(WaliDir,'_build')

BaseEnv['CMAKE'] = os.environ.get('CMAKE', 'cmake')

assert 'CPPDEFINES' not in BaseEnv
BaseEnv['CPPDEFINES'] = {}


if 'gcc' == BaseEnv['compiler']:
    # -Waddress -Wlogical-op

    # -Wcast-qual 
    BaseEnv.Append(CCFLAGS='-g -ggdb -Wall')
    if optimize:
       BaseEnv.Append(CCFLAGS=' -O2')
    if CheckedLevel == 'slow':
       BaseEnv['CPPDEFINES']['_GLIBCXX_DEBUG'] = 1
    if strong_warnings:
        BaseEnv.Append(CCFLAGS='-Wextra $WARNING_FLAGS -fdiagnostics-show-option')
        BaseEnv.Append(WARNING_FLAGS='-Werror -Wformat=2 -Winit-self -Wunused -Wfloat-equal -Wundef -Wpointer-arith -Wcast-align -Wwrite-strings -Wconversion -Woverloaded-virtual')
    else:
        BaseEnv.Append(WARNING_FLAGS='')

    if platform_bits == 64 and not Is64:
        # If we're on a 64-bit platform but want to compile for 32.
        # This will only happen if Is64 is changed from what it is
        # right now (Is64 = (platform_bits == 64))
        BaseEnv.Append(CCFLAGS='-m32')
        BaseEnv.Append(LINKFLAGS='-m32')
elif BaseEnv['compiler'] in ['cl', 'cl.EXE']:
    # Mostly copied from VS C++ 2005 Command line
    BaseEnv.Append(CCFLAGS='/errorReport:prompt /W4 /wd4512 /GR /EHsc /Zi')
    BaseEnv.Append(LINKFLAGS='/DEBUG')
    BaseEnv.Append(WARNING_FLAGS='')
    if optimize:
       BaseEnv.Append(CCFLAGS=' /O2')
    if CheckedLevel == 'slow':
       BaseEnv.Append(CCFLAGS=' /MTd')
    else:
       BaseEnv.Append(CCFLAGS=' /MT')
BaseEnv.Append(CPPPATH = [os.path.join(WaliDir , 'Source')])
BaseEnv.Append(CPPPATH = [os.path.join(WaliDir , '..', 'boost')])
try:
    BaseEnv.Append(CPPPATH = [os.environ['BOOST_HOME']])
except KeyError:
    pass

levels={'slow': 2, 'fast':1, 'none':0}
BaseEnv['CPPDEFINES']['CHECKED_LEVEL'] = levels[CheckedLevel]

if os.path.split(BaseEnv['CXX'])[1] == 'pathCC':
   BaseEnv.Append(LIBS=['gcc_s'])
   BaseEnv.Append(LIBPATH=['/s/gcc-4.6.1/lib64'])

if os.path.split(BaseEnv['CXX'])[1] == 'icpc':
   BaseEnv.Append(RPATH=['/s/intel_cc-11.0/lib/intel64'])

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
    print "+ %20s : '%s'" % ('optimize', optimize)
    print "+ %20s : '%s'" % ('CheckedLevel', CheckedLevel)


Export('Debug')
Export('WaliDir')
Export('LibInstallDir')
Export('BinInstallDir')
Export('BuildDir')
Export('ThirdPartyDir')
Export('MkStatic')
Export('BaseEnv')
Export('Platform')
Export('Is64')
Export('CheckedLevel')

## Setup a default environment for building executables that use WALi
ProgEnv = BaseEnv.Clone()
 
if MkStatic:
  ProgEnv['StaticLibs'] = [os.path.join(LibInstallDir,'libwali.a')]
else:
  ProgEnv['StaticLibs'] = []
  ProgEnv.Append(RPATH = LibInstallDir )
ProgEnv.Append(LIBS = ['wali'])
ProgEnv.Append(LIBPATH = LibInstallDir )

Export('ProgEnv')


## ####################################
## Calling of WALi SConscript files for
## building.
if 'help' not in COMMAND_LINE_TARGETS:
    ## ##################
    ## libwali
    built = SConscript('Source/SConscript', variant_dir=os.path.join(BuildDir,'lib'), duplicate=0)
    built += SConscript('Source/bin/SConscript', variant_dir=os.path.join(BuildDir,'bin'), duplicate=0)
    #built += SConscript('Doc/tex/SConscript')

    ## ##################
    ## All
    if 'all' in COMMAND_LINE_TARGETS:
        for d in ['AddOns','Examples']:
            built += SConscript('%s/SConscript' % d)
        built += SConscript('Tests/SConscript', variant_dir=os.path.join(BuildDir,'tests'), duplicate=0)
        unit_tests = SConscript('Tests/unit-tests/SConscript', variant_dir=os.path.join(BuildDir,'unit-tests'), duplicate=0)
        built += unit_tests
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
            built += SConscript('Tests/SConscript', variant_dir=os.path.join(BuildDir,'tests'), duplicate=0)
            unit_tests = SConscript('Tests/unit-tests/SConscript', variant_dir=os.path.join(BuildDir,'unit-tests'), duplicate=0)
            built += unit_tests
            BaseEnv.Alias('tests',built)
else:
    BaseEnv.Alias('help',[])
    print """
    scons [all addons examples tests]
    """

