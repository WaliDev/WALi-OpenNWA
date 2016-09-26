 #!/usr/bin/env python

from distutils.core import setup, Extension
import os

lib64_dir = os.path.normpath(os.path.join(os.getcwd(), '..', 'lib64'))

wali_c_module = Extension(
    '_wali',
    sources=[
        #'wali_wrap.cc'
        'wali.i',
    ],
    swig_opts = [
        '-c++',
    ],
    include_dirs = [
        '../Source',
    ],
    libraries = [
        'wali',
    ],
    library_dirs = [
        '../lib64',
    ],
    extra_link_args = [
        '-Wl,-rpath=' + lib64_dir,
    ],
)

setup(
    name = 'wali',
    version = '0.1',
    maintainer = 'Evan Driscoll',
    maintainer_email = 'driscoll@cs.wisc.edu',
    description = 'WALi -- Weighted Automaton Library',
    ext_modules = [wali_c_module],
    py_modules = ['wail'],
)
