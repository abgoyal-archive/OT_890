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
 * Copyright 1998-2008 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2008 S3 Graphics, Inc. All Rights Reserved.

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTIES OR REPRESENTATIONS; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.See the GNU General Public License
 * for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "global.h"

int viafb_ioctl_get_viafb_info(u_long arg)
{
	struct viafb_ioctl_info viainfo;

	viainfo.viafb_id = VIAID;
	viainfo.vendor_id = PCI_VIA_VENDOR_ID;

	switch (viaparinfo->chip_info->gfx_chip_name) {
	case UNICHROME_CLE266:
		viainfo.device_id = UNICHROME_CLE266_DID;
		break;

	case UNICHROME_K400:
		viainfo.device_id = UNICHROME_K400_DID;
		break;

	case UNICHROME_K800:
		viainfo.device_id = UNICHROME_K800_DID;
		break;

	case UNICHROME_PM800:
		viainfo.device_id = UNICHROME_PM800_DID;
		break;

	case UNICHROME_CN700:
		viainfo.device_id = UNICHROME_CN700_DID;
		break;

	case UNICHROME_CX700:
		viainfo.device_id = UNICHROME_CX700_DID;
		break;

	case UNICHROME_K8M890:
		viainfo.device_id = UNICHROME_K8M890_DID;
		break;

	case UNICHROME_P4M890:
		viainfo.device_id = UNICHROME_P4M890_DID;
		break;

	case UNICHROME_P4M900:
		viainfo.device_id = UNICHROME_P4M900_DID;
		break;
	}

	viainfo.version = VERSION_MAJOR;
	viainfo.revision = VERSION_MINOR;

	if (copy_to_user((void __user *)arg, &viainfo, sizeof(viainfo)))
		return -EFAULT;

	return 0;
}

/* Hot-Plug Priority: DVI > CRT*/
int viafb_ioctl_hotplug(int hres, int vres, int bpp)
{
	int DVIsense, status = 0;
	DEBUG_MSG(KERN_INFO "viafb_ioctl_hotplug!!\n");

	if (viaparinfo->chip_info->tmds_chip_info.tmds_chip_name !=
		NON_TMDS_TRANSMITTER) {
		DVIsense = viafb_dvi_sense();

		if (DVIsense) {
			DEBUG_MSG(KERN_INFO "DVI Attached...\n");
			if (viafb_DeviceStatus != DVI_Device) {
				viafb_DVI_ON = 1;
				viafb_CRT_ON = 0;
				viafb_LCD_ON = 0;
				viafb_DeviceStatus = DVI_Device;
				return viafb_DeviceStatus;
			}
			status = 1;
		} else
			DEBUG_MSG(KERN_INFO "DVI De-attached...\n");
	}

	if ((viafb_DeviceStatus != CRT_Device) && (status == 0)) {
		viafb_CRT_ON = 1;
		viafb_DVI_ON = 0;
		viafb_LCD_ON = 0;

		viafb_DeviceStatus = CRT_Device;
		return viafb_DeviceStatus;
	}

	return 0;
}
