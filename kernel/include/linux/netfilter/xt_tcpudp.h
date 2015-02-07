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

#ifndef _XT_TCPUDP_H
#define _XT_TCPUDP_H

/* TCP matching stuff */
struct xt_tcp
{
	u_int16_t spts[2];			/* Source port range. */
	u_int16_t dpts[2];			/* Destination port range. */
	u_int8_t option;			/* TCP Option iff non-zero*/
	u_int8_t flg_mask;			/* TCP flags mask byte */
	u_int8_t flg_cmp;			/* TCP flags compare byte */
	u_int8_t invflags;			/* Inverse flags */
};

/* Values for "inv" field in struct ipt_tcp. */
#define XT_TCP_INV_SRCPT	0x01	/* Invert the sense of source ports. */
#define XT_TCP_INV_DSTPT	0x02	/* Invert the sense of dest ports. */
#define XT_TCP_INV_FLAGS	0x04	/* Invert the sense of TCP flags. */
#define XT_TCP_INV_OPTION	0x08	/* Invert the sense of option test. */
#define XT_TCP_INV_MASK		0x0F	/* All possible flags. */

/* UDP matching stuff */
struct xt_udp
{
	u_int16_t spts[2];			/* Source port range. */
	u_int16_t dpts[2];			/* Destination port range. */
	u_int8_t invflags;			/* Inverse flags */
};

/* Values for "invflags" field in struct ipt_udp. */
#define XT_UDP_INV_SRCPT	0x01	/* Invert the sense of source ports. */
#define XT_UDP_INV_DSTPT	0x02	/* Invert the sense of dest ports. */
#define XT_UDP_INV_MASK	0x03	/* All possible flags. */


#endif
