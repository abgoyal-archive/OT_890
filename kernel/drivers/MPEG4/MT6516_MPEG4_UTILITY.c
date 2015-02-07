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

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 *
 * Filename:
 * ---------
 * mp4_utility.cpp
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   mp4 codec utility (WM version)
 *
 * Author:
 * -------
 * mtk01845 (Port from Nucleus, provided by mtk00796)
 *
 *------------------------------------------------------------------------------
 * $Revision$
 * $Modtime:$
 * $Log:$
 *
 * Dec 10 2008 mtk01845
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 * add to source control recursely
 *
 * Apr 18 2008 mtk01845
 * [DUMA00000073] [Drv][Video] Video driver first check-in
 * 
 *
 * Apr 14 2008 mtk01845
 * [DUMA00000073] [Drv][Video] Video driver first check-in
 * add to source control recursely
 *
 *******************************************************************************/

#include "MT6516_MPEG4_UTILITY.h"

///////////////////////////////////////////
//             mp4 utilities
//

//UINT32 MP4_base;

#define MAX_MP4_WARNING  16
UINT32 mp4_warning_line[MAX_MP4_WARNING];
void mp4_warning(UINT32 line)
{
	static UINT32 mp4_warning_index = 0;
	mp4_warning_line[mp4_warning_index&(MAX_MP4_WARNING - 1)] = line;
	mp4_warning_index++;
}


INT32 mp4_util_show_bits(UINT8 * data, INT32 bitcnt, INT32 num)
{
	INT32 tmp, out, tmp1;

	tmp = (bitcnt & 0x7) + num;

	if (tmp <= 8)
		out = (data[bitcnt >> 3] >> (8 - tmp)) & ((1 << num) - 1);
	else
	{
		out = data[bitcnt >> 3]&((1 << (8 - (bitcnt & 0x7))) - 1);

		tmp -= 8;
		bitcnt += (8 - (bitcnt & 0x7));

		while (tmp > 8)
		{
			out = (out << 8) + data[bitcnt >> 3];

			tmp -= 8;
			bitcnt += 8;
		}

		tmp1 = (data[bitcnt >> 3] >> (8 - tmp)) & ((1 << tmp) - 1);
		out = (out << tmp) + tmp1;
	}

	return out;
}


INT32 mp4_util_get_bits(UINT8 * data, INT32 *bitcnt, INT32 num)
{
	UINT32 ret;
	
	ret = mp4_util_show_bits(data,*bitcnt,num);
	(*bitcnt)+=num;

	return ret;
}


INT32 mp4_util_show_word(UINT8 * a)
{
	return ((a[0] << 24) + (a[1] << 16) + (a[2] << 8) + a[3]);
}


INT32 mp4_util_log2ceil(INT32 arg)
{
	INT32 j = 0, i = 1;

	while (arg > i)
	{
		i *= 2;
		j++;
	}

	if (j == 0)
		j = 1;

	return j;
}


INT32 mp4_util_user_data(UINT8 * data, INT32 * bitcnt, UINT32 max_parse_data_size)
{
	INT32 bit = 0;

	*bitcnt += 32;

	while (mp4_util_show_bits(data, *bitcnt + bit, 24) != 1)
	{
		bit += 8;
		if ((UINT32)bit > max_parse_data_size)
		{
			break;
		}   
	}

	*bitcnt += bit;

	return 0;
}


void mp4_putbits(UINT8 * in, INT32 * bitcnt, INT32 data, INT32 data_length)
{
	UINT8 *temp, *temp1;

	INT32 t, count, count2;
	t = *bitcnt;
	temp = in;

	if ((t & 0x7) == 0)
		temp[t >> 3] = 0;

	if ((t & 0x7) + data_length <= 8)
	{
		temp[(t >> 3)] |= (data << (8 - ((t & 0x7) + data_length)));
	}
	else
	{
		count = (t & 7) + data_length;
		
		temp1 = &temp[t >> 3];
		*temp1 |= (data >> (data_length - 8 + (t & 7)));
		count2 = count - 16;
		temp1++;
		
		while (count2 >= 0)
		{
			*temp1 = (data >> count2) & 0xFF;
			
			temp1++;
			count2 -= 8;
		}
		*temp1 = (data&((1 << (count % 8)) - 1)) << ((8 - count % 8));
	}

	*bitcnt += data_length;
}



