/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 *
 *		SNMP MIB entries for the IP subsystem.
 *		
 *		Alan Cox <gw4pts@gw4pts.ampr.org>
 *
 *		We don't chose to implement SNMP in the kernel (this would
 *		be silly as SNMP is a pain in the backside in places). We do
 *		however need to collect the MIB statistics and export them
 *		out of /proc (eventually)
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 */
 
#ifndef _SNMP_H
#define _SNMP_H

#include <linux/cache.h>
#include <linux/snmp.h>
#include <linux/smp.h>

/*
 * Mibs are stored in array of unsigned long.
 */
/*
 * struct snmp_mib{}
 *  - list of entries for particular API (such as /proc/net/snmp)
 *  - name of entries.
 */
struct snmp_mib {
	char *name;
	int entry;
};

#define SNMP_MIB_ITEM(_name,_entry)	{	\
	.name = _name,				\
	.entry = _entry,			\
}

#define SNMP_MIB_SENTINEL {	\
	.name = NULL,		\
	.entry = 0,		\
}

/*
 * We use all unsigned longs. Linux will soon be so reliable that even 
 * these will rapidly get too small 8-). Seriously consider the IpInReceives 
 * count on the 20Gb/s + networks people expect in a few years time!
 */

/* 
 * The rule for padding: 
 * Best is power of two because then the right structure can be found by a 
 * simple shift. The structure should be always cache line aligned.
 * gcc needs n=alignto(cachelinesize, popcnt(sizeof(bla_mib))) shift/add 
 * instructions to emulate multiply in case it is not power-of-two. 
 * Currently n is always <=3 for all sizes so simple cache line alignment 
 * is enough. 
 * 
 * The best solution would be a global CPU local area , especially on 64 
 * and 128byte cacheline machine it makes a *lot* of sense -AK
 */ 

#define __SNMP_MIB_ALIGN__	____cacheline_aligned

/* IPstats */
#define IPSTATS_MIB_MAX	__IPSTATS_MIB_MAX
struct ipstats_mib {
	unsigned long	mibs[IPSTATS_MIB_MAX];
} __SNMP_MIB_ALIGN__;

/* ICMP */
#define ICMP_MIB_DUMMY	__ICMP_MIB_MAX
#define ICMP_MIB_MAX	(__ICMP_MIB_MAX + 1)

struct icmp_mib {
	unsigned long	mibs[ICMP_MIB_MAX];
} __SNMP_MIB_ALIGN__;

#define ICMPMSG_MIB_MAX	__ICMPMSG_MIB_MAX
struct icmpmsg_mib {
	unsigned long	mibs[ICMPMSG_MIB_MAX];
} __SNMP_MIB_ALIGN__;

/* ICMP6 (IPv6-ICMP) */
#define ICMP6_MIB_MAX	__ICMP6_MIB_MAX
struct icmpv6_mib {
	unsigned long	mibs[ICMP6_MIB_MAX];
} __SNMP_MIB_ALIGN__;

#define ICMP6MSG_MIB_MAX  __ICMP6MSG_MIB_MAX
struct icmpv6msg_mib {
	unsigned long	mibs[ICMP6MSG_MIB_MAX];
} __SNMP_MIB_ALIGN__;


/* TCP */
#define TCP_MIB_MAX	__TCP_MIB_MAX
struct tcp_mib {
	unsigned long	mibs[TCP_MIB_MAX];
} __SNMP_MIB_ALIGN__;

/* UDP */
#define UDP_MIB_MAX	__UDP_MIB_MAX
struct udp_mib {
	unsigned long	mibs[UDP_MIB_MAX];
} __SNMP_MIB_ALIGN__;

/* Linux */
#define LINUX_MIB_MAX	__LINUX_MIB_MAX
struct linux_mib {
	unsigned long	mibs[LINUX_MIB_MAX];
};

/* Linux Xfrm */
#define LINUX_MIB_XFRMMAX	__LINUX_MIB_XFRMMAX
struct linux_xfrm_mib {
	unsigned long	mibs[LINUX_MIB_XFRMMAX];
};

/* 
 * FIXME: On x86 and some other CPUs the split into user and softirq parts
 * is not needed because addl $1,memory is atomic against interrupts (but 
 * atomic_inc would be overkill because of the lock cycles). Wants new 
 * nonlocked_atomic_inc() primitives -AK
 */ 
#define DEFINE_SNMP_STAT(type, name)	\
	__typeof__(type) *name[2]
#define DECLARE_SNMP_STAT(type, name)	\
	extern __typeof__(type) *name[2]

#define SNMP_STAT_BHPTR(name)	(name[0])
#define SNMP_STAT_USRPTR(name)	(name[1])

#define SNMP_INC_STATS_BH(mib, field) 	\
	(per_cpu_ptr(mib[0], raw_smp_processor_id())->mibs[field]++)
#define SNMP_INC_STATS_USER(mib, field) \
	do { \
		per_cpu_ptr(mib[1], get_cpu())->mibs[field]++; \
		put_cpu(); \
	} while (0)
#define SNMP_INC_STATS(mib, field) 	\
	do { \
		per_cpu_ptr(mib[!in_softirq()], get_cpu())->mibs[field]++; \
		put_cpu(); \
	} while (0)
#define SNMP_DEC_STATS(mib, field) 	\
	do { \
		per_cpu_ptr(mib[!in_softirq()], get_cpu())->mibs[field]--; \
		put_cpu(); \
	} while (0)
#define SNMP_ADD_STATS_BH(mib, field, addend) 	\
	(per_cpu_ptr(mib[0], raw_smp_processor_id())->mibs[field] += addend)
#define SNMP_ADD_STATS_USER(mib, field, addend) 	\
	do { \
		per_cpu_ptr(mib[1], get_cpu())->mibs[field] += addend; \
		put_cpu(); \
	} while (0)

#endif
