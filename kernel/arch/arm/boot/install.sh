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
# arch/arm/boot/install.sh
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995 by Linus Torvalds
#
# Adapted from code in arch/i386/boot/Makefile by H. Peter Anvin
# Adapted from code in arch/i386/boot/install.sh by Russell King
#
# "make install" script for arm architecture
#
# Arguments:
#   $1 - kernel version
#   $2 - kernel image file
#   $3 - kernel map file
#   $4 - default install path (blank if root directory)
#

# User may have a custom install script
if [ -x ~/bin/${CROSS_COMPILE}installkernel ]; then exec ~/bin/${CROSS_COMPILE}installkernel "$@"; fi
if [ -x /sbin/${CROSS_COMPILE}installkernel ]; then exec /sbin/${CROSS_COMPILE}installkernel "$@"; fi

if [ "$(basename $2)" = "zImage" ]; then
# Compressed install
  echo "Installing compressed kernel"
  base=vmlinuz
else
# Normal install
  echo "Installing normal kernel"
  base=vmlinux
fi

if [ -f $4/$base-$1 ]; then
  mv $4/$base-$1 $4/$base-$1.old
fi
cat $2 > $4/$base-$1

# Install system map file
if [ -f $4/System.map-$1 ]; then
  mv $4/System.map-$1 $4/System.map-$1.old
fi
cp $3 $4/System.map-$1

if [ -x /sbin/loadmap ]; then
  /sbin/loadmap
else
  echo "You have to install it yourself"
fi
