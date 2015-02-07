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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2000-2005 Silicon Graphics, Inc. All rights reserved.
 */

#ifndef _ASM_IA64_SN_TIO_H
#define _ASM_IA64_SN_TIO_H

#define	TIO_MMR_ADDR_MOD

#define TIO_NODE_ID     TIO_MMR_ADDR_MOD(0x0000000090060e80)

#define TIO_ITTE_BASE   0xb0008800        /* base of translation table entries */
#define TIO_ITTE(bigwin)        (TIO_ITTE_BASE + 8*(bigwin))

#define TIO_ITTE_OFFSET_BITS    8       /* size of offset field */
#define TIO_ITTE_OFFSET_MASK    ((1<<TIO_ITTE_OFFSET_BITS)-1)
#define TIO_ITTE_OFFSET_SHIFT   0

#define TIO_ITTE_WIDGET_BITS    2       /* size of widget field */
#define TIO_ITTE_WIDGET_MASK    ((1<<TIO_ITTE_WIDGET_BITS)-1)
#define TIO_ITTE_WIDGET_SHIFT   12
#define TIO_ITTE_VALID_MASK	0x1
#define TIO_ITTE_VALID_SHIFT	16

#define TIO_ITTE_WIDGET(itte) \
	(((itte) >> TIO_ITTE_WIDGET_SHIFT) & TIO_ITTE_WIDGET_MASK)
#define TIO_ITTE_VALID(itte) \
	(((itte) >> TIO_ITTE_VALID_SHIFT) & TIO_ITTE_VALID_MASK)

#define TIO_ITTE_PUT(nasid, bigwin, widget, addr, valid) \
        REMOTE_HUB_S((nasid), TIO_ITTE(bigwin), \
                (((((addr) >> TIO_BWIN_SIZE_BITS) & \
                   TIO_ITTE_OFFSET_MASK) << TIO_ITTE_OFFSET_SHIFT) | \
                (((widget) & TIO_ITTE_WIDGET_MASK) << TIO_ITTE_WIDGET_SHIFT)) | \
		(( (valid) & TIO_ITTE_VALID_MASK) << TIO_ITTE_VALID_SHIFT))

#endif /*  _ASM_IA64_SN_TIO_H */
