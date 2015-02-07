#!/usr/bin/perl -w
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
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.


#
# headers_check.pl execute a number of trivial consistency checks
#
# Usage: headers_check.pl dir [files...]
# dir:   dir to look for included files
# arch:  architecture
# files: list of files to check
#
# The script reads the supplied files line by line and:
#
# 1) for each include statement it checks if the
#    included file actually exists.
#    Only include files located in asm* and linux* are checked.
#    The rest are assumed to be system include files.
#
# 2) It is checked that prototypes does not use "extern"
#
# 3) Check for leaked CONFIG_ symbols

use strict;

my ($dir, $arch, @files) = @ARGV;

my $ret = 0;
my $line;
my $lineno = 0;
my $filename;

foreach my $file (@files) {
	local *FH;
	$filename = $file;
	open(FH, "<$filename") or die "$filename: $!\n";
	$lineno = 0;
	while ($line = <FH>) {
		$lineno++;
		&check_include();
		&check_asm_types();
		&check_sizetypes();
		&check_prototypes();
		&check_config();
	}
	close FH;
}
exit $ret;

sub check_include
{
	if ($line =~ m/^\s*#\s*include\s+<((asm|linux).*)>/) {
		my $inc = $1;
		my $found;
		$found = stat($dir . "/" . $inc);
		if (!$found) {
			$inc =~ s#asm/#asm-$arch/#;
			$found = stat($dir . "/" . $inc);
		}
		if (!$found) {
			printf STDERR "$filename:$lineno: included file '$inc' is not exported\n";
			$ret = 1;
		}
	}
}

sub check_prototypes
{
	if ($line =~ m/^\s*extern\b/) {
		printf STDERR "$filename:$lineno: extern's make no sense in userspace\n";
	}
}

sub check_config
{
	if ($line =~ m/[^a-zA-Z0-9_]+CONFIG_([a-zA-Z0-9]+)[^a-zA-Z0-9]/) {
		printf STDERR "$filename:$lineno: leaks CONFIG_$1 to userspace where it is not valid\n";
	}
}

my $linux_asm_types;
sub check_asm_types()
{
	if ($filename =~ /types.h|int-l64.h|int-ll64.h/o) {
		return;
	}
	if ($lineno == 1) {
		$linux_asm_types = 0;
	} elsif ($linux_asm_types >= 1) {
		return;
	}
	if ($line =~ m/^\s*#\s*include\s+<asm\/types.h>/) {
		$linux_asm_types = 1;
		printf STDERR "$filename:$lineno: " .
		"include of <linux/types.h> is preferred over <asm/types.h>\n"
		# Warn until headers are all fixed
		#$ret = 1;
	}
}

my $linux_types;
sub check_sizetypes
{
	if ($filename =~ /types.h|int-l64.h|int-ll64.h/o) {
		return;
	}
	if ($lineno == 1) {
		$linux_types = 0;
	} elsif ($linux_types >= 1) {
		return;
	}
	if ($line =~ m/^\s*#\s*include\s+<linux\/types.h>/) {
		$linux_types = 1;
		return;
	}
	if ($line =~ m/__[us](8|16|32|64)\b/) {
		printf STDERR "$filename:$lineno: " .
		              "found __[us]{8,16,32,64} type " .
		              "without #include <linux/types.h>\n";
		$linux_types = 2;
		# Warn until headers are all fixed
		#$ret = 1;
	}
}

