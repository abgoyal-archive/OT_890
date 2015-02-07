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


if [ `id -u` -ne 0 ]; then
	echo "$0: must be root to install the selinux policy"
	exit 1
fi
SF=`which setfiles`
if [ $? -eq 1 ]; then
	if [ -f /sbin/setfiles ]; then
		SF="/usr/setfiles"
	else
		echo "no selinux tools installed: setfiles"
		exit 1
	fi
fi

cd mdp

CP=`which checkpolicy`
VERS=`$CP -V | awk '{print $1}'`

./mdp policy.conf file_contexts
$CP -o policy.$VERS policy.conf

mkdir -p /etc/selinux/dummy/policy
mkdir -p /etc/selinux/dummy/contexts/files

cp file_contexts /etc/selinux/dummy/contexts/files
cp dbus_contexts /etc/selinux/dummy/contexts
cp policy.$VERS /etc/selinux/dummy/policy
FC_FILE=/etc/selinux/dummy/contexts/files/file_contexts

if [ ! -d /etc/selinux ]; then
	mkdir -p /etc/selinux
fi
if [ ! -f /etc/selinux/config ]; then
	cat > /etc/selinux/config << EOF
SELINUX=enforcing
SELINUXTYPE=dummy
EOF
else
	TYPE=`cat /etc/selinux/config | grep "^SELINUXTYPE" | tail -1 | awk -F= '{ print $2 '}`
	if [ "eq$TYPE" != "eqdummy" ]; then
		selinuxenabled
		if [ $? -eq 0 ]; then
			echo "SELinux already enabled with a non-dummy policy."
			echo "Exiting.  Please install policy by hand if that"
			echo "is what you REALLY want."
			exit 1
		fi
		mv /etc/selinux/config /etc/selinux/config.mdpbak
		grep -v "^SELINUXTYPE" /etc/selinux/config.mdpbak >> /etc/selinux/config
		echo "SELINUXTYPE=dummy" >> /etc/selinux/config
	fi
fi

cd /etc/selinux/dummy/contexts/files
$SF file_contexts /

mounts=`cat /proc/$$/mounts | egrep "ext2|ext3|xfs|jfs|ext4|ext4dev|gfs2" | awk '{ print $2 '}`
$SF file_contexts $mounts


dodev=`cat /proc/$$/mounts | grep "/dev "`
if [ "eq$dodev" != "eq" ]; then
	mount --move /dev /mnt
	$SF file_contexts /dev
	mount --move /mnt /dev
fi

