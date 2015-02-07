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
# (C) Copyright IBM Corporation 2006.
#	Released under GPL v2.
#	Author : Ram Pai (linuxram@us.ibm.com)
#
# Usage: export_report.pl -k Module.symvers [-o report_file ] -f *.mod.c
#

use Getopt::Std;
use strict;

sub numerically {
	my $no1 = (split /\s+/, $a)[1];
	my $no2 = (split /\s+/, $b)[1];
	return $no1 <=> $no2;
}

sub alphabetically {
	my ($module1, $value1) = @{$a};
	my ($module2, $value2) = @{$b};
	return $value1 <=> $value2 || $module2 cmp $module1;
}

sub print_depends_on {
	my ($href) = @_;
	print "\n";
	while (my ($mod, $list) = each %$href) {
		print "\t$mod:\n";
		foreach my $sym (sort numerically @{$list}) {
			my ($symbol, $no) = split /\s+/, $sym;
			printf("\t\t%-25s\t%-25d\n", $symbol, $no);
		}
		print "\n";
	}
	print "\n";
	print "~"x80 , "\n";
}

sub usage {
        print "Usage: @_ -h -k Module.symvers  [ -o outputfile ] \n",
	      "\t-f: treat all the non-option argument as .mod.c files. ",
	      "Recommend using this as the last option\n",
	      "\t-h: print detailed help\n",
	      "\t-k: the path to Module.symvers file. By default uses ",
	      "the file from the current directory\n",
	      "\t-o outputfile: output the report to outputfile\n";
	exit 0;
}

sub collectcfiles {
        my @file = `cat .tmp_versions/*.mod | grep '.*\.ko\$'`;
        @file = grep {s/\.ko/.mod.c/} @file;
	chomp @file;
        return @file;
}

my (%SYMBOL, %MODULE, %opt, @allcfiles);

if (not getopts('hk:o:f',\%opt) or defined $opt{'h'}) {
        usage($0);
}

if (defined $opt{'f'}) {
	@allcfiles = @ARGV;
} else {
	@allcfiles = collectcfiles();
}

if (not defined $opt{'k'}) {
	$opt{'k'} = "Module.symvers";
}

unless (open(MODULE_SYMVERS, $opt{'k'})) {
	die "Sorry, cannot open $opt{'k'}: $!\n";
}

if (defined $opt{'o'}) {
	unless (open(OUTPUT_HANDLE, ">$opt{'o'}")) {
		die "Sorry, cannot open $opt{'o'} $!\n";
	}
	select OUTPUT_HANDLE;
}
#
# collect all the symbols and their attributes from the
# Module.symvers file
#
while ( <MODULE_SYMVERS> ) {
	chomp;
	my (undef, $symbol, $module, $gpl) = split;
	$SYMBOL { $symbol } =  [ $module , "0" , $symbol, $gpl];
}
close(MODULE_SYMVERS);

#
# collect the usage count of each symbol.
#
foreach my $thismod (@allcfiles) {
	unless (open(MODULE_MODULE, $thismod)) {
		print "Sorry, cannot open $thismod: $!\n";
		next;
	}
	my $state=0;
	while ( <MODULE_MODULE> ) {
		chomp;
		if ($state == 0) {
			$state = 1 if ($_ =~ /static const struct modversion_info/);
			next;
		}
		if ($state == 1) {
			$state = 2 if ($_ =~ /__attribute__\(\(section\("__versions"\)\)\)/);
			next;
		}
		if ($state == 2) {
			if ( $_ !~ /0x[0-9a-f]+,/ ) {
				next;
			}
			my $sym = (split /([,"])/,)[4];
			my ($module, $value, $symbol, $gpl) = @{$SYMBOL{$sym}};
			$SYMBOL{ $sym } =  [ $module, $value+1, $symbol, $gpl];
			push(@{$MODULE{$thismod}} , $sym);
		}
	}
	if ($state != 2) {
		print "WARNING:$thismod is not built with CONFIG_MODVERSION enabled\n";
	}
	close(MODULE_MODULE);
}

print "\tThis file reports the exported symbols usage patterns by in-tree\n",
	"\t\t\t\tmodules\n";
printf("%s\n\n\n","x"x80);
printf("\t\t\t\tINDEX\n\n\n");
printf("SECTION 1: Usage counts of all exported symbols\n");
printf("SECTION 2: List of modules and the exported symbols they use\n");
printf("%s\n\n\n","x"x80);
printf("SECTION 1:\tThe exported symbols and their usage count\n\n");
printf("%-25s\t%-25s\t%-5s\t%-25s\n", "Symbol", "Module", "Usage count",
	"export type");

#
# print the list of unused exported symbols
#
foreach my $list (sort alphabetically values(%SYMBOL)) {
	my ($module, $value, $symbol, $gpl) = @{$list};
	printf("%-25s\t%-25s\t%-10s\t", $symbol, $module, $value);
	if (defined $gpl) {
		printf("%-25s\n",$gpl);
	} else {
		printf("\n");
	}
}
printf("%s\n\n\n","x"x80);

printf("SECTION 2:\n\tThis section reports export-symbol-usage of in-kernel
modules. Each module lists the modules, and the symbols from that module that
it uses.  Each listed symbol reports the number of modules using it\n");

print "~"x80 , "\n";
while (my ($thismod, $list) = each %MODULE) {
	my %depends;
	$thismod =~ s/\.mod\.c/.ko/;
	print "\t\t\t$thismod\n";
	foreach my $symbol (@{$list}) {
		my ($module, $value, undef, $gpl) = @{$SYMBOL{$symbol}};
		push (@{$depends{"$module"}}, "$symbol $value");
	}
	print_depends_on(\%depends);
}
