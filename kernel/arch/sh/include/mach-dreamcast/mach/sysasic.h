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

/* include/asm-sh/dreamcast/sysasic.h
 *
 * Definitions for the Dreamcast System ASIC and related peripherals.
 *
 * Copyright (c) 2001 M. R. Brown <mrbrown@linuxdc.org>
 * Copyright (C) 2003 Paul Mundt <lethal@linux-sh.org>
 *
 * This file is part of the LinuxDC project (www.linuxdc.org)
 *
 * Released under the terms of the GNU GPL v2.0.
 *
 */
#ifndef __ASM_SH_DREAMCAST_SYSASIC_H
#define __ASM_SH_DREAMCAST_SYSASIC_H

#include <asm/irq.h>

/* Hardware events -

   Each of these events correspond to a bit within the Event Mask Registers/
   Event Status Registers.  Because of the virtual IRQ numbering scheme, a
   base offset must be used when calculating the virtual IRQ that each event
   takes.
*/

#define HW_EVENT_IRQ_BASE  48

/* IRQ 13 */
#define HW_EVENT_VSYNC     (HW_EVENT_IRQ_BASE +  5) /* VSync */
#define HW_EVENT_MAPLE_DMA (HW_EVENT_IRQ_BASE + 12) /* Maple DMA complete */
#define HW_EVENT_GDROM_DMA (HW_EVENT_IRQ_BASE + 14) /* GD-ROM DMA complete */
#define HW_EVENT_G2_DMA    (HW_EVENT_IRQ_BASE + 15) /* G2 DMA complete */
#define HW_EVENT_PVR2_DMA  (HW_EVENT_IRQ_BASE + 19) /* PVR2 DMA complete */

/* IRQ 11 */
#define HW_EVENT_GDROM_CMD (HW_EVENT_IRQ_BASE + 32) /* GD-ROM cmd. complete */
#define HW_EVENT_AICA_SYS  (HW_EVENT_IRQ_BASE + 33) /* AICA-related */
#define HW_EVENT_EXTERNAL  (HW_EVENT_IRQ_BASE + 35) /* Ext. (expansion) */

#define HW_EVENT_IRQ_MAX (HW_EVENT_IRQ_BASE + 95)

#endif /* __ASM_SH_DREAMCAST_SYSASIC_H */

