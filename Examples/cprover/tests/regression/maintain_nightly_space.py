#!/usr/bin/python

import os.path, subprocess

ROOT = "/p/stac/public/html"
KEEP_MOST_RECENT = 60
ARCHIVE_TEMPLATE = "/p/stac/private/old_ICRA_nightly_outputs/old_%s.tar.xz" # date goes in for the %s

# 1. Read http://www.stat.wisc.edu/sites/default/files/long_running_jobs_instructions_0.pdf
# 2. Run "/s/std/bin/stashticket" once to stash your AFS ticket
# 3. Then run crontab -e and add a line like this:
# 01 23 * * * /s/std/bin/runauth -a -l jbreck /afs/cs.wisc.edu/u/j/b/jbreck/research/2013/STAC/newton_sandbox/space_maintainer/maintain_nightly_space.py
# (note: the above line begins with "20" not with "#")
# (note: replace "jbreck" with your username and the path after it with the path to this python script)

allSubdirs = list()

print "Cleaning up nightly testing server disk space"

for subdir in os.listdir(ROOT) :
    if len(subdir) != 8 : continue # want things like "20161231"
    fullPath = os.path.join(ROOT, subdir)
    if not os.path.isdir(fullPath) : continue
    if not "outputs" in os.listdir(fullPath) : continue
    allSubdirs.append(subdir)

enumeratedSubdirs = enumerate(sorted(allSubdirs,reverse=True))

for i, subdir in sorted(enumeratedSubdirs, reverse=True) :
    #print i, "->", subdir
    if i < KEEP_MOST_RECENT : continue
    fullPath = os.path.join(ROOT, subdir)
    os.chdir(fullPath)
    command = ["tar", "cJf", ARCHIVE_TEMPLATE % subdir, "outputs", "--remove-files"]
    #print command
    print "Archiving outputs from: " + subdir
    subprocess.call(command)
    #print "Archiving at most one night's outputs..."; break

print "Done cleaning up nightly testing server disk space"
