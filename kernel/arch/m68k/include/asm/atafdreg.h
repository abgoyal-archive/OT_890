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

#ifndef _LINUX_FDREG_H
#define _LINUX_FDREG_H

/*
** WD1772 stuff
 */

/* register codes */

#define FDCSELREG_STP   (0x80)   /* command/status register */
#define FDCSELREG_TRA   (0x82)   /* track register */
#define FDCSELREG_SEC   (0x84)   /* sector register */
#define FDCSELREG_DTA   (0x86)   /* data register */

/* register names for FDC_READ/WRITE macros */

#define FDCREG_CMD		0
#define FDCREG_STATUS	0
#define FDCREG_TRACK	2
#define FDCREG_SECTOR	4
#define FDCREG_DATA		6

/* command opcodes */

#define FDCCMD_RESTORE  (0x00)   /*  -                   */
#define FDCCMD_SEEK     (0x10)   /*   |                  */
#define FDCCMD_STEP     (0x20)   /*   |  TYP 1 Commands  */
#define FDCCMD_STIN     (0x40)   /*   |                  */
#define FDCCMD_STOT     (0x60)   /*  -                   */
#define FDCCMD_RDSEC    (0x80)   /*  -   TYP 2 Commands  */
#define FDCCMD_WRSEC    (0xa0)   /*  -          "        */
#define FDCCMD_RDADR    (0xc0)   /*  -                   */
#define FDCCMD_RDTRA    (0xe0)   /*   |  TYP 3 Commands  */
#define FDCCMD_WRTRA    (0xf0)   /*  -                   */
#define FDCCMD_FORCI    (0xd0)   /*  -   TYP 4 Command   */

/* command modifier bits */

#define FDCCMDADD_SR6   (0x00)   /* step rate settings */
#define FDCCMDADD_SR12  (0x01)
#define FDCCMDADD_SR2   (0x02)
#define FDCCMDADD_SR3   (0x03)
#define FDCCMDADD_V     (0x04)   /* verify */
#define FDCCMDADD_H     (0x08)   /* wait for spin-up */
#define FDCCMDADD_U     (0x10)   /* update track register */
#define FDCCMDADD_M     (0x10)   /* multiple sector access */
#define FDCCMDADD_E     (0x04)   /* head settling flag */
#define FDCCMDADD_P     (0x02)   /* precompensation off */
#define FDCCMDADD_A0    (0x01)   /* DAM flag */

/* status register bits */

#define	FDCSTAT_MOTORON	(0x80)   /* motor on */
#define	FDCSTAT_WPROT	(0x40)   /* write protected (FDCCMD_WR*) */
#define	FDCSTAT_SPINUP	(0x20)   /* motor speed stable (Type I) */
#define	FDCSTAT_DELDAM	(0x20)   /* sector has deleted DAM (Type II+III) */
#define	FDCSTAT_RECNF	(0x10)   /* record not found */
#define	FDCSTAT_CRC		(0x08)   /* CRC error */
#define	FDCSTAT_TR00	(0x04)   /* Track 00 flag (Type I) */
#define	FDCSTAT_LOST	(0x04)   /* Lost Data (Type II+III) */
#define	FDCSTAT_IDX		(0x02)   /* Index status (Type I) */
#define	FDCSTAT_DRQ		(0x02)   /* DRQ status (Type II+III) */
#define	FDCSTAT_BUSY	(0x01)   /* FDC is busy */


/* PSG Port A Bit Nr 0 .. Side Sel .. 0 -> Side 1  1 -> Side 2 */
#define DSKSIDE     (0x01)

#define DSKDRVNONE  (0x06)
#define DSKDRV0     (0x02)
#define DSKDRV1     (0x04)

/* step rates */
#define	FDCSTEP_6	0x00
#define	FDCSTEP_12	0x01
#define	FDCSTEP_2	0x02
#define	FDCSTEP_3	0x03

#endif
