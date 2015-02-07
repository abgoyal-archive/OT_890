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


/*  Driver for the Iomega MatchMaker parallel port SCSI HBA embedded in 
 * the Iomega ZIP Plus drive
 * 
 * (c) 1998     David Campbell
 *
 * Please note that I live in Perth, Western Australia. GMT+0800
 */

#ifndef _IMM_H
#define _IMM_H

#define   IMM_VERSION   "2.05 (for Linux 2.4.0)"

/* 
 * 10 Apr 1998 (Good Friday) - Received EN144302 by email from Iomega.
 * Scarry thing is the level of support from one of their managers.
 * The onus is now on us (the developers) to shut up and start coding.
 *                                              11Apr98 [ 0.10 ]
 *
 * --- SNIP ---
 *
 * It manages to find the drive which is a good start. Writing data during
 * data phase is known to be broken (due to requirements of two byte writes).
 * Removing "Phase" debug messages.
 *
 * PS: Took four hours of coding after I bought a drive.
 *      ANZAC Day (Aus "War Veterans Holiday")  25Apr98 [ 0.14 ]
 *
 * Ten minutes later after a few fixes.... (LITERALLY!!!)
 * Have mounted disk, copied file, dismounted disk, remount disk, diff file
 *                    -----  It actually works!!! -----
 *                                              25Apr98 [ 0.15 ]
 *
 * Twenty minutes of mucking around, rearanged the IEEE negotiate mechanism.
 * Now have byte mode working (only EPP and ECP to go now... :=)
 *                                              26Apr98 [ 0.16 ]
 *
 * Thirty minutes of further coding results in EPP working on my machine.
 *                                              27Apr98 [ 0.17 ]
 *
 * Due to work commitments and inability to get a "true" ECP mode functioning
 * I have decided to code the parport support into imm.
 *                                              09Jun98 [ 0.18 ]
 *
 * Driver is now out of beta testing.
 * Support for parport has been added.
 * Now distributed with the ppa driver.
 *                                              12Jun98 [ 2.00 ]
 *
 * Err.. It appears that imm-2.00 was broken....
 *                                              18Jun98 [ 2.01 ]
 *
 * Patch applied to sync this against the Linux 2.1.x kernel code
 * Included qboot_zip.sh
 *                                              21Jun98 [ 2.02 ]
 *
 * Other clean ups include the follow changes:
 *    CONFIG_SCSI_PPA_HAVE_PEDANTIC => CONFIG_SCSI_IZIP_EPP16
 *    added CONFIG_SCSI_IZIP_SLOW_CTR option
 *                                                      [2.03]
 *  Fix kernel panic on scsi timeout.		20Aug00 [2.04]
 *
 *  Avoid io_request_lock problems.
 *  John Cavan <johncavan@home.com>		16Nov00 [2.05]
 */
/* ------ END OF USER CONFIGURABLE PARAMETERS ----- */

#include  <linux/stddef.h>
#include  <linux/module.h>
#include  <linux/kernel.h>
#include  <linux/ioport.h>
#include  <linux/delay.h>
#include  <linux/proc_fs.h>
#include  <linux/stat.h>
#include  <linux/blkdev.h>
#include  <linux/sched.h>
#include  <linux/interrupt.h>

#include  <asm/io.h>
#include  <scsi/scsi_host.h>
/* batteries not included :-) */

/*
 * modes in which the driver can operate 
 */
#define   IMM_AUTODETECT        0	/* Autodetect mode                */
#define   IMM_NIBBLE            1	/* work in standard 4 bit mode    */
#define   IMM_PS2               2	/* PS/2 byte mode         */
#define   IMM_EPP_8             3	/* EPP mode, 8 bit                */
#define   IMM_EPP_16            4	/* EPP mode, 16 bit               */
#define   IMM_EPP_32            5	/* EPP mode, 32 bit               */
#define   IMM_UNKNOWN           6	/* Just in case...                */

static char *IMM_MODE_STRING[] =
{
	[IMM_AUTODETECT] = "Autodetect",
	[IMM_NIBBLE]	 = "SPP",
	[IMM_PS2]	 = "PS/2",
	[IMM_EPP_8]	 = "EPP 8 bit",
	[IMM_EPP_16]	 = "EPP 16 bit",
#ifdef CONFIG_SCSI_IZIP_EPP16
	[IMM_EPP_32]	 = "EPP 16 bit",
#else
	[IMM_EPP_32]	 = "EPP 32 bit",
#endif
	[IMM_UNKNOWN]	 = "Unknown",
};

/* other options */
#define IMM_BURST_SIZE	512	/* data burst size */
#define IMM_SELECT_TMO  500	/* 500 how long to wait for target ? */
#define IMM_SPIN_TMO    5000	/* 50000 imm_wait loop limiter */
#define IMM_DEBUG	0	/* debugging option */
#define IN_EPP_MODE(x) (x == IMM_EPP_8 || x == IMM_EPP_16 || x == IMM_EPP_32)

/* args to imm_connect */
#define CONNECT_EPP_MAYBE 1
#define CONNECT_NORMAL  0

#define r_dtr(x)        (unsigned char)inb((x))
#define r_str(x)        (unsigned char)inb((x)+1)
#define r_ctr(x)        (unsigned char)inb((x)+2)
#define r_epp(x)        (unsigned char)inb((x)+4)
#define r_fifo(x)       (unsigned char)inb((x))   /* x must be base_hi */
					/* On PCI is: base+0x400 != base_hi */
#define r_ecr(x)        (unsigned char)inb((x)+2) /* x must be base_hi */

#define w_dtr(x,y)      outb(y, (x))
#define w_str(x,y)      outb(y, (x)+1)
#define w_epp(x,y)      outb(y, (x)+4)
#define w_fifo(x,y)     outb(y, (x))     /* x must be base_hi */
#define w_ecr(x,y)      outb(y, (x)+0x2) /* x must be base_hi */

#ifdef CONFIG_SCSI_IZIP_SLOW_CTR
#define w_ctr(x,y)      outb_p(y, (x)+2)
#else
#define w_ctr(x,y)      outb(y, (x)+2)
#endif

static int imm_engine(imm_struct *, struct scsi_cmnd *);

#endif				/* _IMM_H */
