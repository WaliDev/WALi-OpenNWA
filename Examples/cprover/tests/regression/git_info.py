import subprocess, os

def getMostRecentCommitHash(path) :
    cwd = os.getcwd()
    try :
        os.chdir(path)
        hashcode = subprocess.check_output(["git","rev-parse","HEAD"]).strip()
    except :
        hashcode = ""
    os.chdir(cwd)
    return hashcode

def getMostRecentCommitDate(path) :
    "Get the most recent commit date"
    cwd = os.getcwd()
    try :
        os.chdir(path)
        date = subprocess.check_output(["git","show","-s","--format=%ci"]).strip()
    except :
        date = ""
    os.chdir(cwd)
    return date

