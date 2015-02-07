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
# config3270 -- Autoconfigure /dev/3270/* and /etc/inittab
#
#       Usage:
#               config3270
#
#       Output:
#               /tmp/mkdev3270
#
#       Operation:
#               1. Run this script
#               2. Run the script it produces: /tmp/mkdev3270
#               3. Issue "telinit q" or reboot, as appropriate.
#
P=/proc/tty/driver/tty3270
ROOT=
D=$ROOT/dev
SUBD=3270
TTY=$SUBD/tty
TUB=$SUBD/tub
SCR=$ROOT/tmp/mkdev3270
SCRTMP=$SCR.a
GETTYLINE=:2345:respawn:/sbin/mingetty
INITTAB=$ROOT/etc/inittab
NINITTAB=$ROOT/etc/NEWinittab
OINITTAB=$ROOT/etc/OLDinittab
ADDNOTE=\\"# Additional mingettys for the 3270/tty* driver, tub3270 ---\\"

if ! ls $P > /dev/null 2>&1; then
	modprobe tub3270 > /dev/null 2>&1
fi
ls $P > /dev/null 2>&1 || exit 1

# Initialize two files, one for /dev/3270 commands and one
# to replace the /etc/inittab file (old one saved in OLDinittab)
echo "#!/bin/sh" > $SCR || exit 1
echo " " >> $SCR
echo "# Script built by /sbin/config3270" >> $SCR
if [ ! -d /dev/dasd ]; then
	echo rm -rf "$D/$SUBD/*" >> $SCR
fi
echo "grep -v $TTY $INITTAB > $NINITTAB" > $SCRTMP || exit 1
echo "echo $ADDNOTE >> $NINITTAB" >> $SCRTMP
if [ ! -d /dev/dasd ]; then
	echo mkdir -p $D/$SUBD >> $SCR
fi

# Now query the tub3270 driver for 3270 device information
# and add appropriate mknod and mingetty lines to our files
echo what=config > $P
while read devno maj min;do
	if [ $min = 0 ]; then
		fsmaj=$maj
		if [ ! -d /dev/dasd ]; then
			echo mknod $D/$TUB c $fsmaj 0 >> $SCR
			echo chmod 666 $D/$TUB >> $SCR
		fi
	elif [ $maj = CONSOLE ]; then
		if [ ! -d /dev/dasd ]; then
			echo mknod $D/$TUB$devno c $fsmaj $min >> $SCR
		fi
	else
		if [ ! -d /dev/dasd ]; then
			echo mknod $D/$TTY$devno c $maj $min >>$SCR
			echo mknod $D/$TUB$devno c $fsmaj $min >> $SCR
		fi
		echo "echo t$min$GETTYLINE $TTY$devno >> $NINITTAB" >> $SCRTMP
	fi
done < $P

echo mv $INITTAB $OINITTAB >> $SCRTMP || exit 1
echo mv $NINITTAB $INITTAB >> $SCRTMP
cat $SCRTMP >> $SCR
rm $SCRTMP
exit 0
