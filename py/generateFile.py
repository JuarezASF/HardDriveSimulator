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

base_filename = input[1]

filesize = int(input[2])

filename = base_filename + str(filesize) + '.b'

random.seed(time.time())

with open(filename, 'wb') as fp:
    fp.seek(0)

    data = list()
    for i in xrange(filesize):
        data.append(random.randint(0,255))
        if len(data) > 1024:
            fp.write(bytearray(data))
            data = list()
    # write ramaining data
    fp.write(bytearray(data))
    data = list()

hash = md5sum(filename)

new_filename = "{}{:04}_{}.b".format(base_filename , filesize , hash)

os.rename(filename, new_filename)
