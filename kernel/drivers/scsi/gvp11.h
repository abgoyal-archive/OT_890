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

#ifndef GVP11_H

/* $Id$
 *
 * Header file for the GVP Series II SCSI controller for Linux
 *
 * Written and (C) 1993, Ralf Baechle, see gvp11.c for more info
 * based on a2091.h (C) 1993 by Hamish Macdonald
 *
 */

#include <linux/types.h>

int gvp11_detect(struct scsi_host_template *);
int gvp11_release(struct Scsi_Host *);

#ifndef CMD_PER_LUN
#define CMD_PER_LUN 2
#endif

#ifndef CAN_QUEUE
#define CAN_QUEUE 16
#endif

#ifndef HOSTS_C

/*
 * if the transfer address ANDed with this results in a non-zero
 * result, then we can't use DMA.
 */
#define GVP11_XFER_MASK  (0xff000001)

typedef struct {
             unsigned char      pad1[64];
    volatile unsigned short     CNTR;
             unsigned char      pad2[31];
    volatile unsigned char      SASR;
             unsigned char      pad3;
    volatile unsigned char      SCMD;
             unsigned char      pad4[4];
    volatile unsigned short     BANK;
             unsigned char      pad5[6];
    volatile unsigned long      ACR;
    volatile unsigned short     secret1; /* store 0 here */
    volatile unsigned short     ST_DMA;
    volatile unsigned short     SP_DMA;
    volatile unsigned short     secret2; /* store 1 here */
    volatile unsigned short     secret3; /* store 15 here */
} gvp11_scsiregs;

/* bits in CNTR */
#define GVP11_DMAC_BUSY		(1<<0)
#define GVP11_DMAC_INT_PENDING	(1<<1)
#define GVP11_DMAC_INT_ENABLE	(1<<3)
#define GVP11_DMAC_DIR_WRITE	(1<<4)

#endif /* else def HOSTS_C */

#endif /* GVP11_H */
