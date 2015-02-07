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
 *  arch/arm/mach-l7200/include/mach/pmpcon.h
 *
 *   Registers and  helper functions for the L7200 Link-Up Systems
 *   DC/DC converter register.
 *
 *   (C) Copyright 2000, S A McConnell  (samcconn@cotw.com)
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive for
 *  more details.
 */

/****************************************************************************/

#define PMPCON_OFF 0x00006000  /* Offset from IO_START_2. */

/* IO_START_2 and IO_BASE_2 are defined in hardware.h */

#define PMPCON_START (IO_START_2 + PMPCON_OFF)  /* Physical address of reg. */
#define PMPCON_BASE  (IO_BASE_2  + PMPCON_OFF)  /* Virtual address of reg. */


#define PMPCON (*(volatile unsigned int *)(PMPCON_BASE))

#define PWM2_50CYCLE 0x800
#define CONTRAST     0x9

#define PWM1H (CONTRAST)
#define PWM1L (CONTRAST << 4)

#define PMPCON_VALUE  (PWM2_50CYCLE | PWM1L | PWM1H) 
	
/* PMPCON = 0x811;   // too light and fuzzy
 * PMPCON = 0x844;   
 * PMPCON = 0x866;   // better color poor depth
 * PMPCON = 0x888;   // Darker but better depth 
 * PMPCON = 0x899;   // Darker even better depth
 * PMPCON = 0x8aa;   // too dark even better depth
 * PMPCON = 0X8cc;   // Way too dark
 */

/* As CONTRAST value increases the greater the depth perception and
 * the darker the colors.
 */
