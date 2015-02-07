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

#ifndef _ATP870U_H
#define _ATP870U_H

#include <linux/types.h>
#include <linux/kdev_t.h>

/* I/O Port */

#define MAX_CDB 	12
#define MAX_SENSE 	14
#define qcnt	       	32
#define ATP870U_SCATTER 	128
#define ATP870U_CMDLUN  	1

#define MAX_ADAPTER	8
#define MAX_SCSI_ID	16
#define ATP870U_MAX_SECTORS 128

#define ATP885_DEVID 0x808A
#define ATP880_DEVID1 0x8080
#define ATP880_DEVID2 0x8081

//#define ED_DBGP

struct atp_unit
{
	unsigned long baseport;
	unsigned long ioport[2];
	unsigned long pciport[2];
	unsigned long irq;
	unsigned char last_cmd[2];
	unsigned char in_snd[2];
	unsigned char in_int[2];
	unsigned char quhd[2];
	unsigned char quend[2];
	unsigned char global_map[2];
	unsigned char chip_ver;
	unsigned char scam_on;
	unsigned char host_id[2];
	unsigned int working[2];
	unsigned short wide_id[2];
	unsigned short active_id[2];
	unsigned short ultra_map[2];
	unsigned short async[2];
	unsigned short dev_id;
	unsigned char sp[2][16];
	unsigned char r1f[2][16];		
	struct scsi_cmnd *quereq[2][qcnt];
	struct atp_id
	{
		unsigned char dirct;
		unsigned char devsp;
		unsigned char devtype;
		unsigned long tran_len;
		unsigned long last_len;
		unsigned char *prd_pos;
		unsigned char *prd_table;	/* Kernel address of PRD table */
		dma_addr_t prd_bus;		/* Bus address of PRD */
		dma_addr_t prdaddr;		/* Dynamically updated in driver */
		struct scsi_cmnd *curr_req;
	} id[2][16];
    	struct Scsi_Host *host;
    	struct pci_dev *pdev;
	unsigned int unit;
};

#endif
