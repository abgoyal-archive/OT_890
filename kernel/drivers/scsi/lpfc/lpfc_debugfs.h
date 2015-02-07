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

/*******************************************************************
 * This file is part of the Emulex Linux Device Driver for         *
 * Fibre Channel Host Bus Adapters.                                *
 * Copyright (C) 2007 Emulex.  All rights reserved.                *
 * EMULEX and SLI are trademarks of Emulex.                        *
 * www.emulex.com                                                  *
 *                                                                 *
 * This program is free software; you can redistribute it and/or   *
 * modify it under the terms of version 2 of the GNU General       *
 * Public License as published by the Free Software Foundation.    *
 * This program is distributed in the hope that it will be useful. *
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND          *
 * WARRANTIES, INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,  *
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT, ARE      *
 * DISCLAIMED, EXCEPT TO THE EXTENT THAT SUCH DISCLAIMERS ARE HELD *
 * TO BE LEGALLY INVALID.  See the GNU General Public License for  *
 * more details, a copy of which can be found in the file COPYING  *
 * included with this package.                                     *
 *******************************************************************/

#ifndef _H_LPFC_DEBUG_FS
#define _H_LPFC_DEBUG_FS

#ifdef CONFIG_SCSI_LPFC_DEBUG_FS
struct lpfc_debugfs_trc {
	char *fmt;
	uint32_t data1;
	uint32_t data2;
	uint32_t data3;
	uint32_t seq_cnt;
	unsigned long jif;
};
#endif

/* Mask for discovery_trace */
#define LPFC_DISC_TRC_ELS_CMD		0x1	/* Trace ELS commands */
#define LPFC_DISC_TRC_ELS_RSP		0x2	/* Trace ELS response */
#define LPFC_DISC_TRC_ELS_UNSOL		0x4	/* Trace ELS rcv'ed   */
#define LPFC_DISC_TRC_ELS_ALL		0x7	/* Trace ELS */
#define LPFC_DISC_TRC_MBOX_VPORT	0x8	/* Trace vport MBOXs */
#define LPFC_DISC_TRC_MBOX		0x10	/* Trace other MBOXs */
#define LPFC_DISC_TRC_MBOX_ALL		0x18	/* Trace all MBOXs */
#define LPFC_DISC_TRC_CT		0x20	/* Trace disc CT requests */
#define LPFC_DISC_TRC_DSM		0x40    /* Trace DSM events */
#define LPFC_DISC_TRC_RPORT		0x80    /* Trace rport events */
#define LPFC_DISC_TRC_NODE		0x100   /* Trace ndlp state changes */

#define LPFC_DISC_TRC_DISCOVERY		0xef    /* common mask for general
						 * discovery */
#endif /* H_LPFC_DEBUG_FS */
