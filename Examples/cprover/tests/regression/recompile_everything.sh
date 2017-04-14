#!/bin/bash

# Jason found that this line was required to get nightly_script.sh to run from 
# (I just stole it from ~/.profile)
source /bat0/stac/.opam/opam-init/init.sh > /dev/null 2> /dev/null || true

shopt -s nullglob

# Main ICRA installation (Ark2)
NROOT=/bat0/stac/Code/Ark2-Sandbox
NEWTON=$NROOT/WALi-OpenNWA
LIBTSLRE=$NEWTON/Examples/cprover/libtslre
DUET=$NROOT/duet
SUITE=$NEWTON/Examples/cprover/tests/regression

# "Java" ICRA installation
JROOT=/bat0/stac/Code/WALi-Sandbox
JNEWTON=$JROOT/WALi-OpenNWA
JLIBTSLRE=$JNEWTON/Examples/cprover/libtslre
JDUET=$JROOT/duet
JSUITE=$JNEWTON/Examples/cprover/tests/regression

# Updating the repos
echo "################## PULL DUET (MAIN) ###################"
cd $DUET
/usr/bin/git pull
echo "################## PULL ICRA (MAIN) ###################"
#cd $LIBTSLRE
#/usr/bin/git pull
cd $NEWTON
/usr/bin/git pull
echo "################## RECOMPILE (MAIN) ###################"
./full_icra_rebuild.sh

echo "################## PULL DUET (JAVA) ###################"
cd $JDUET
/usr/bin/git pull
echo "################## PULL ICRA (JAVA) ###################"
#cd $JLIBTSLRE
#/usr/bin/git pull
cd $JNEWTON
/usr/bin/git pull
echo "################## RECOMPILE (JAVA) ###################"
./full_icra_rebuild.sh

