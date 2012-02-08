WALi and OpenNWA
----------------

Prerequsites:
  - Boost  (old versions are OK; we actually variously use 1.33.1 and 1.42)
  - SCons
  - G++ or MSVC

Optional prerequsites for the 'scons tests' pseudotarget (or 'scons all'):
  - cmake
  - internet connection (will download Google Test automatically;
    no system-wide installation is done)

Optional prerequsites for Doxygen documentation:
  - Doxgen  (we use 1.7.1) 

Optional prerequsites for Latex documentation
  - Latex  (we use TexLive 2010, but it has also built with MikTex)
  - pygments
  - dot2tex  (optional even if you are building the documentation; we have
    2.8.7)


Building the Library
--------------------

To build, simply run 'scons'. By default, this will build the library (a
shared library with GCC and a static library for MSVC). Other possible
targets are:
  - addons    Build the contents of AddOns/, such as the XML WPDS parser

  - examples  Build the contents of Examples/

  - tests     Build the contents of Tests/, including the NWA unit tests.
              The NWA unit tests build to Tests/nwa/nwa-tests (or
              nwa-tests.exe) but are not automatically run.

              CMAKE either needs to be picked up by SCons by default (and it
              does NOT look at your $PATH) or set the $CMAKE environment
              variable to point to the executable.

              The NWA tests are actually reasonably complete, relative to the
              Latex documentation. TODO.txt mentions the omissions. Also
              mostly omitted are a number of functions that did not make it
              into the Latex documentation.

  - all       Build everything!

You can pass 'arch=x86' to SCons if you are on a 64-bit Linux machine to get
a 32-bit build. (The 32 and 64 bit builds live next to each other, so they
can coexist.) Note: because of the way 'cmake' is called for the Google
Test library, you probably won't be able to do this for the 'tests' or 'all'
pseudo-targets.

You can also pass 'CXX=...' to pick the particular compiler, e.g.  'scons
CXX=icc'. (If you are using compilers other than GCC or CL, you'll probably
also have to pass strong_warnings=0 to disable a bunch of -W flags that your
compiler probably doesn't understand.)

There is also a Visual Studio 2005 project, though the NWA unit tests aren't
hooked up for this at all.


WALi and OpenNWA are built to a single library, which will be copied into the
lib/ or lib64/ directory. Addons will be built to other shared libraries in
that directory.


Building Documentation
----------------------

To get Doxygen output, run 'doxygen Doxyfile' from the top-level
directory. By default, it will generate HTML output to Doc/doxygen/html and
Latex output to Doc/doxygen/latex. (Run 'make' in the latter directory to
build the Latex to a PDF.)


To build the NWA documentation, change to Doc/opennwa/ and run pdflatex on
opennwa.tex, passing '-shell-escape'. ('-shell-escape' is required by
the pygmentize package, used for source-highlighting the example code at the
beginning.) If you would like to rebuild the figures (though this is not
necessary if you do not change them), change to Doc/opennwa/Figures and run
'make'. Note that a couple of the figures used in the "official" version of
the NWA manual have been modified by hand, and regenerating them will cause
those changes to be lost.


NWA documentation
-----------------

The NWA documentation is a little bit... strange. It doesn't quite know what
it wants to be. It's way too long to just sit down and read, but is probably
less convenient to use as a reference than, say, Doxygen comments would
be. (The exception is the function tables at the end of the document.)
But... we also have Doxygen comments, and they aren't usually the same. So at
some point in the future, we need to unify the comments. (In virtually all
cases, the Latex documentation is far more detailed.)

My suggestion is to look over sections 1 ("Library Overview") and 3 ("The NWA
class"), and then refer to the more detailed sections as appropriate.



