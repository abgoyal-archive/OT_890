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
# Makefile for ia64-specific library routines..
#

obj-y := io.o

lib-y := __divsi3.o __udivsi3.o __modsi3.o __umodsi3.o			\
	__divdi3.o __udivdi3.o __moddi3.o __umoddi3.o			\
	checksum.o clear_page.o csum_partial_copy.o			\
	clear_user.o strncpy_from_user.o strlen_user.o strnlen_user.o	\
	flush.o ip_fast_csum.o do_csum.o				\
	memset.o strlen.o xor.o

obj-$(CONFIG_ITANIUM)	+= copy_page.o copy_user.o memcpy.o
obj-$(CONFIG_MCKINLEY)	+= copy_page_mck.o memcpy_mck.o
lib-$(CONFIG_PERFMON)	+= carta_random.o

AFLAGS___divdi3.o	=
AFLAGS___udivdi3.o	= -DUNSIGNED
AFLAGS___moddi3.o	= 	     -DMODULO
AFLAGS___umoddi3.o	= -DUNSIGNED -DMODULO

AFLAGS___divsi3.o	=
AFLAGS___udivsi3.o	= -DUNSIGNED
AFLAGS___modsi3.o	=	     -DMODULO
AFLAGS___umodsi3.o	= -DUNSIGNED -DMODULO

$(obj)/__divdi3.o: $(src)/idiv64.S FORCE
	$(call if_changed_dep,as_o_S)

$(obj)/__udivdi3.o: $(src)/idiv64.S FORCE
	$(call if_changed_dep,as_o_S)

$(obj)/__moddi3.o: $(src)/idiv64.S FORCE
	$(call if_changed_dep,as_o_S)

$(obj)/__umoddi3.o: $(src)/idiv64.S FORCE
	$(call if_changed_dep,as_o_S)

$(obj)/__divsi3.o: $(src)/idiv32.S FORCE
	$(call if_changed_dep,as_o_S)

$(obj)/__udivsi3.o: $(src)/idiv32.S FORCE
	$(call if_changed_dep,as_o_S)

$(obj)/__modsi3.o: $(src)/idiv32.S FORCE
	$(call if_changed_dep,as_o_S)

$(obj)/__umodsi3.o: $(src)/idiv32.S FORCE
	$(call if_changed_dep,as_o_S)
