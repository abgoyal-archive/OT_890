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
 *	coldfire.h -- Motorola ColdFire CPU sepecific defines
 *
 *	(C) Copyright 1999-2006, Greg Ungerer (gerg@snapgear.com)
 *	(C) Copyright 2000, Lineo (www.lineo.com)
 */

/****************************************************************************/
#ifndef	coldfire_h
#define	coldfire_h
/****************************************************************************/


/*
 *	Define master clock frequency. This is essentially done at config
 *	time now. No point enumerating dozens of possible clock options
 *	here. Also the peripheral clock (bus clock) divide ratio is set
 *	at config time too.
 */
#ifdef CONFIG_CLOCK_SET
#define	MCF_CLK		CONFIG_CLOCK_FREQ
#define	MCF_BUSCLK	(CONFIG_CLOCK_FREQ / CONFIG_CLOCK_DIV)
#else
#error "Don't know what your ColdFire CPU clock frequency is??"
#endif

/*
 *	Define the processor support peripherals base address.
 *	This is generally setup by the boards start up code.
 */
#define	MCF_MBAR	0x10000000
#define	MCF_MBAR2	0x80000000
#if defined(CONFIG_M520x)
#define	MCF_IPSBAR	0xFC000000
#else
#define	MCF_IPSBAR	0x40000000
#endif

#if defined(CONFIG_M523x) || defined(CONFIG_M527x) || defined(CONFIG_M528x) || \
    defined(CONFIG_M520x)
#undef MCF_MBAR
#define	MCF_MBAR	MCF_IPSBAR
#elif defined(CONFIG_M532x)
#undef MCF_MBAR
#define MCF_MBAR	0x00000000
#endif

/****************************************************************************/
#endif	/* coldfire_h */
