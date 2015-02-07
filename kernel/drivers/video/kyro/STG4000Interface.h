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
 *  linux/drivers/video/kyro/STG4000Interface.h
 *
 *  Copyright (C) 2002 STMicroelectronics
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#ifndef _STG4000INTERFACE_H
#define _STG4000INTERFACE_H

#include <linux/pci.h>
#include <video/kyro.h>

/*
 * Ramdac Setup
 */
extern int InitialiseRamdac(volatile STG4000REG __iomem *pSTGReg, u32 displayDepth,
			    u32 displayWidth, u32 displayHeight,
			    s32 HSyncPolarity, s32 VSyncPolarity,
			    u32 *pixelClock);

extern void DisableRamdacOutput(volatile STG4000REG __iomem *pSTGReg);
extern void EnableRamdacOutput(volatile STG4000REG __iomem *pSTGReg);

/*
 * Timing generator setup
 */
extern void DisableVGA(volatile STG4000REG __iomem *pSTGReg);
extern void StopVTG(volatile STG4000REG __iomem *pSTGReg);
extern void StartVTG(volatile STG4000REG __iomem *pSTGReg);
extern void SetupVTG(volatile STG4000REG __iomem *pSTGReg,
		     const struct kyrofb_info * pTiming);

extern u32 ProgramClock(u32 refClock, u32 coreClock, u32 *FOut, u32 *ROut, u32 *POut);
extern int SetCoreClockPLL(volatile STG4000REG __iomem *pSTGReg, struct pci_dev *pDev);

/*
 * Overlay setup
 */
extern void ResetOverlayRegisters(volatile STG4000REG __iomem *pSTGReg);

extern int CreateOverlaySurface(volatile STG4000REG __iomem *pSTGReg,
				u32 ulWidth, u32 ulHeight,
				int bLinear,
				u32 ulOverlayOffset,
				u32 * retStride, u32 * retUVStride);

extern int SetOverlayBlendMode(volatile STG4000REG __iomem *pSTGReg,
			       OVRL_BLEND_MODE mode,
			       u32 ulAlpha, u32 ulColorKey);

extern int SetOverlayViewPort(volatile STG4000REG __iomem *pSTGReg,
			      u32 left, u32 top,
			      u32 right, u32 bottom);

extern void EnableOverlayPlane(volatile STG4000REG __iomem *pSTGReg);

#endif /* _STG4000INTERFACE_H */
