#!/usr/bin/env python
# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.


#
# rt-mutex tester
#
# (C) 2006 Thomas Gleixner <tglx@linutronix.de>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
import os
import sys
import getopt
import shutil
import string

# Globals
quiet = 0
test = 0
comments = 0

sysfsprefix = "/sys/devices/system/rttest/rttest"
statusfile = "/status"
commandfile = "/command"

# Command opcodes
cmd_opcodes = {
    "schedother"    : "1",
    "schedfifo"     : "2",
    "lock"          : "3",
    "locknowait"    : "4",
    "lockint"       : "5",
    "lockintnowait" : "6",
    "lockcont"      : "7",
    "unlock"        : "8",
    "lockbkl"       : "9",
    "unlockbkl"     : "10",
    "signal"        : "11",
    "resetevent"    : "98",
    "reset"         : "99",
    }

test_opcodes = {
    "prioeq"        : ["P" , "eq" , None],
    "priolt"        : ["P" , "lt" , None],
    "priogt"        : ["P" , "gt" , None],
    "nprioeq"       : ["N" , "eq" , None],
    "npriolt"       : ["N" , "lt" , None],
    "npriogt"       : ["N" , "gt" , None],
    "unlocked"      : ["M" , "eq" , 0],
    "trylock"       : ["M" , "eq" , 1],
    "blocked"       : ["M" , "eq" , 2],
    "blockedwake"   : ["M" , "eq" , 3],
    "locked"        : ["M" , "eq" , 4],
    "opcodeeq"      : ["O" , "eq" , None],
    "opcodelt"      : ["O" , "lt" , None],
    "opcodegt"      : ["O" , "gt" , None],
    "eventeq"       : ["E" , "eq" , None],
    "eventlt"       : ["E" , "lt" , None],
    "eventgt"       : ["E" , "gt" , None],
    }

# Print usage information
def usage():
    print "rt-tester.py <-c -h -q -t> <testfile>"
    print " -c    display comments after first command"
    print " -h    help"
    print " -q    quiet mode"
    print " -t    test mode (syntax check)"
    print " testfile: read test specification from testfile"
    print " otherwise from stdin"
    return

# Print progress when not in quiet mode
def progress(str):
    if not quiet:
        print str

# Analyse a status value
def analyse(val, top, arg):

    intval = int(val)

    if top[0] == "M":
        intval = intval / (10 ** int(arg))
	intval = intval % 10
        argval = top[2]
    elif top[0] == "O":
        argval = int(cmd_opcodes.get(arg, arg))
    else:
        argval = int(arg)

    # progress("%d %s %d" %(intval, top[1], argval))

    if top[1] == "eq" and intval == argval:
	return 1
    if top[1] == "lt" and intval < argval:
        return 1
    if top[1] == "gt" and intval > argval:
	return 1
    return 0

# Parse the commandline
try:
    (options, arguments) = getopt.getopt(sys.argv[1:],'chqt')
except getopt.GetoptError, ex:
    usage()
    sys.exit(1)

# Parse commandline options
for option, value in options:
    if option == "-c":
        comments = 1
    elif option == "-q":
        quiet = 1
    elif option == "-t":
        test = 1
    elif option == '-h':
        usage()
        sys.exit(0)

# Select the input source
if arguments:
    try:
        fd = open(arguments[0])
    except Exception,ex:
        sys.stderr.write("File not found %s\n" %(arguments[0]))
        sys.exit(1)
else:
    fd = sys.stdin

linenr = 0

# Read the test patterns
while 1:

    linenr = linenr + 1
    line = fd.readline()
    if not len(line):
        break

    line = line.strip()
    parts = line.split(":")

    if not parts or len(parts) < 1:
        continue

    if len(parts[0]) == 0:
        continue

    if parts[0].startswith("#"):
	if comments > 1:
	    progress(line)
	continue

    if comments == 1:
	comments = 2

    progress(line)

    cmd = parts[0].strip().lower()
    opc = parts[1].strip().lower()
    tid = parts[2].strip()
    dat = parts[3].strip()

    try:
        # Test or wait for a status value
        if cmd == "t" or cmd == "w":
            testop = test_opcodes[opc]

            fname = "%s%s%s" %(sysfsprefix, tid, statusfile)
            if test:
		print fname
                continue

            while 1:
                query = 1
                fsta = open(fname, 'r')
                status = fsta.readline().strip()
                fsta.close()
                stat = status.split(",")
                for s in stat:
		    s = s.strip()
                    if s.startswith(testop[0]):
                        # Seperate status value
                        val = s[2:].strip()
                        query = analyse(val, testop, dat)
                        break
                if query or cmd == "t":
                    break

            progress("   " + status)

            if not query:
                sys.stderr.write("Test failed in line %d\n" %(linenr))
		sys.exit(1)

        # Issue a command to the tester
        elif cmd == "c":
            cmdnr = cmd_opcodes[opc]
            # Build command string and sys filename
            cmdstr = "%s:%s" %(cmdnr, dat)
            fname = "%s%s%s" %(sysfsprefix, tid, commandfile)
            if test:
		print fname
                continue
            fcmd = open(fname, 'w')
            fcmd.write(cmdstr)
            fcmd.close()

    except Exception,ex:
    	sys.stderr.write(str(ex))
        sys.stderr.write("\nSyntax error in line %d\n" %(linenr))
        if not test:
            fd.close()
            sys.exit(1)

# Normal exit pass
print "Pass"
sys.exit(0)


