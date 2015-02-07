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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 - 1997, 2000-2004 Silicon Graphics, Inc. All rights reserved.
 */

#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <asm/sn/types.h>
#include <asm/sn/module.h>
#include <asm/sn/l1.h>

char brick_types[MAX_BRICK_TYPES + 1] = "cri.xdpn%#=vo^kjbf890123456789...";
/*
 * Format a module id for printing.
 *
 * There are three possible formats:
 *
 *   MODULE_FORMAT_BRIEF	is the brief 6-character format, including
 *				the actual brick-type as recorded in the 
 *				moduleid_t, eg. 002c15 for a C-brick, or
 *				101#17 for a PX-brick.
 *
 *   MODULE_FORMAT_LONG		is the hwgraph format, eg. rack/002/bay/15
 *				of rack/101/bay/17 (note that the brick
 *				type does not appear in this format).
 *
 *   MODULE_FORMAT_LCD		is like MODULE_FORMAT_BRIEF, except that it
 *				ensures that the module id provided appears
 *				exactly as it would on the LCD display of
 *				the corresponding brick, eg. still 002c15
 *				for a C-brick, but 101p17 for a PX-brick.
 *
 * maule (9/13/04):  Removed top-level check for (fmt == MODULE_FORMAT_LCD)
 * making MODULE_FORMAT_LCD equivalent to MODULE_FORMAT_BRIEF.  It was
 * decided that all callers should assume the returned string should be what
 * is displayed on the brick L1 LCD.
 */
void
format_module_id(char *buffer, moduleid_t m, int fmt)
{
	int rack, position;
	unsigned char brickchar;

	rack = MODULE_GET_RACK(m);
	brickchar = MODULE_GET_BTCHAR(m);

	/* Be sure we use the same brick type character as displayed
	 * on the brick's LCD
	 */
	switch (brickchar) 
	{
	case L1_BRICKTYPE_GA:
	case L1_BRICKTYPE_OPUS_TIO:
		brickchar = L1_BRICKTYPE_C;
		break;

	case L1_BRICKTYPE_PX:
	case L1_BRICKTYPE_PE:
	case L1_BRICKTYPE_PA:
	case L1_BRICKTYPE_SA: /* we can move this to the "I's" later
			       * if that makes more sense
			       */
		brickchar = L1_BRICKTYPE_P;
		break;

	case L1_BRICKTYPE_IX:
	case L1_BRICKTYPE_IA:

		brickchar = L1_BRICKTYPE_I;
		break;
	}

	position = MODULE_GET_BPOS(m);

	if ((fmt == MODULE_FORMAT_BRIEF) || (fmt == MODULE_FORMAT_LCD)) {
		/* Brief module number format, eg. 002c15 */

		/* Decompress the rack number */
		*buffer++ = '0' + RACK_GET_CLASS(rack);
		*buffer++ = '0' + RACK_GET_GROUP(rack);
		*buffer++ = '0' + RACK_GET_NUM(rack);

		/* Add the brick type */
		*buffer++ = brickchar;
	}
	else if (fmt == MODULE_FORMAT_LONG) {
		/* Fuller hwgraph format, eg. rack/002/bay/15 */

		strcpy(buffer, "rack" "/");  buffer += strlen(buffer);

		*buffer++ = '0' + RACK_GET_CLASS(rack);
		*buffer++ = '0' + RACK_GET_GROUP(rack);
		*buffer++ = '0' + RACK_GET_NUM(rack);

		strcpy(buffer, "/" "bay" "/");  buffer += strlen(buffer);
	}

	/* Add the bay position, using at least two digits */
	if (position < 10)
		*buffer++ = '0';
	sprintf(buffer, "%d", position);
}
