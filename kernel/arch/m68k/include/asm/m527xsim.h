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

/****************************************************************************/

/*
 *	m527xsim.h -- ColdFire 5270/5271 System Integration Module support.
 *
 *	(C) Copyright 2004, Greg Ungerer (gerg@snapgear.com)
 */

/****************************************************************************/
#ifndef	m527xsim_h
#define	m527xsim_h
/****************************************************************************/


/*
 *	Define the 5270/5271 SIM register set addresses.
 */
#define	MCFICM_INTC0		0x0c00		/* Base for Interrupt Ctrl 0 */
#define	MCFICM_INTC1		0x0d00		/* Base for Interrupt Ctrl 1 */
#define	MCFINTC_IPRH		0x00		/* Interrupt pending 32-63 */
#define	MCFINTC_IPRL		0x04		/* Interrupt pending 1-31 */
#define	MCFINTC_IMRH		0x08		/* Interrupt mask 32-63 */
#define	MCFINTC_IMRL		0x0c		/* Interrupt mask 1-31 */
#define	MCFINTC_INTFRCH		0x10		/* Interrupt force 32-63 */
#define	MCFINTC_INTFRCL		0x14		/* Interrupt force 1-31 */
#define	MCFINTC_IRLR		0x18		/* */
#define	MCFINTC_IACKL		0x19		/* */
#define	MCFINTC_ICR0		0x40		/* Base ICR register */

#define	MCFINT_VECBASE		64		/* Vector base number */
#define	MCFINT_UART0		13		/* Interrupt number for UART0 */
#define	MCFINT_UART1		14		/* Interrupt number for UART1 */
#define	MCFINT_UART2		15		/* Interrupt number for UART2 */
#define	MCFINT_PIT1		36		/* Interrupt number for PIT1 */

/*
 *	SDRAM configuration registers.
 */
#ifdef CONFIG_M5271
#define	MCFSIM_DCR		0x40		/* SDRAM control */
#define	MCFSIM_DACR0		0x48		/* SDRAM base address 0 */
#define	MCFSIM_DMR0		0x4c		/* SDRAM address mask 0 */
#define	MCFSIM_DACR1		0x50		/* SDRAM base address 1 */
#define	MCFSIM_DMR1		0x54		/* SDRAM address mask 1 */
#endif
#ifdef CONFIG_M5275
#define	MCFSIM_DMR		0x40		/* SDRAM mode */
#define	MCFSIM_DCR		0x44		/* SDRAM control */
#define	MCFSIM_DCFG1		0x48		/* SDRAM configuration 1 */
#define	MCFSIM_DCFG2		0x4c		/* SDRAM configuration 2 */
#define	MCFSIM_DBAR0		0x50		/* SDRAM base address 0 */
#define	MCFSIM_DMR0		0x54		/* SDRAM address mask 0 */
#define	MCFSIM_DBAR1		0x58		/* SDRAM base address 1 */
#define	MCFSIM_DMR1		0x5c		/* SDRAM address mask 1 */
#endif

/*
 *	GPIO pins setups to enable the UARTs.
 */
#ifdef CONFIG_M5271
#define MCF_GPIO_PAR_UART	0x100048	/* PAR UART address */
#define UART0_ENABLE_MASK	0x000f
#define UART1_ENABLE_MASK	0x0ff0
#define UART2_ENABLE_MASK	0x3000
#endif
#ifdef CONFIG_M5275
#define MCF_GPIO_PAR_UART	0x10007c	/* PAR UART address */
#define UART0_ENABLE_MASK	0x000f
#define UART1_ENABLE_MASK	0x00f0
#define UART2_ENABLE_MASK	0x3f00 
#endif

/****************************************************************************/
#endif	/* m527xsim_h */
