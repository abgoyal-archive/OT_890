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

/* Get frame buffer size from VGA BIOS */

unsigned int viafb_get_memsize(void)
{
	unsigned int m;

	/* If memory size provided by user */
	if (viafb_memsize)
		m = viafb_memsize * Mb;
	else {
		m = (unsigned int)viafb_read_reg(VIASR, SR39);
		m = m * (4 * Mb);

		if ((m < (16 * Mb)) || (m > (64 * Mb)))
			m = 16 * Mb;
	}
	DEBUG_MSG(KERN_INFO "framebuffer size = %d Mb\n", m / Mb);
	return m;
}

/* Get Video Buffer Starting Physical Address(back door)*/

unsigned long viafb_get_videobuf_addr(void)
{
	struct pci_dev *pdev = NULL;
	unsigned char sys_mem;
	unsigned char video_mem;
	unsigned long sys_mem_size;
	unsigned long video_mem_size;
	/*system memory = 256 MB, video memory 64 MB */
	unsigned long vmem_starting_adr = 0x0C000000;

	pdev =
	    (struct pci_dev *)pci_get_device(VIA_K800_BRIDGE_VID,
					     VIA_K800_BRIDGE_DID, NULL);
	if (pdev != NULL) {
		pci_read_config_byte(pdev, VIA_K800_SYSTEM_MEMORY_REG,
				     &sys_mem);
		pci_read_config_byte(pdev, VIA_K800_VIDEO_MEMORY_REG,
				     &video_mem);
		video_mem = (video_mem & 0x70) >> 4;
		sys_mem_size = ((unsigned long)sys_mem) << 24;
		if (video_mem != 0)
			video_mem_size = (1 << (video_mem)) * 1024 * 1024;
		else
			video_mem_size = 0;

		vmem_starting_adr = sys_mem_size - video_mem_size;
		pci_dev_put(pdev);
	}

	DEBUG_MSG(KERN_INFO "Video Memory Starting Address = %lx \n",
		  vmem_starting_adr);
	return vmem_starting_adr;
}
