
import subprocess
import sys
import os
import getopt
import math
import datetime

import grade



def usage(fname):

    ustring = "Usage: %s [-h]" % fname
    print ustring
    print "(All lists given as colon-separated text.)"
    print "    -h            Print this message"
    sys.exit(0)

def outmsg(s, noreturn = False):
    if len(s) > 0 and s[-1] != '\n' and not noreturn:
        s += "\n"
    sys.stdout.write(s)
    sys.stdout.flush()
    if outFile is not None:
        outFile.write(s)

def run(name, args):
    global outFile, sleepSeconds
    optString = "hms"
    optlist, args = getopt.getopt(args, optString)
    otherArgs = []



    for (opt, val) in optlist:
        if opt == '-h':
            usage(name)

    stdoutFileNumber = 1
    gmcd = "./encode"
    gmcd2 = "./decode"

    try:
        tstart = datetime.datetime.now()
        simulate = subprocess.Popen(gmcd, stderr = stdoutFileNumber)
        simulate.wait()
        returnInfo = simulate.returncode
    except Exception as e:
        print "Execution of command '%s' failed. %s" % (gcmdLine, e)
        return False
    if retcode == 0:
        delta = datetime.datetime.now() - tstart
        secs = delta.seconds + 24 * 3600 * delta.days + 1e-6 * delta.microseconds
        outmsg(secs)



if __name__ == "__main__":
    run(sys.argv[0], sys.argv[1:])
