#!/usr/bin/python3

import os
import shutil
import subprocess
import sys


expr = sys.argv[1]
name = sys.argv[2]
name_s = name + ".s"

cmd = ["./ntlang", "-e", expr, "-c", name]
p = subprocess.run(cmd, capture_output=True)
with open(name_s, "w") as f:
    f.write(p.stdout.decode("utf-8"))
p = subprocess.run(["gcc", "-o", name, name_s])
exe = ["./" + name] + sys.argv[3:]
p = subprocess.run(exe, capture_output=True)
print(p.stdout.decode("utf-8"), end="")
os.remove(name)
os.remove(name_s)
