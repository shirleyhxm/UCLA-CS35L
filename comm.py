#!/usr/bin/python

"""
comm - Select or reject lines common to two files.

Options:
    -1: Suppress the output column of lines unique to file1.
    -2: Suppress the output column of lines unique to file2.
    -3: Suppress the output column of lines duplicated in file1 and file2.
    -u: Files not sorted.

"""

import random, sys
from optparse import OptionParser

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [-123u]... FILE1 FILE2

"""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-1",
                      action="store_true", dest="first", default=0,
                      help="Suppress the output column of lines unique to file1")
    parser.add_option("-2",
                      action="store_true", dest="second", default=0,
                      help="Suppress the output column of lines unique to file2")
    parser.add_option("-3",
                      action="store_true", dest="third", default=0,
                      help="Suppress the output column of lines duplicated in file1 and file2")
    parser.add_option("-u",
                      action="store_true", dest="unsort", default=0,
                      help="Inputs are not sorted")

    options, args = parser.parse_args(sys.argv[1:])

    #try:
    #    numlines = int(options.numlines)
    #except:
    #    parser.error("invalid NUMLINES: {0}".
    #                 format(options.numlines))

    if len(args) != 2:
        parser.error("wrong number of operands")
    if args[0] == "-" and args[1] == "-":
        parser.error("File1 and file2 cannot both refer to standard input.")
    elif args[0] == "-":
        file1 = open(sys.stdin, 'r')
        file2 = open(args[1], 'r')
    elif args[1] == "-":
        file1 = open(args[0],'r')
        file2 = open(sys.stdin, 'r')
    else:
        file1 = open(args[0],'r')
        file2 = open(args[1],'r')

    
    lines1 = file1.readlines()
    lines2 = file2.readlines()
    file1.close()
    file2.close()
            
    duplicates = []
    results = ""

    if options.unsort == 1:
        dup = 0
        for i in lines1:
            for j in lines2:
                if i == j:
                    if options.third == 0:
                        if options.first == 0 or options.second == 0:
                            if options.first == options.second:
                                results+="\t\t"
                            else:
                                results+="\t"
                        results+=i
                    dup = 1
                    duplicates.append(i)
                    break
            if dup == 0 and options.first == 0:
                results+=i
            dup = 0
        if options.second == 0:
            dup = 0
            for j in lines2:
                for k in duplicates:
                    if j == k:
                        dup = 1
                        break
                if dup == 0:
                    if options.second == 0:
                        if options.first == 0:
                            results+="\t"
                        results+=j
                dup = 0
    else:
        i = 0
        j = 0
        while i < len(lines1) and j < len(lines2):
            if lines1[i] < lines2[j]:
                if options.first == 0:
                    results+=lines1[i]
                i+=1
            elif lines1[i] > lines2[j]:
                if options.second == 0:
                    if options.first == 0:
                        results+="\t"
                    results+=lines2[j]
                j+=1
            else:               
                if options.third == 0:
                    if options.first == 0 or options.second == 0:
                        if options.first == options.second:
                            results+="\t\t"
                        else:
                            results+="\t"
                    results+=lines1[i]
                i+=1
                j+=1
        while i < len(lines1):
            if options.first == 0:
                results+=lines1[i]
            i+=1
        while j < len(lines2):
            if options.second == 0:
                if options.first == 0:
                    results+="\t"
                results+=lines2[j]
            j+=1
    sys.stdout.write(results)            
        




if __name__ == "__main__":
    main()
