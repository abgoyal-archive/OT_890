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
 * arch/x86_64/lib/csum-partial.c
 *
 * This file contains network checksum routines that are better done
 * in an architecture-specific manner due to speed.
 */
 
#include <linux/compiler.h>
#include <linux/module.h>
#include <asm/checksum.h>

static inline unsigned short from32to16(unsigned a) 
{
	unsigned short b = a >> 16; 
	asm("addw %w2,%w0\n\t"
	    "adcw $0,%w0\n" 
	    : "=r" (b)
	    : "0" (b), "r" (a));
	return b;
}

/*
 * Do a 64-bit checksum on an arbitrary memory area.
 * Returns a 32bit checksum.
 *
 * This isn't as time critical as it used to be because many NICs
 * do hardware checksumming these days.
 * 
 * Things tried and found to not make it faster:
 * Manual Prefetching
 * Unrolling to an 128 bytes inner loop.
 * Using interleaving with more registers to break the carry chains.
 */
static unsigned do_csum(const unsigned char *buff, unsigned len)
{
	unsigned odd, count;
	unsigned long result = 0;

	if (unlikely(len == 0))
		return result; 
	odd = 1 & (unsigned long) buff;
	if (unlikely(odd)) {
		result = *buff << 8;
		len--;
		buff++;
	}
	count = len >> 1;		/* nr of 16-bit words.. */
	if (count) {
		if (2 & (unsigned long) buff) {
			result += *(unsigned short *)buff;
			count--;
			len -= 2;
			buff += 2;
		}
		count >>= 1;		/* nr of 32-bit words.. */
		if (count) {
			unsigned long zero;
			unsigned count64;
			if (4 & (unsigned long) buff) {
				result += *(unsigned int *) buff;
				count--;
				len -= 4;
				buff += 4;
			}
			count >>= 1;	/* nr of 64-bit words.. */

			/* main loop using 64byte blocks */
			zero = 0;
			count64 = count >> 3;
			while (count64) { 
				asm("addq 0*8(%[src]),%[res]\n\t"
				    "adcq 1*8(%[src]),%[res]\n\t"
				    "adcq 2*8(%[src]),%[res]\n\t"
				    "adcq 3*8(%[src]),%[res]\n\t"
				    "adcq 4*8(%[src]),%[res]\n\t"
				    "adcq 5*8(%[src]),%[res]\n\t"
				    "adcq 6*8(%[src]),%[res]\n\t"
				    "adcq 7*8(%[src]),%[res]\n\t"
				    "adcq %[zero],%[res]"
				    : [res] "=r" (result)
				    : [src] "r" (buff), [zero] "r" (zero),
				    "[res]" (result));
				buff += 64;
				count64--;
			}

			/* last upto 7 8byte blocks */
			count %= 8; 
			while (count) { 
				asm("addq %1,%0\n\t"
				    "adcq %2,%0\n" 
					    : "=r" (result)
				    : "m" (*(unsigned long *)buff), 
				    "r" (zero),  "0" (result));
				--count; 
					buff += 8;
			}
			result = add32_with_carry(result>>32,
						  result&0xffffffff); 

			if (len & 4) {
				result += *(unsigned int *) buff;
				buff += 4;
			}
		}
		if (len & 2) {
			result += *(unsigned short *) buff;
			buff += 2;
		}
	}
	if (len & 1)
		result += *buff;
	result = add32_with_carry(result>>32, result & 0xffffffff); 
	if (unlikely(odd)) { 
		result = from32to16(result);
		result = ((result >> 8) & 0xff) | ((result & 0xff) << 8);
	}
	return result;
}

/*
 * computes the checksum of a memory block at buff, length len,
 * and adds in "sum" (32-bit)
 *
 * returns a 32-bit number suitable for feeding into itself
 * or csum_tcpudp_magic
 *
 * this function must be called with even lengths, except
 * for the last fragment, which may be odd
 *
 * it's best to have buff aligned on a 64-bit boundary
 */
__wsum csum_partial(const void *buff, int len, __wsum sum)
{
	return (__force __wsum)add32_with_carry(do_csum(buff, len),
						(__force u32)sum);
}

/*
 * this routine is used for miscellaneous IP-like checksums, mainly
 * in icmp.c
 */
__sum16 ip_compute_csum(const void *buff, int len)
{
	return csum_fold(csum_partial(buff,len,0));
}
EXPORT_SYMBOL(ip_compute_csum);

