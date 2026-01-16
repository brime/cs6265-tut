#!/usr/bin/env python2

import os

from pwn import *

SERVER = "52.201.10.159"

r = ssh("ubuntu", SERVER)
r.set_working_directory("/home/")

try:
    os.makedirs("tut")
except:
    True

tuts = r.run_to_end("sudo find . -name 'tut*' | grep -P 'tut[0-9][0-9]'")[0].split()

r.run("rm -rf /tmp/tuts")
r.run("mkdir -p /tmp/tuts")
for tut in tuts:
    toks = tut.split("/")
    assert(len(toks) == 3 and toks[0] == ".")
    lab = toks[1]
    tut = toks[2]

    r.run("sudo mkdir /tmp/tuts/%s" % lab)
    r.run("sudo cp -r '/home/%s/%s' /tmp/tuts/%s/" % (lab, tut, lab))

r.run("sudo chown -R ubuntu: /tmp/tuts/")
r.close()

os.system("scp -r ubuntu@%s:/tmp/tuts/ tuts" % SERVER)
