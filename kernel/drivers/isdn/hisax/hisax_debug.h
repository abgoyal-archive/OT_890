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
 * Common debugging macros for use with the hisax driver
 *
 * Author       Frode Isaksen
 * Copyright    2001 by Frode Isaksen      <fisaksen@bewan.com>
 *              2001 by Kai Germaschewski  <kai.germaschewski@gmx.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 * How to use:
 * 
 * Before including this file, you need to
 *   #define __debug_variable my_debug
 * where my_debug is a variable in your code which
 * determines the debug bitmask.
 *
 * If CONFIG_HISAX_DEBUG is not set, all macros evaluate to nothing
 *
 */

#ifndef __HISAX_DEBUG_H__
#define __HISAX_DEBUG_H__


#ifdef CONFIG_HISAX_DEBUG

#define DBG(level, format, arg...) do { \
if (level & __debug_variable) \
printk(KERN_DEBUG "%s: " format "\n" , __func__ , ## arg); \
} while (0)

#define DBG_PACKET(level,data,count) \
  if (level & __debug_variable) dump_packet(__func__,data,count)

#define DBG_SKB(level,skb) \
  if ((level & __debug_variable) && skb) dump_packet(__func__,skb->data,skb->len)


static void __attribute__((unused))
dump_packet(const char *name,const u_char *data,int pkt_len)
{
#define DUMP_HDR_SIZE 20
#define DUMP_TLR_SIZE 8
	if (pkt_len) {
		int i,len1,len2;

		printk(KERN_DEBUG "%s: length=%d,data=",name,pkt_len);

		if (pkt_len >  DUMP_HDR_SIZE+ DUMP_TLR_SIZE) {
			len1 = DUMP_HDR_SIZE;
			len2 = DUMP_TLR_SIZE;
		} else {
			len1 = pkt_len > DUMP_HDR_SIZE ? DUMP_HDR_SIZE : pkt_len;
			len2 = 0;			
		}
		for (i = 0; i < len1; ++i) {
		 	printk ("%.2x", data[i]);
		}
		if (len2) {
		 	printk ("..");
			for (i = pkt_len-DUMP_TLR_SIZE; i < pkt_len; ++i) {
				printk ("%.2x", data[i]);
			}
		}
		printk ("\n");
	}
#undef DUMP_HDR_SIZE
#undef DUMP_TLR_SIZE
}

#else

#define DBG(level, format, arg...) do {} while (0)
#define DBG_PACKET(level,data,count) do {} while (0)
#define DBG_SKB(level,skb) do {} while (0)

#endif

#endif
