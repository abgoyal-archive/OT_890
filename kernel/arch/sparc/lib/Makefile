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


# Makefile for Sparc library files..
#

asflags-y := -ansi -DST_DIV0=0x02
ccflags-y := -Werror

lib-$(CONFIG_SPARC32) += mul.o rem.o sdiv.o udiv.o umul.o urem.o ashrdi3.o
lib-$(CONFIG_SPARC32) += memcpy.o memset.o
lib-y                 += strlen.o
lib-y                 += checksum_$(BITS).o
lib-$(CONFIG_SPARC32) += blockops.o
lib-y                 += memscan_$(BITS).o memcmp.o strncmp_$(BITS).o
lib-y                 += strncpy_from_user_$(BITS).o strlen_user_$(BITS).o
lib-$(CONFIG_SPARC32) += divdi3.o udivdi3.o
lib-$(CONFIG_SPARC32) += copy_user.o locks.o
lib-y                 += atomic_$(BITS).o
lib-$(CONFIG_SPARC32) += lshrdi3.o ashldi3.o
lib-y                 += rwsem_$(BITS).o
lib-$(CONFIG_SPARC32) += muldi3.o bitext.o cmpdi2.o

lib-$(CONFIG_SPARC64) += copy_page.o clear_page.o bzero.o
lib-$(CONFIG_SPARC64) += csum_copy.o csum_copy_from_user.o csum_copy_to_user.o
lib-$(CONFIG_SPARC64) += VISsave.o
lib-$(CONFIG_SPARC64) += bitops.o

lib-$(CONFIG_SPARC64) += U1memcpy.o U1copy_from_user.o U1copy_to_user.o

lib-$(CONFIG_SPARC64) += U3memcpy.o U3copy_from_user.o U3copy_to_user.o
lib-$(CONFIG_SPARC64) += U3patch.o

lib-$(CONFIG_SPARC64) += NGmemcpy.o NGcopy_from_user.o NGcopy_to_user.o
lib-$(CONFIG_SPARC64) += NGpatch.o NGpage.o NGbzero.o

lib-$(CONFIG_SPARC64) += NG2memcpy.o NG2copy_from_user.o NG2copy_to_user.o
lib-$(CONFIG_SPARC64) +=  NG2patch.o NG2page.o

lib-$(CONFIG_SPARC64) += GENmemcpy.o GENcopy_from_user.o GENcopy_to_user.o
lib-$(CONFIG_SPARC64) += GENpatch.o GENpage.o GENbzero.o

lib-$(CONFIG_SPARC64) += copy_in_user.o user_fixup.o memmove.o
lib-$(CONFIG_SPARC64) += mcount.o ipcsum.o xor.o

obj-y                 += iomap.o
obj-$(CONFIG_SPARC32) += atomic32.o
obj-y                 += ksyms.o
obj-$(CONFIG_SPARC64) += PeeCeeI.o
