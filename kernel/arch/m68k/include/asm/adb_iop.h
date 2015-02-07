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
 * ADB through the IOP
 * Written by Joshua M. Thompson
 */

/* IOP number and channel number for ADB */

#define ADB_IOP		IOP_NUM_ISM
#define ADB_CHAN	2

/* From the A/UX headers...maybe important, maybe not */

#define ADB_IOP_LISTEN	0x01
#define ADB_IOP_TALK	0x02
#define ADB_IOP_EXISTS	0x04
#define ADB_IOP_FLUSH	0x08
#define ADB_IOP_RESET	0x10
#define ADB_IOP_INT	0x20
#define ADB_IOP_POLL	0x40
#define ADB_IOP_UNINT	0x80

#define AIF_RESET	0x00
#define AIF_FLUSH	0x01
#define AIF_LISTEN	0x08
#define AIF_TALK	0x0C

/* Flag bits in struct adb_iopmsg */

#define ADB_IOP_EXPLICIT	0x80	/* nonzero if explicit command */
#define ADB_IOP_AUTOPOLL	0x40	/* auto/SRQ polling enabled    */
#define ADB_IOP_SRQ		0x04	/* SRQ detected                */
#define ADB_IOP_TIMEOUT		0x02	/* nonzero if timeout          */

#ifndef __ASSEMBLY__

struct adb_iopmsg {
	__u8 flags;		/* ADB flags         */
	__u8 count;		/* no. of data bytes */
	__u8 cmd;		/* ADB command       */
	__u8 data[8];		/* ADB data          */
	__u8 spare[21];		/* spare             */
};

#endif /* __ASSEMBLY__ */
