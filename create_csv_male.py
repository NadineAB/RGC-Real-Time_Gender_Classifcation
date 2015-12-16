#/usr/bin/env python

import sys
import os.path

# This is a tiny script to help you creating a CSV file from a face
# database with a similar hierarchie:

BASE_PATH="Train\example\male"
SEPARATOR=";"
label = 0
for dirname, dirnames, filenames in os.walk(BASE_PATH):
    for subdirname in dirnames:
        subject_path = os.path.join(dirname, subdirname)
        for filename in os.listdir(subject_path):
                abs_path = "%s\%s" % (subject_path, filename)
                print "%s%s%d" % (abs_path, SEPARATOR, label)
                #label = label + 1
