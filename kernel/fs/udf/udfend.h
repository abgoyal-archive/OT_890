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

#ifndef __UDF_ENDIAN_H
#define __UDF_ENDIAN_H

#include <asm/byteorder.h>
#include <linux/string.h>

static inline kernel_lb_addr lelb_to_cpu(lb_addr in)
{
	kernel_lb_addr out;

	out.logicalBlockNum = le32_to_cpu(in.logicalBlockNum);
	out.partitionReferenceNum = le16_to_cpu(in.partitionReferenceNum);

	return out;
}

static inline lb_addr cpu_to_lelb(kernel_lb_addr in)
{
	lb_addr out;

	out.logicalBlockNum = cpu_to_le32(in.logicalBlockNum);
	out.partitionReferenceNum = cpu_to_le16(in.partitionReferenceNum);

	return out;
}

static inline short_ad lesa_to_cpu(short_ad in)
{
	short_ad out;

	out.extLength = le32_to_cpu(in.extLength);
	out.extPosition = le32_to_cpu(in.extPosition);

	return out;
}

static inline short_ad cpu_to_lesa(short_ad in)
{
	short_ad out;

	out.extLength = cpu_to_le32(in.extLength);
	out.extPosition = cpu_to_le32(in.extPosition);

	return out;
}

static inline kernel_long_ad lela_to_cpu(long_ad in)
{
	kernel_long_ad out;

	out.extLength = le32_to_cpu(in.extLength);
	out.extLocation = lelb_to_cpu(in.extLocation);

	return out;
}

static inline long_ad cpu_to_lela(kernel_long_ad in)
{
	long_ad out;

	out.extLength = cpu_to_le32(in.extLength);
	out.extLocation = cpu_to_lelb(in.extLocation);

	return out;
}

static inline kernel_extent_ad leea_to_cpu(extent_ad in)
{
	kernel_extent_ad out;

	out.extLength = le32_to_cpu(in.extLength);
	out.extLocation = le32_to_cpu(in.extLocation);

	return out;
}

#endif /* __UDF_ENDIAN_H */
