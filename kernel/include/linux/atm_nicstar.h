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

/******************************************************************************
 *
 * atm_nicstar.h
 *
 * Driver-specific declarations for use by NICSTAR driver specific utils.
 *
 * Author: Rui Prior
 *
 * (C) INESC 1998
 *
 ******************************************************************************/


#ifndef LINUX_ATM_NICSTAR_H
#define LINUX_ATM_NICSTAR_H

/* Note: non-kernel programs including this file must also include
 * sys/types.h for struct timeval
 */

#include <linux/atmapi.h>
#include <linux/atmioc.h>

#define NS_GETPSTAT	_IOWR('a',ATMIOC_SARPRV+1,struct atmif_sioc)
						/* get pool statistics */
#define NS_SETBUFLEV	_IOW('a',ATMIOC_SARPRV+2,struct atmif_sioc)
						/* set buffer level markers */
#define NS_ADJBUFLEV	_IO('a',ATMIOC_SARPRV+3)
						/* adjust buffer level */

typedef struct buf_nr
{
   unsigned min;
   unsigned init;
   unsigned max;
}buf_nr;


typedef struct pool_levels
{
   int buftype;
   int count;		/* (At least for now) only used in NS_GETPSTAT */
   buf_nr level;
} pool_levels;

/* type must be one of the following: */
#define NS_BUFTYPE_SMALL 1
#define NS_BUFTYPE_LARGE 2
#define NS_BUFTYPE_HUGE 3
#define NS_BUFTYPE_IOVEC 4


#endif /* LINUX_ATM_NICSTAR_H */
