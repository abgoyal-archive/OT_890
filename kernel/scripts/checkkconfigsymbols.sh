#!/bin/sh
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


# Find Kconfig variables used in source code but never defined in Kconfig
# Copyright (C) 2007, Paolo 'Blaisorblade' Giarrusso <blaisorblade@yahoo.it>

# Tested with dash.
paths="$@"
[ -z "$paths" ] && paths=.

# Doing this once at the beginning saves a lot of time, on a cache-hot tree.
Kconfigs="`find . -name 'Kconfig' -o -name 'Kconfig*[^~]'`"

echo -e "File list \tundefined symbol used"
find $paths -name '*.[chS]' -o -name 'Makefile' -o -name 'Makefile*[^~]'| while read i
do
	# Output the bare Kconfig variable and the filename; the _MODULE part at
	# the end is not removed here (would need perl an not-hungry regexp for that).
	sed -ne 's!^.*\<\(UML_\)\?CONFIG_\([0-9A-Z_]\+\).*!\2 '$i'!p' < $i
done | \
# Smart "sort|uniq" implemented in awk and tuned to collect the names of all
# files which use a given symbol
awk '{map[$1, count[$1]++] = $2; }
END {
	for (combIdx in map) {
		split(combIdx, separate, SUBSEP);
		# The value may have been removed.
		if (! ( (separate[1], separate[2]) in map ) )
			continue;
		symb=separate[1];
		printf "%s ", symb;
		#Use gawk extension to delete the names vector
		delete names;
		#Portably delete the names vector
		#split("", names);
		for (i=0; i < count[symb]; i++) {
			names[map[symb, i]] = 1;
			# Unfortunately, we may still encounter symb, i in the
			# outside iteration.
			delete map[symb, i];
		}
		i=0;
		for (name in names) {
			if (i > 0)
				printf ", %s", name;
			else
				printf "%s", name;
			i++;
		}
		printf "\n";
	}
}' |
while read symb files; do
	# Remove the _MODULE suffix when checking the variable name. This should
	# be done only on tristate symbols, actually, but Kconfig parsing is
	# beyond the purpose of this script.
	symb_bare=`echo $symb | sed -e 's/_MODULE//'`
	if ! grep -q "\<$symb_bare\>" $Kconfigs; then
		echo -e "$files: \t$symb"
	fi
done|sort
