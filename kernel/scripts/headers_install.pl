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
# headers_install prepare the listed header files for use in
# user space and copy the files to their destination.
#
# Usage: headers_install.pl readdir installdir arch [files...]
# readdir:    dir to open files
# installdir: dir to install the files
# arch:       current architecture
#             arch is used to force a reinstallation when the arch
#             changes because kbuild then detect a command line change.
# files:      list of files to check
#
# Step in preparation for users space:
# 1) Drop all use of compiler.h definitions
# 2) Drop include of compiler.h
# 3) Drop all sections defined out by __KERNEL__ (using unifdef)

use strict;

my ($readdir, $installdir, $arch, @files) = @ARGV;

my $unifdef = "scripts/unifdef -U__KERNEL__";

foreach my $file (@files) {
	local *INFILE;
	local *OUTFILE;
	my $tmpfile = "$installdir/$file.tmp";
	open(INFILE, "<$readdir/$file")
		or die "$readdir/$file: $!\n";
	open(OUTFILE, ">$tmpfile") or die "$tmpfile: $!\n";
	while (my $line = <INFILE>) {
		$line =~ s/([\s(])__user\s/$1/g;
		$line =~ s/([\s(])__force\s/$1/g;
		$line =~ s/([\s(])__iomem\s/$1/g;
		$line =~ s/\s__attribute_const__\s/ /g;
		$line =~ s/\s__attribute_const__$//g;
		$line =~ s/^#include <linux\/compiler.h>//;
		$line =~ s/(^|\s)(inline)\b/$1__$2__/g;
		$line =~ s/(^|\s)(asm)\b(\s|[(]|$)/$1__$2__$3/g;
		$line =~ s/(^|\s|[(])(volatile)\b(\s|[(]|$)/$1__$2__$3/g;
		printf OUTFILE "%s", $line;
	}
	close OUTFILE;
	close INFILE;
	system $unifdef . " $tmpfile > $installdir/$file";
	unlink $tmpfile;
}
exit 0;
