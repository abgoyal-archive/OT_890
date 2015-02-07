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

#ifndef A2091_H
#define A2091_H

/* $Id$
 *
 * Header file for the Commodore A2091 Zorro II SCSI controller for Linux
 *
 * Written and (C) 1993, Hamish Macdonald, see a2091.c for more info
 *
 */

#include <linux/types.h>

int a2091_detect(struct scsi_host_template *);
int a2091_release(struct Scsi_Host *);

#ifndef CMD_PER_LUN
#define CMD_PER_LUN 2
#endif

#ifndef CAN_QUEUE
#define CAN_QUEUE 16
#endif

/*
 * if the transfer address ANDed with this results in a non-zero
 * result, then we can't use DMA.
 */
#define A2091_XFER_MASK  (0xff000001)

typedef struct {
             unsigned char      pad1[64];
    volatile unsigned short     ISTR;
    volatile unsigned short     CNTR;
             unsigned char      pad2[60];
    volatile unsigned int       WTC;
    volatile unsigned long      ACR;
             unsigned char      pad3[6];
    volatile unsigned short     DAWR;
             unsigned char      pad4;
    volatile unsigned char      SASR;
             unsigned char      pad5;
    volatile unsigned char      SCMD;
             unsigned char      pad6[76];
    volatile unsigned short     ST_DMA;
    volatile unsigned short     SP_DMA;
    volatile unsigned short     CINT;
             unsigned char      pad7[2];
    volatile unsigned short     FLUSH;
} a2091_scsiregs;

#define DAWR_A2091		(3)

/* CNTR bits. */
#define CNTR_TCEN		(1<<7)
#define CNTR_PREST		(1<<6)
#define CNTR_PDMD		(1<<5)
#define CNTR_INTEN		(1<<4)
#define CNTR_DDIR		(1<<3)

/* ISTR bits. */
#define ISTR_INTX		(1<<8)
#define ISTR_INT_F		(1<<7)
#define ISTR_INTS		(1<<6)
#define ISTR_E_INT		(1<<5)
#define ISTR_INT_P		(1<<4)
#define ISTR_UE_INT		(1<<3)
#define ISTR_OE_INT		(1<<2)
#define ISTR_FF_FLG		(1<<1)
#define ISTR_FE_FLG		(1<<0)

#endif /* A2091_H */
