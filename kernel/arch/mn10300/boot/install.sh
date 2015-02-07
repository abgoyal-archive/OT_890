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
# arch/mn10300/boot/install -c.sh
#
# This file is subject to the terms and conditions of the GNU General Public
# Licence.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995 by Linus Torvalds
#
# Adapted from code in arch/i386/boot/Makefile by H. Peter Anvin
#
# "make install -c" script for i386 architecture
#
# Arguments:
#   $1 - kernel version
#   $2 - kernel image file
#   $3 - kernel map file
#   $4 - default install -c path (blank if root directory)
#   $5 - boot rom file
#

# User may have a custom install -c script

rm -fr $4/../usr/include/linux $4/../usr/include/asm
install -c -m 0755 $2 $4/vmlinuz
install -c -m 0755 $5 $4/boot.rom
install -c -m 0755 -d $4/../usr/include/linux
cd ${srctree}/include/linux
for i in `find . -maxdepth 1 -name '*.h' -print`; do
  install -c -m 0644 $i $4/../usr/include/linux
done
install -c -m 0755 -d $4/../usr/include/linux/byteorder
cd ${srctree}/include/linux/byteorder
for i in `find . -name '*.h' -print`; do
  install -c -m 0644 $i $4/../usr/include/linux/byteorder
done
install -c -m 0755 -d $4/../usr/include/linux/lockd
cd ${srctree}/include/linux/lockd
for i in `find . -name '*.h' -print`; do
  install -c -m 0644 $i $4/../usr/include/linux/lockd
done
install -c -m 0755 -d $4/../usr/include/linux/netfilter_ipv4
cd ${srctree}/include/linux/netfilter_ipv4
for i in `find . -name '*.h' -print`; do
  install -c -m 0644 $i $4/../usr/include/linux/netfilter_ipv4
done
install -c -m 0755 -d $4/../usr/include/linux/nfsd
cd ${srctree}/include/linux/nfsd
for i in `find . -name '*.h' -print`; do
  install -c -m 0644 $i $4/../usr/include/linux/nfsd/$i
done
install -c -m 0755 -d $4/../usr/include/linux/raid
cd ${srctree}/include/linux/raid
for i in `find . -name '*.h' -print`; do
  install -c -m 0644 $i $4/../usr/include/linux/raid
done
install -c -m 0755 -d $4/../usr/include/linux/sunrpc
cd ${srctree}/include/linux/sunrpc
for i in `find . -name '*.h' -print`; do
  install -c -m 0644 $i $4/../usr/include/linux/sunrpc
done
install -c -m 0755 -d $4/../usr/include/asm
cd ${srctree}/include/asm
for i in `find . -name '*.h' -print`; do
  install -c -m 0644 $i $4/../usr/include/asm
done
