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
# Copyright (C) 2000, 2002, 2003 Jeff Dike (jdike@karaya.com)
# Licensed under the GPL
#

# pcap is broken in 2.5 because kbuild doesn't allow pcap.a to be linked
# in to pcap.o

slip-objs := slip_kern.o slip_user.o
slirp-objs := slirp_kern.o slirp_user.o
daemon-objs := daemon_kern.o daemon_user.o
mcast-objs := mcast_kern.o mcast_user.o
net-objs := net_kern.o net_user.o
mconsole-objs := mconsole_kern.o mconsole_user.o
hostaudio-objs := hostaudio_kern.o
ubd-objs := ubd_kern.o ubd_user.o
port-objs := port_kern.o port_user.o
harddog-objs := harddog_kern.o harddog_user.o

LDFLAGS_pcap.o := -r $(shell $(CC) $(KBUILD_CFLAGS) -print-file-name=libpcap.a)

LDFLAGS_vde.o := -r $(shell $(CC) $(CFLAGS) -print-file-name=libvdeplug.a)

targets := pcap_kern.o pcap_user.o vde_kern.o vde_user.o

$(obj)/pcap.o: $(obj)/pcap_kern.o $(obj)/pcap_user.o
	$(LD) -r -dp -o $@ $^ $(LDFLAGS) $(LDFLAGS_pcap.o)

$(obj)/vde.o: $(obj)/vde_kern.o $(obj)/vde_user.o
	$(LD) -r -dp -o $@ $^ $(LDFLAGS) $(LDFLAGS_vde.o)

#XXX: The call below does not work because the flags are added before the
# object name, so nothing from the library gets linked.
#$(call if_changed,ld)

# When the above is fixed, don't forget to add this too!
#targets += $(obj)/pcap.o

obj-y := stdio_console.o fd.o chan_kern.o chan_user.o line.o
obj-$(CONFIG_SSL) += ssl.o
obj-$(CONFIG_STDERR_CONSOLE) += stderr_console.o

obj-$(CONFIG_UML_NET_SLIP) += slip.o slip_common.o
obj-$(CONFIG_UML_NET_SLIRP) += slirp.o slip_common.o
obj-$(CONFIG_UML_NET_DAEMON) += daemon.o 
obj-$(CONFIG_UML_NET_VDE) += vde.o
obj-$(CONFIG_UML_NET_MCAST) += mcast.o 
obj-$(CONFIG_UML_NET_PCAP) += pcap.o
obj-$(CONFIG_UML_NET) += net.o 
obj-$(CONFIG_MCONSOLE) += mconsole.o
obj-$(CONFIG_MMAPPER) += mmapper_kern.o 
obj-$(CONFIG_BLK_DEV_UBD) += ubd.o 
obj-$(CONFIG_HOSTAUDIO) += hostaudio.o
obj-$(CONFIG_NULL_CHAN) += null.o 
obj-$(CONFIG_PORT_CHAN) += port.o
obj-$(CONFIG_PTY_CHAN) += pty.o
obj-$(CONFIG_TTY_CHAN) += tty.o 
obj-$(CONFIG_XTERM_CHAN) += xterm.o xterm_kern.o
obj-$(CONFIG_UML_WATCHDOG) += harddog.o
obj-$(CONFIG_BLK_DEV_COW_COMMON) += cow_user.o
obj-$(CONFIG_UML_RANDOM) += random.o

# pcap_user.o must be added explicitly.
USER_OBJS := fd.o null.o pty.o tty.o xterm.o slip_common.o pcap_user.o vde_user.o
CFLAGS_null.o = -DDEV_NULL=$(DEV_NULL_PATH)

include arch/um/scripts/Makefile.rules
