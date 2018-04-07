#!/usr/bin/env python3

# 2018 Matthias J. Kannwischer
# public domain
import subprocess
import os
import hashlib 
ctr = 0
for testfile in os.listdir("test/data/in"):
    with open("test/data/in/"+testfile) as f: 
        subprocess.call(["./hcc"], stdin=f, stdout=subprocess.DEVNULL)
        # check if flow model is as expected        
        with open("out", "rb") as f:
            out1 = hashlib.sha3_256(f.read()).hexdigest()
        with open("test/data/out/{}".format(testfile), "rb") as f:
            out2 = hashlib.sha3_256(f.read()).hexdigest()

        if out1 != out2:
            print("TEST FAILED. Unexpected flow model for /test/data/in/{}".format(testfile))
        # check if symbol table is as expected
        with open("out.tab", "rb") as f:
            out1 = hashlib.sha3_256(f.read()).hexdigest()
        with open("test/data/out/{}.tab".format(testfile), "rb") as f:
            out2 = hashlib.sha3_256(f.read()).hexdigest()
        
        if out1 != out2:
           print("TEST FAILED. Unexpected symbol table for /test/data/in/{}".format(testfile))
    ctr = ctr + 1


print("Succesfully checked {} files.".format(ctr))
