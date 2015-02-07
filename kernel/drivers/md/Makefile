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
# Makefile for the kernel software RAID and LVM drivers.
#

dm-mod-objs	:= dm.o dm-table.o dm-target.o dm-linear.o dm-stripe.o \
		   dm-ioctl.o dm-io.o dm-kcopyd.o dm-sysfs.o
dm-multipath-objs := dm-path-selector.o dm-mpath.o
dm-snapshot-objs := dm-snap.o dm-exception-store.o dm-snap-transient.o \
		    dm-snap-persistent.o
dm-mirror-objs	:= dm-raid1.o
md-mod-objs     := md.o bitmap.o
raid456-objs	:= raid5.o raid6algos.o raid6recov.o raid6tables.o \
		   raid6int1.o raid6int2.o raid6int4.o \
		   raid6int8.o raid6int16.o raid6int32.o \
		   raid6altivec1.o raid6altivec2.o raid6altivec4.o \
		   raid6altivec8.o \
		   raid6mmx.o raid6sse1.o raid6sse2.o
hostprogs-y	:= mktables

# Note: link order is important.  All raid personalities
# and must come before md.o, as they each initialise 
# themselves, and md.o may use the personalities when it 
# auto-initialised.

obj-$(CONFIG_MD_LINEAR)		+= linear.o
obj-$(CONFIG_MD_RAID0)		+= raid0.o
obj-$(CONFIG_MD_RAID1)		+= raid1.o
obj-$(CONFIG_MD_RAID10)		+= raid10.o
obj-$(CONFIG_MD_RAID456)	+= raid456.o
obj-$(CONFIG_MD_MULTIPATH)	+= multipath.o
obj-$(CONFIG_MD_FAULTY)		+= faulty.o
obj-$(CONFIG_BLK_DEV_MD)	+= md-mod.o
obj-$(CONFIG_BLK_DEV_DM)	+= dm-mod.o
obj-$(CONFIG_DM_CRYPT)		+= dm-crypt.o
obj-$(CONFIG_DM_DELAY)		+= dm-delay.o
obj-$(CONFIG_DM_MULTIPATH)	+= dm-multipath.o dm-round-robin.o
obj-$(CONFIG_DM_SNAPSHOT)	+= dm-snapshot.o
obj-$(CONFIG_DM_MIRROR)		+= dm-mirror.o dm-log.o dm-region-hash.o
obj-$(CONFIG_DM_ZERO)		+= dm-zero.o

quiet_cmd_unroll = UNROLL  $@
      cmd_unroll = $(PERL) $(srctree)/$(src)/unroll.pl $(UNROLL) \
                   < $< > $@ || ( rm -f $@ && exit 1 )

ifeq ($(CONFIG_ALTIVEC),y)
altivec_flags := -maltivec -mabi=altivec
endif

ifeq ($(CONFIG_DM_UEVENT),y)
dm-mod-objs			+= dm-uevent.o
endif

targets += raid6int1.c
$(obj)/raid6int1.c:   UNROLL := 1
$(obj)/raid6int1.c:   $(src)/raid6int.uc $(src)/unroll.pl FORCE
	$(call if_changed,unroll)

targets += raid6int2.c
$(obj)/raid6int2.c:   UNROLL := 2
$(obj)/raid6int2.c:   $(src)/raid6int.uc $(src)/unroll.pl FORCE
	$(call if_changed,unroll)

targets += raid6int4.c
$(obj)/raid6int4.c:   UNROLL := 4
$(obj)/raid6int4.c:   $(src)/raid6int.uc $(src)/unroll.pl FORCE
	$(call if_changed,unroll)

targets += raid6int8.c
$(obj)/raid6int8.c:   UNROLL := 8
$(obj)/raid6int8.c:   $(src)/raid6int.uc $(src)/unroll.pl FORCE
	$(call if_changed,unroll)

targets += raid6int16.c
$(obj)/raid6int16.c:  UNROLL := 16
$(obj)/raid6int16.c:  $(src)/raid6int.uc $(src)/unroll.pl FORCE
	$(call if_changed,unroll)

targets += raid6int32.c
$(obj)/raid6int32.c:  UNROLL := 32
$(obj)/raid6int32.c:  $(src)/raid6int.uc $(src)/unroll.pl FORCE
	$(call if_changed,unroll)

CFLAGS_raid6altivec1.o += $(altivec_flags)
targets += raid6altivec1.c
$(obj)/raid6altivec1.c:   UNROLL := 1
$(obj)/raid6altivec1.c:   $(src)/raid6altivec.uc $(src)/unroll.pl FORCE
	$(call if_changed,unroll)

CFLAGS_raid6altivec2.o += $(altivec_flags)
targets += raid6altivec2.c
$(obj)/raid6altivec2.c:   UNROLL := 2
$(obj)/raid6altivec2.c:   $(src)/raid6altivec.uc $(src)/unroll.pl FORCE
	$(call if_changed,unroll)

CFLAGS_raid6altivec4.o += $(altivec_flags)
targets += raid6altivec4.c
$(obj)/raid6altivec4.c:   UNROLL := 4
$(obj)/raid6altivec4.c:   $(src)/raid6altivec.uc $(src)/unroll.pl FORCE
	$(call if_changed,unroll)

CFLAGS_raid6altivec8.o += $(altivec_flags)
targets += raid6altivec8.c
$(obj)/raid6altivec8.c:   UNROLL := 8
$(obj)/raid6altivec8.c:   $(src)/raid6altivec.uc $(src)/unroll.pl FORCE
	$(call if_changed,unroll)

quiet_cmd_mktable = TABLE   $@
      cmd_mktable = $(obj)/mktables > $@ || ( rm -f $@ && exit 1 )

targets += raid6tables.c
$(obj)/raid6tables.c: $(obj)/mktables FORCE
	$(call if_changed,mktable)
