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
# Makefile for x86-compatible CPU details and quirks
#

# Don't trace early stages of a secondary CPU boot
ifdef CONFIG_FUNCTION_TRACER
CFLAGS_REMOVE_common.o = -pg
endif

obj-y			:= intel_cacheinfo.o addon_cpuid_features.o
obj-y			+= proc.o capflags.o powerflags.o common.o
obj-y			+= vmware.o hypervisor.o

obj-$(CONFIG_X86_32)	+= bugs.o cmpxchg.o
obj-$(CONFIG_X86_64)	+= bugs_64.o

obj-$(CONFIG_CPU_SUP_INTEL)		+= intel.o
obj-$(CONFIG_CPU_SUP_AMD)		+= amd.o
obj-$(CONFIG_CPU_SUP_CYRIX_32)		+= cyrix.o
obj-$(CONFIG_CPU_SUP_CENTAUR_32)	+= centaur.o
obj-$(CONFIG_CPU_SUP_CENTAUR_64)	+= centaur_64.o
obj-$(CONFIG_CPU_SUP_TRANSMETA_32)	+= transmeta.o
obj-$(CONFIG_CPU_SUP_UMC_32)		+= umc.o

obj-$(CONFIG_X86_MCE)	+= mcheck/
obj-$(CONFIG_MTRR)	+= mtrr/
obj-$(CONFIG_CPU_FREQ)	+= cpufreq/

obj-$(CONFIG_X86_LOCAL_APIC) += perfctr-watchdog.o

quiet_cmd_mkcapflags = MKCAP   $@
      cmd_mkcapflags = $(PERL) $(srctree)/$(src)/mkcapflags.pl $< $@

cpufeature = $(src)/../../include/asm/cpufeature.h

targets += capflags.c
$(obj)/capflags.c: $(cpufeature) $(src)/mkcapflags.pl FORCE
	$(call if_changed,mkcapflags)
