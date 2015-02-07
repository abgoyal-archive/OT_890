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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*
 * include/linux/random.h
 *
 * Include file for the random number generator.
 */

#ifndef _LINUX_RANDOM_H
#define _LINUX_RANDOM_H

#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/irqnr.h>

#include <asm/tcm.h>

/* ioctl()'s for the random number generator */

/* Get the entropy count. */
#define RNDGETENTCNT	_IOR( 'R', 0x00, int )

/* Add to (or subtract from) the entropy count.  (Superuser only.) */
#define RNDADDTOENTCNT	_IOW( 'R', 0x01, int )

/* Get the contents of the entropy pool.  (Superuser only.) */
#define RNDGETPOOL	_IOR( 'R', 0x02, int [2] )

/* 
 * Write bytes into the entropy pool and add to the entropy count.
 * (Superuser only.)
 */
#define RNDADDENTROPY	_IOW( 'R', 0x03, int [2] )

/* Clear entropy count to 0.  (Superuser only.) */
#define RNDZAPENTCNT	_IO( 'R', 0x04 )

/* Clear the entropy pool and associated counters.  (Superuser only.) */
#define RNDCLEARPOOL	_IO( 'R', 0x06 )

struct rand_pool_info {
	int	entropy_count;
	int	buf_size;
	__u32	buf[0];
};

/* Exported functions */

#ifdef __KERNEL__

extern void rand_initialize_irq(int irq);

extern void add_input_randomness(unsigned int type, unsigned int code,
				 unsigned int value);
extern void __tcmfunc add_interrupt_randomness(int irq);

extern void get_random_bytes(void *buf, int nbytes);
void generate_random_uuid(unsigned char uuid_out[16]);

extern __u32 secure_ip_id(__be32 daddr);
extern u32 secure_ipv4_port_ephemeral(__be32 saddr, __be32 daddr, __be16 dport);
extern u32 secure_ipv6_port_ephemeral(const __be32 *saddr, const __be32 *daddr,
				      __be16 dport);
extern __u32 secure_tcp_sequence_number(__be32 saddr, __be32 daddr,
					__be16 sport, __be16 dport);
extern __u32 secure_tcpv6_sequence_number(__be32 *saddr, __be32 *daddr,
					  __be16 sport, __be16 dport);
extern u64 secure_dccp_sequence_number(__be32 saddr, __be32 daddr,
				       __be16 sport, __be16 dport);

#ifndef MODULE
extern const struct file_operations random_fops, urandom_fops;
#endif

unsigned int get_random_int(void);
unsigned long randomize_range(unsigned long start, unsigned long end, unsigned long len);

u32 random32(void);
void srandom32(u32 seed);

#endif /* __KERNEL___ */

#endif /* _LINUX_RANDOM_H */
