#!/usr/bin/env python2

import time
import os
import pty

(pid, fd) = pty.fork()
if pid == 0:
    os.execle("./target", "./target", os.environ)
    exit(0)
else:
    print("pid = %s" % pid)
    os.write(fd, "hello")
