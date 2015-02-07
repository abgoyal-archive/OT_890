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


# Guest requires the device configuration and probing code.
obj-$(CONFIG_LGUEST_GUEST) += lguest_device.o

# Host requires the other files, which can be a module.
obj-$(CONFIG_LGUEST)	+= lg.o
lg-y = core.o hypercalls.o page_tables.o interrupts_and_traps.o \
	segments.o lguest_user.o

lg-$(CONFIG_X86_32) += x86/switcher_32.o x86/core.o

Preparation Preparation!: PREFIX=P
Guest: PREFIX=G
Drivers: PREFIX=D
Launcher: PREFIX=L
Host: PREFIX=H
Switcher: PREFIX=S
Mastery: PREFIX=M
Beer:
	@for f in Preparation Guest Drivers Launcher Host Switcher Mastery; do echo "{==- $$f -==}"; make -s $$f; done; echo "{==-==}"
Preparation Preparation! Guest Drivers Launcher Host Switcher Mastery:
	@sh ../../Documentation/lguest/extract $(PREFIX) `find ../../* -name '*.[chS]' -wholename '*lguest*'`
Puppy:
	@clear
	@printf "      __  \n (___()'\`;\n /,    /\`\n \\\\\\\"--\\\\\\   \n"
	@sleep 2; clear; printf "\n\n   Sit!\n\n"; sleep 1; clear
	@printf "    __    \n   ()'\`;  \n   /\\|\` \n  /  |  \n(/_)_|_   \n"
	@sleep 2; clear; printf "\n\n  Stand!\n\n"; sleep 1; clear
	@printf "    __    \n   ()'\`;  \n   /\\|\` \n  /._.= \n /| /     \n(_\_)_    \n"
	@sleep 2; clear; printf "\n\n  Good puppy!\n\n"; sleep 1; clear
