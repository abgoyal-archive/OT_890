#! /usr/bin/perl
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
# checkversion find uses of LINUX_VERSION_CODE or KERNEL_VERSION
# without including <linux/version.h>, or cases of
# including <linux/version.h> that don't need it.
# Copyright (C) 2003, Randy Dunlap <rdunlap@xenotime.net>

$| = 1;

my $debugging = 0;

foreach $file (@ARGV)
{
    # Open this file.
    open(FILE, $file) || die "Can't open $file: $!\n";

    # Initialize variables.
    my $fInComment   = 0;
    my $fInString    = 0;
    my $fUseVersion   = 0;
    my $iLinuxVersion = 0;

    LINE: while ( <FILE> )
    {
	# Strip comments.
	$fInComment && (s+^.*?\*/+ +o ? ($fInComment = 0) : next);
	m+/\*+o && (s+/\*.*?\*/+ +go, (s+/\*.*$+ +o && ($fInComment = 1)));

	# Pick up definitions.
	if ( m/^\s*#/o ) {
	    $iLinuxVersion      = $. if m/^\s*#\s*include\s*"linux\/version\.h"/o;
	}

	# Strip strings.
	$fInString && (s+^.*?"+ +o ? ($fInString = 0) : next);
	m+"+o && (s+".*?"+ +go, (s+".*$+ +o && ($fInString = 1)));

	# Pick up definitions.
	if ( m/^\s*#/o ) {
	    $iLinuxVersion      = $. if m/^\s*#\s*include\s*<linux\/version\.h>/o;
	}

	# Look for uses: LINUX_VERSION_CODE, KERNEL_VERSION, UTS_RELEASE
	if (($_ =~ /LINUX_VERSION_CODE/) || ($_ =~ /\WKERNEL_VERSION/)) {
	    $fUseVersion = 1;
	    last LINE if $iLinuxVersion;
	}
    }

    # Report used version IDs without include?
    if ($fUseVersion && ! $iLinuxVersion) {
	print "$file: $.: need linux/version.h\n";
    }

    # Report superfluous includes.
    if ($iLinuxVersion && ! $fUseVersion) {
	print "$file: $iLinuxVersion linux/version.h not needed.\n";
    }

    # debug: report OK results:
    if ($debugging) {
        if ($iLinuxVersion && $fUseVersion) {
	    print "$file: version use is OK ($iLinuxVersion)\n";
        }
        if (! $iLinuxVersion && ! $fUseVersion) {
	    print "$file: version use is OK (none)\n";
        }
    }

    close(FILE);
}
