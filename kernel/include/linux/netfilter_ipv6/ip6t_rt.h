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

#ifndef _IP6T_RT_H
#define _IP6T_RT_H

/*#include <linux/in6.h>*/

#define IP6T_RT_HOPS 16

struct ip6t_rt
{
	u_int32_t rt_type;			/* Routing Type */
	u_int32_t segsleft[2];			/* Segments Left */
	u_int32_t hdrlen;			/* Header Length */
	u_int8_t  flags;			/*  */
	u_int8_t  invflags;			/* Inverse flags */
	struct in6_addr addrs[IP6T_RT_HOPS];	/* Hops */
	u_int8_t addrnr;			/* Nr of Addresses */
};

#define IP6T_RT_TYP 		0x01
#define IP6T_RT_SGS 		0x02
#define IP6T_RT_LEN 		0x04
#define IP6T_RT_RES 		0x08
#define IP6T_RT_FST_MASK	0x30
#define IP6T_RT_FST 		0x10
#define IP6T_RT_FST_NSTRICT	0x20

/* Values for "invflags" field in struct ip6t_rt. */
#define IP6T_RT_INV_TYP		0x01	/* Invert the sense of type. */
#define IP6T_RT_INV_SGS		0x02	/* Invert the sense of Segments. */
#define IP6T_RT_INV_LEN		0x04	/* Invert the sense of length. */
#define IP6T_RT_INV_MASK	0x07	/* All possible flags. */

#endif /*_IP6T_RT_H*/
