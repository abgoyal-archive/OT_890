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

#ifndef _ASM_POWERPC_HEATHROW_H
#define _ASM_POWERPC_HEATHROW_H
#ifdef __KERNEL__
/*
 * heathrow.h: definitions for using the "Heathrow" I/O controller chip.
 *
 * Grabbed from Open Firmware definitions on a PowerBook G3 Series
 *
 * Copyright (C) 1997 Paul Mackerras.
 */

/* Front light color on Yikes/B&W G3. 32 bits */
#define HEATHROW_FRONT_LIGHT		0x32 /* (set to 0 or 0xffffffff) */

/* Brightness/contrast (gossamer iMac ?). 8 bits */
#define HEATHROW_BRIGHTNESS_CNTL	0x32
#define HEATHROW_CONTRAST_CNTL		0x33

/* offset from ohare base for feature control register */
#define HEATHROW_MBCR			0x34	/* Media bay control */
#define HEATHROW_FCR			0x38	/* Feature control */
#define HEATHROW_AUX_CNTL_REG		0x3c	/* Aux control */

/*
 * Bits in feature control register.
 * Bits postfixed with a _N are in inverse logic
 */
#define HRW_SCC_TRANS_EN_N	0x00000001	/* Also controls modem power */
#define HRW_BAY_POWER_N		0x00000002
#define HRW_BAY_PCI_ENABLE	0x00000004
#define HRW_BAY_IDE_ENABLE	0x00000008
#define HRW_BAY_FLOPPY_ENABLE	0x00000010
#define HRW_IDE0_ENABLE		0x00000020
#define HRW_IDE0_RESET_N	0x00000040
#define HRW_BAY_DEV_MASK	0x0000001c
#define HRW_BAY_RESET_N		0x00000080
#define HRW_IOBUS_ENABLE	0x00000100	/* Internal IDE ? */
#define HRW_SCC_ENABLE		0x00000200
#define HRW_MESH_ENABLE		0x00000400
#define HRW_SWIM_ENABLE		0x00000800
#define HRW_SOUND_POWER_N	0x00001000
#define HRW_SOUND_CLK_ENABLE	0x00002000
#define HRW_SCCA_IO		0x00004000
#define HRW_SCCB_IO		0x00008000
#define HRW_PORT_OR_DESK_VIA_N	0x00010000	/* This one is 0 on PowerBook */
#define HRW_PWM_MON_ID_N	0x00020000	/* ??? (0) */
#define HRW_HOOK_MB_CNT_N	0x00040000	/* ??? (0) */
#define HRW_SWIM_CLONE_FLOPPY	0x00080000	/* ??? (0) */
#define HRW_AUD_RUN22		0x00100000	/* ??? (1) */
#define HRW_SCSI_LINK_MODE	0x00200000	/* Read ??? (1) */
#define HRW_ARB_BYPASS		0x00400000	/* Disable internal PCI arbitrer */
#define HRW_IDE1_RESET_N	0x00800000	/* Media bay */
#define HRW_SLOW_SCC_PCLK	0x01000000	/* ??? (0) */
#define HRW_RESET_SCC		0x02000000
#define HRW_MFDC_CELL_ENABLE	0x04000000	/* ??? (0) */
#define HRW_USE_MFDC		0x08000000	/* ??? (0) */
#define HRW_BMAC_IO_ENABLE	0x60000000	/* two bits, not documented in OF */
#define HRW_BMAC_RESET		0x80000000	/* not documented in OF */

/* We OR those features at boot on desktop G3s */
#define HRW_DEFAULTS		(HRW_SCCA_IO | HRW_SCCB_IO | HRW_SCC_ENABLE)

/* Looks like Heathrow has some sort of GPIOs as well... */
#define HRW_GPIO_MODEM_RESET	0x6d

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_HEATHROW_H */
