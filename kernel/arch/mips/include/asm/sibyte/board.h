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
 * Copyright (C) 2000,2001,2002,2003,2004 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef _SIBYTE_BOARD_H
#define _SIBYTE_BOARD_H

#if defined(CONFIG_SIBYTE_SWARM) || defined(CONFIG_SIBYTE_CRHONE) || \
    defined(CONFIG_SIBYTE_CRHINE) || defined(CONFIG_SIBYTE_LITTLESUR)
#include <asm/sibyte/swarm.h>
#endif

#if defined(CONFIG_SIBYTE_SENTOSA) || defined(CONFIG_SIBYTE_RHONE)
#include <asm/sibyte/sentosa.h>
#endif

#ifdef CONFIG_SIBYTE_CARMEL
#include <asm/sibyte/carmel.h>
#endif

#ifdef CONFIG_SIBYTE_BIGSUR
#include <asm/sibyte/bigsur.h>
#endif

#ifdef __ASSEMBLY__

#ifdef LEDS_PHYS
#define setleds(t0, t1, c0, c1, c2, c3) \
	li	t0, (LEDS_PHYS|0xa0000000); \
	li	t1, c0; \
	sb	t1, 0x18(t0); \
	li	t1, c1; \
	sb	t1, 0x10(t0); \
	li	t1, c2; \
	sb	t1, 0x08(t0); \
	li	t1, c3; \
	sb	t1, 0x00(t0)
#else
#define setleds(t0, t1, c0, c1, c2, c3)
#endif /* LEDS_PHYS */

#else

void swarm_setup(void);

#ifdef LEDS_PHYS
extern void setleds(char *str);
#else
#define setleds(s) do { } while (0)
#endif /* LEDS_PHYS */

#endif /* __ASSEMBLY__ */

#endif /* _SIBYTE_BOARD_H */
