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

/* SCTP kernel reference Implementation
 * Copyright (c) 1999-2001 Motorola, Inc.
 * Copyright (c) 2001-2003 International Business Machines, Corp.
 *
 * This file is part of the SCTP kernel reference Implementation
 *
 * SCTP Checksum functions
 *
 * The SCTP reference implementation is free software;
 * you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * The SCTP reference implementation is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *                 ************************
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU CC; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Please send any bug reports or fixes you make to the
 * email address(es):
 *    lksctp developers <lksctp-developers@lists.sourceforge.net>
 *
 * Or submit a bug report through the following website:
 *    http://www.sf.net/projects/lksctp
 *
 * Written or modified by:
 *    Dinakaran Joseph
 *    Jon Grimm <jgrimm@us.ibm.com>
 *    Sridhar Samudrala <sri@us.ibm.com>
 *
 * Rewritten to use libcrc32c by:
 *    Vlad Yasevich <vladislav.yasevich@hp.com>
 *
 * Any bugs reported given to us we will try to fix... any fixes shared will
 * be incorporated into the next SCTP release.
 */

#include <linux/types.h>
#include <net/sctp/sctp.h>
#include <linux/crc32c.h>

static inline __be32 sctp_crc32c(__be32 crc, u8 *buffer, u16 length)
{
	return (__force __be32)crc32c((__force u32)crc, buffer, length);
}

static inline __be32 sctp_start_cksum(__u8 *buffer, __u16 length)
{
	__be32 crc = ~cpu_to_be32(0);
	__u8  zero[sizeof(__u32)] = {0};

	/* Optimize this routine to be SCTP specific, knowing how
	 * to skip the checksum field of the SCTP header.
	 */

	/* Calculate CRC up to the checksum. */
	crc = sctp_crc32c(crc, buffer, sizeof(struct sctphdr) - sizeof(__u32));

	/* Skip checksum field of the header. */
	crc = sctp_crc32c(crc, zero, sizeof(__u32));

	/* Calculate the rest of the CRC. */
	crc = sctp_crc32c(crc, &buffer[sizeof(struct sctphdr)],
			    length - sizeof(struct sctphdr));
	return crc;
}

static inline __be32 sctp_update_cksum(__u8 *buffer, __u16 length, __be32 crc32)
{
	return sctp_crc32c(crc32, buffer, length);
}

static inline __be32 sctp_end_cksum(__be32 crc32)
{
	return (__force __be32)~cpu_to_le32((__force u32)crc32);
}
