#!/usr/bin/perl
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



# Copyright 2008, Intel Corporation
#
# This file is part of the Linux kernel
#
# This program file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program in a file named COPYING; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA
#
# Authors:
# 	Arjan van de Ven <arjan@linux.intel.com>


#
# This script turns a cstate ftrace output into a SVG graphic that shows
# historic C-state information
#
#
# 	cat /sys/kernel/debug/tracing/trace | perl power.pl > out.svg
#

my @styles;
my $base = 0;

my @pstate_last;
my @pstate_level;

$styles[0] = "fill:rgb(0,0,255);fill-opacity:0.5;stroke-width:1;stroke:rgb(0,0,0)";
$styles[1] = "fill:rgb(0,255,0);fill-opacity:0.5;stroke-width:1;stroke:rgb(0,0,0)";
$styles[2] = "fill:rgb(255,0,20);fill-opacity:0.5;stroke-width:1;stroke:rgb(0,0,0)";
$styles[3] = "fill:rgb(255,255,20);fill-opacity:0.5;stroke-width:1;stroke:rgb(0,0,0)";
$styles[4] = "fill:rgb(255,0,255);fill-opacity:0.5;stroke-width:1;stroke:rgb(0,0,0)";
$styles[5] = "fill:rgb(0,255,255);fill-opacity:0.5;stroke-width:1;stroke:rgb(0,0,0)";
$styles[6] = "fill:rgb(0,128,255);fill-opacity:0.5;stroke-width:1;stroke:rgb(0,0,0)";
$styles[7] = "fill:rgb(0,255,128);fill-opacity:0.5;stroke-width:1;stroke:rgb(0,0,0)";
$styles[8] = "fill:rgb(0,25,20);fill-opacity:0.5;stroke-width:1;stroke:rgb(0,0,0)";


print "<?xml version=\"1.0\" standalone=\"no\"?> \n";
print "<svg width=\"10000\" height=\"100%\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n";

my $scale = 30000.0;
while (<>) {
	my $line = $_;
	if ($line =~ /([0-9\.]+)\] CSTATE: Going to C([0-9]) on cpu ([0-9]+) for ([0-9\.]+)/) {
		if ($base == 0) {
			$base = $1;
		}
		my $time = $1 - $base;
		$time = $time * $scale;
		my $C = $2;
		my $cpu = $3;
		my $y = 400 * $cpu;
		my $duration = $4 * $scale;
		my $msec = int($4 * 100000)/100.0;
		my $height = $C * 20;
		$style = $styles[$C];

		$y = $y + 140 - $height;

		$x2 = $time + 4;
		$y2 = $y + 4;


		print "<rect x=\"$time\" width=\"$duration\" y=\"$y\" height=\"$height\" style=\"$style\"/>\n";
		print "<text transform=\"translate($x2,$y2) rotate(90)\">C$C $msec</text>\n";
	}
	if ($line =~ /([0-9\.]+)\] PSTATE: Going to P([0-9]) on cpu ([0-9]+)/) {
		my $time = $1 - $base;
		my $state = $2;
		my $cpu = $3;

		if (defined($pstate_last[$cpu])) {
			my $from = $pstate_last[$cpu];
			my $oldstate = $pstate_state[$cpu];
			my $duration = ($time-$from) * $scale;

			$from = $from * $scale;
			my $to = $from + $duration;
			my $height = 140 - ($oldstate * (140/8));

			my $y = 400 * $cpu + 200 + $height;
			my $y2 = $y+4;
			my $style = $styles[8];

			print "<rect x=\"$from\" y=\"$y\" width=\"$duration\" height=\"5\" style=\"$style\"/>\n";
			print "<text transform=\"translate($from,$y2)\">P$oldstate (cpu $cpu)</text>\n";
		};

		$pstate_last[$cpu] = $time;
		$pstate_state[$cpu] = $state;
	}
}


print "</svg>\n";
