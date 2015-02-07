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
 * Pixart PAC207BCA / PAC73xx common functions
 *
 * Copyright (C) 2008 Hans de Goede <j.w.r.degoede@hhs.nl>
 * Copyright (C) 2005 Thomas Kaiser thomas@kaiser-linux.li
 * Copyleft (C) 2005 Michel Xhaard mxhaard@magic.fr
 *
 * V4L2 by Jean-Francois Moine <http://moinejf.free.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* We calculate the autogain at the end of the transfer of a frame, at this
   moment a frame with the old settings is being transmitted, and a frame is
   being captured with the old settings. So if we adjust the autogain we must
   ignore atleast the 2 next frames for the new settings to come into effect
   before doing any other adjustments */
#define PAC_AUTOGAIN_IGNORE_FRAMES	3

static const unsigned char pac_sof_marker[5] =
		{ 0xff, 0xff, 0x00, 0xff, 0x96 };

static unsigned char *pac_find_sof(struct gspca_dev *gspca_dev,
					unsigned char *m, int len)
{
	struct sd *sd = (struct sd *) gspca_dev;
	int i;

	/* Search for the SOF marker (fixed part) in the header */
	for (i = 0; i < len; i++) {
		if (m[i] == pac_sof_marker[sd->sof_read]) {
			sd->sof_read++;
			if (sd->sof_read == sizeof(pac_sof_marker)) {
				PDEBUG(D_FRAM,
					"SOF found, bytes to analyze: %u."
					" Frame starts at byte #%u",
					len, i + 1);
				sd->sof_read = 0;
				return m + i + 1;
			}
		} else {
			sd->sof_read = 0;
		}
	}

	return NULL;
}
