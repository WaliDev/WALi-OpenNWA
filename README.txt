Prerequsites:
  - Boost  (old versions are OK; we actually use 1.33.1)
  - SCons
  - G++ or MSVC
  - cmake (only for 'scons tests' and 'scons all')

Optional prerequsites (for documentation)
  - Doxgen  (we use 1.7.1)
  - Latex  (we use TexLive 2010, but it has also built with MikTex)
  - dot2tex  (optional even if you are building the documentation; we have 2.8.7)


To build, simply run 'scons'. By default, this will build the library (a
shared library with GCC and a static library for MSVC). Other possible
targets are:
  - addons    Build the contents of AddOns/, such as the XML WPDS parser
  - examples  Build the contents of Examples/
  - tests     Build the contents of Tests/, including the NWA unit tests.
              (The NWA unit tests build to Tests/nwa/nwa-tests (or
              nwa-tests.exe) but are not automatically run.) CMAKE either
              needs to be picked up by SCons by default (and it does NOT
              pick up your $PATH) or set the $CMAKE environment variable to
              point to the executable.
  - all       Build everything!


To get Doxygen output, run 'doxygen Doxyfile' from the top-level
directory. By default, it will generate HTML output to Doc/doxygen/html and
Latex output to Doc/doxygen/latex. (Run 'make' in the latter directory to
build the Latex to a PDF.)


To build the NWA documentation, change to Doc/NWA_tex/ and run (pdf)latex on
WALi_NWA_Doc.tex. If you would like to rebuild the figures (though this is
not necessary if you do not change them), change to Doc/NWA_tex/Figures and
run 'make'. Note that a couple of the figures used in the "official" version
of the NWA manual have been modified by hand, and regenerating them will
cause those changes to be lost.

