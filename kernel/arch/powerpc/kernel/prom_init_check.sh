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
# Copyright © 2008 IBM Corporation
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version
# 2 of the License, or (at your option) any later version.

# This script checks prom_init.o to see what external symbols it
# is using, if it finds symbols not in the whitelist it returns
# an error. The point of this is to discourage people from
# intentionally or accidentally adding new code to prom_init.c
# which has side effects on other parts of the kernel.

# If you really need to reference something from prom_init.o add
# it to the list below:

WHITELIST="add_reloc_offset __bss_start __bss_stop copy_and_flush
_end enter_prom memcpy memset reloc_offset __secondary_hold
__secondary_hold_acknowledge __secondary_hold_spinloop __start
strcmp strcpy strlcpy strlen strncmp strstr logo_linux_clut224
reloc_got2 kernstart_addr memstart_addr"

NM="$1"
OBJ="$2"

ERROR=0

for UNDEF in $($NM -u $OBJ | awk '{print $2}')
do
	# On 64-bit nm gives us the function descriptors, which have
	# a leading . on the name, so strip it off here.
	UNDEF="${UNDEF#.}"

	if [ $KBUILD_VERBOSE ]; then
		if [ $KBUILD_VERBOSE -ne 0 ]; then
			echo "Checking prom_init.o symbol '$UNDEF'"
		fi
	fi

	OK=0
	for WHITE in $WHITELIST
	do
		if [ "$UNDEF" = "$WHITE" ]; then
			OK=1
			break
		fi
	done

	# ignore register save/restore funcitons
	if [ "${UNDEF:0:9}" = "_restgpr_" ]; then
		OK=1
	fi
	if [ "${UNDEF:0:11}" = "_rest32gpr_" ]; then
		OK=1
	fi
	if [ "${UNDEF:0:9}" = "_savegpr_" ]; then
		OK=1
	fi
	if [ "${UNDEF:0:11}" = "_save32gpr_" ]; then
		OK=1
	fi

	if [ $OK -eq 0 ]; then
		ERROR=1
		echo "Error: External symbol '$UNDEF' referenced" \
		     "from prom_init.c" >&2
	fi
done

exit $ERROR
