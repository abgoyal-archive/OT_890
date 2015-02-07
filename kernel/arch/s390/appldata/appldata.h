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
 * arch/s390/appldata/appldata.h
 *
 * Definitions and interface for Linux - z/VM Monitor Stream.
 *
 * Copyright IBM Corp. 2003, 2008
 *
 * Author: Gerald Schaefer <gerald.schaefer@de.ibm.com>
 */

#define APPLDATA_MAX_REC_SIZE	  4024	/* Maximum size of the */
					/* data buffer */
#define APPLDATA_MAX_PROCS 100

#define APPLDATA_PROC_NAME_LENGTH 16	/* Max. length of /proc name */

#define APPLDATA_RECORD_MEM_ID		0x01	/* IDs to identify the */
#define APPLDATA_RECORD_OS_ID		0x02	/* individual records, */
#define APPLDATA_RECORD_NET_SUM_ID	0x03	/* must be < 256 !     */
#define APPLDATA_RECORD_PROC_ID		0x04

#define CTL_APPLDATA_TIMER 	2121	/* sysctl IDs, must be unique */
#define CTL_APPLDATA_INTERVAL 	2122
#define CTL_APPLDATA_MEM	2123
#define CTL_APPLDATA_OS		2124
#define CTL_APPLDATA_NET_SUM	2125
#define CTL_APPLDATA_PROC	2126

struct appldata_ops {
	struct list_head list;
	struct ctl_table_header *sysctl_header;
	struct ctl_table *ctl_table;
	int    active;				/* monitoring status */

	/* fill in from here */
	char name[APPLDATA_PROC_NAME_LENGTH];	/* name of /proc fs node */
	unsigned char record_nr;		/* Record Nr. for Product ID */
	void (*callback)(void *data);		/* callback function */
	void *data;				/* record data */
	unsigned int size;			/* size of record */
	struct module *owner;			/* THIS_MODULE */
	char mod_lvl[2];			/* modification level, EBCDIC */
};

extern int appldata_register_ops(struct appldata_ops *ops);
extern void appldata_unregister_ops(struct appldata_ops *ops);
extern int appldata_diag(char record_nr, u16 function, unsigned long buffer,
			 u16 length, char *mod_lvl);

