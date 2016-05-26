#!/usr/bin/python

import sys
import random
import time
import os


from hashlib import md5


def md5sum(filename):
    hash = md5()
    with open(filename, "rb") as f:
        for chunk in iter(lambda: f.read(128 * hash.block_size), b""):
            hash.update(chunk)
    return hash.hexdigest()

input = sys.argv

filename = input[1]

hash = md5sum(filename)

hash_on_filename = filename[filename.find('_')+1 : filename.rfind('.')]

if hash == hash_on_filename:
    print 'OK'
else:
    print 'FAILED'
    print 'expected', hash
    print 'got', hash_on_filename


