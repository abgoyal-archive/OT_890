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
 *    Disk Array driver for Compaq SMART2 Controllers
 *    Copyright 1998 Compaq Computer Corporation
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *    NON INFRINGEMENT.  See the GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *    Questions/Comments/Bugfixes to iss_storagedev@hp.com
 *
 */
#ifndef IDA_IOCTL_H
#define IDA_IOCTL_H

#include "ida_cmd.h"
#include "cpqarray.h"

#define IDAGETDRVINFO		0x27272828
#define IDAPASSTHRU		0x28282929
#define IDAGETCTLRSIG		0x29293030
#define IDAREVALIDATEVOLS	0x30303131
#define IDADRIVERVERSION	0x31313232
#define IDAGETPCIINFO		0x32323333

typedef struct _ida_pci_info_struct
{
	unsigned char 	bus;
	unsigned char 	dev_fn;
	__u32 		board_id;
} ida_pci_info_struct;
/*
 * Normally, the ioctl determines the logical unit for this command by
 * the major,minor number of the fd passed to ioctl.  If you need to send
 * a command to a different/nonexistant unit (such as during config), you
 * can override the normal behavior by setting the unit valid bit. (Normally,
 * it should be zero) The controller the command is sent to is still
 * determined by the major number of the open device.
 */

#define UNITVALID	0x80
typedef struct {
	__u8	cmd;
	__u8	rcode;
	__u8	unit;
	__u32	blk;
	__u16	blk_cnt;

/* currently, sg_cnt is assumed to be 1: only the 0th element of sg is used */
	struct {
		void	__user *addr;
		size_t	size;
	} sg[SG_MAX];
	int	sg_cnt;

	union ctlr_cmds {
		drv_info_t		drv;
		unsigned char		buf[1024];

		id_ctlr_t		id_ctlr;
		drv_param_t		drv_param;
		id_log_drv_t		id_log_drv;
		id_log_drv_ext_t	id_log_drv_ext;
		sense_log_drv_stat_t	sense_log_drv_stat;
		id_phys_drv_t		id_phys_drv;
		blink_drv_leds_t	blink_drv_leds;
		sense_blink_leds_t	sense_blink_leds;
		config_t		config;
		reorder_log_drv_t	reorder_log_drv;
		label_log_drv_t		label_log_drv;
		surf_delay_t		surf_delay;
		overhead_delay_t	overhead_delay;
		mp_delay_t		mp_delay;
		scsi_param_t		scsi_param;
	} c;
} ida_ioctl_t;

#endif /* IDA_IOCTL_H */
