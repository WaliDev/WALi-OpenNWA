#!/usr/bin/perl -w

use strict;
use File::Basename;

open MAKEFILE, "> Makefile" or die "Can't open Makefile : $!";

# Set PATH here b/c we fork make later
$ENV{'PATH'} = '/bin:/usr/bin:/usr/local/bin';

my $TOPDIR          = ".";
if( my $val = $ENV{'WALiDIR'} ) {
    $TOPDIR = $val;
}
else {
    print "ENV{'WALiDIR'} not defined. Defaulting to '.'\n";
}
my $BUILDDIR        = "$TOPDIR/build";
my $EDIR            = "$TOPDIR/Examples";
my $TDIR            = "$TOPDIR/Tests";
my $OBJDIR          = "$BUILDDIR/objects";
my $WALiDIR         = "$TOPDIR/wali";
my $WFADIR          = "$WALiDIR/wfa";
my $WPDSDIR         = "$WALiDIR/wpds";
my $EWPDSDIR        = "$WPDSDIR/ewpds";
my $OBJSFX          = "\$(OBJSFX)";
my $CXXSFX          = ".cpp";
my $CC              = "gcc";
my $CFLAGS          = "-g -Wall -Wformat=2 -W -fPIC";
my $DFLAGS          = "";
my $INCS            = "-I$TOPDIR";
my $LIBS            = "";
my $LIBWALi_A       = "libwali.a";
my $LIBWALi_SO      = "libwali\$(SOSFX)";
#Check for g++
my $CXX             = "/usr/bin/g++";
if( my $val = $ENV{'GCCDIR'} ) {
    $CXX = "$val/bin/g++";
}
else {
    print STDERR "ENV{'GCCDIR'} not defined. Defauling to /usr/bin/g++\n";
}

my @WALi_FILES = (
# namespace wali
    "$WALiDIR/Markable$CXXSFX"
    ,"$WALiDIR/Printable$CXXSFX"
    ,"$WALiDIR/SemElem$CXXSFX"
    ,"$WALiDIR/SemElemPair$CXXSFX"
    ,"$WALiDIR/Worklist$CXXSFX"
    ,"$WALiDIR/KeyFactory$CXXSFX"
    ,"$WALiDIR/IntSource$CXXSFX"
    ,"$WALiDIR/StringSource$CXXSFX"
    ,"$WALiDIR/KeyPairSource$CXXSFX"
    ,"$WALiDIR/Witness$CXXSFX"
    ,"$WALiDIR/WitnessExtend$CXXSFX"
    ,"$WALiDIR/WitnessCombine$CXXSFX"
    ,"$WALiDIR/Visitor$CXXSFX"
#namespace wali::wfa
    ,"$WFADIR/WFA$CXXSFX"
    ,"$WFADIR/State$CXXSFX"
    ,"$WFADIR/Trans$CXXSFX"
    ,"$WFADIR/TransFunctor$CXXSFX"
    ,"$WFADIR/WeightMaker$CXXSFX"
    ,"$WFADIR/WitnessTrans$CXXSFX"
#namespace wali::wpds
    ,"$WPDSDIR/Config$CXXSFX"
    ,"$WPDSDIR/Rule$CXXSFX"
    ,"$WPDSDIR/RuleFunctor$CXXSFX"
    ,"$WPDSDIR/LinkedTrans$CXXSFX"
    ,"$WPDSDIR/WPDS$CXXSFX"
    ,"$WPDSDIR/WitnessRule$CXXSFX"
    ,"$WPDSDIR/WitnessWPDS$CXXSFX"
    ,"$EWPDSDIR/EWPDS$CXXSFX"
    ,"$EWPDSDIR/ERule$CXXSFX"
    ,"$EWPDSDIR/MergeFunction$CXXSFX"
);

sub print_obj_files()
{
    foreach my $tmp (@WALi_FILES) {
        my ($name,$path,$suffix) = fileparse($tmp,$CXXSFX);
        print MAKEFILE " $OBJDIR/$name$OBJSFX";
    }
}

#
# Generate preprocessing
#
print MAKEFILE "UNAME := \$(shell which uname)\n";
print MAKEFILE "OS := \$(shell \$(UNAME) -s)\n";
print MAKEFILE "include Makefile.\$(OS)\n";
print MAKEFILE "\$(shell mkdir -p $OBJDIR)\n";

#
# Generate make commands
#
print MAKEFILE "\n.SILENT:\n\n";

#
# Generate all target defs
#
print MAKEFILE "all: $BUILDDIR/$LIBWALi_SO $BUILDDIR/$LIBWALi_A\n\n";

#
# Generate clean
#
print MAKEFILE "\n.PHONY: clean\n";
print MAKEFILE "clean:\n\t rm -rf $BUILDDIR/$LIBWALi_A $BUILDDIR/$LIBWALi_SO";
print_obj_files();
print MAKEFILE "\n\t\$(MAKE) -C $TDIR clean\n\n";

#
# Generate test cases
#
print MAKEFILE "\n.PHONY: t\n";
print MAKEFILE "t:\n\t\$(MAKE) all ; \$(MAKE) -C $TDIR clean all\n\n";

#
# Generate C++ source to object file defs
#
print MAKEFILE "\n";
foreach my $tmp (@WALi_FILES) {
    my ($name,$path,$suffix) = fileparse($tmp,$CXXSFX);
    print MAKEFILE "$OBJDIR/$name$OBJSFX: $tmp\n";
    print MAKEFILE "\t\@echo \"Compiling $name$suffix ...\"\n";
    print MAKEFILE "\t$CXX $CFLAGS $DFLAGS $INCS -o $OBJDIR/$name$OBJSFX -c $tmp\n\n";
}

#
# Generate libwpds++.a def
#
print MAKEFILE "\n";
print MAKEFILE "$BUILDDIR/$LIBWALi_A:";
print_obj_files();
print MAKEFILE "\n";
print MAKEFILE "\t\@echo \"Creating $BUILDDIR/$LIBWALi_A...\"\n";
print MAKEFILE "\tar rcs \$\@ \$\^";
print MAKEFILE "\n\n";

#
# Generate libwpds++.so def
#
print MAKEFILE "$BUILDDIR/$LIBWALi_SO:";
print_obj_files();
print MAKEFILE "\n";
print MAKEFILE "\t\@echo \"Creating $BUILDDIR/$LIBWALi_SO...\"\n";
print MAKEFILE "\t$CXX \$(LCFLAGS) -Wl,\$(LINKNAME),$LIBWALi_SO -o \$\@ \$\^";
print MAKEFILE "\n\n";

#system("/usr/bin/make clean") && die "system failed: $!";
system("/usr/bin/make -j2 all") && die "system failed: $!";
