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
 * Copyright (C) 2000, 2001, 2002, 2003 Broadcom Corporation
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
#ifndef __ASM_SIBYTE_SWARM_H
#define __ASM_SIBYTE_SWARM_H

#include <asm/sibyte/sb1250.h>
#include <asm/sibyte/sb1250_int.h>

#ifdef CONFIG_SIBYTE_SWARM
#define SIBYTE_BOARD_NAME "BCM91250A (SWARM)"
#define SIBYTE_HAVE_PCMCIA 1
#define SIBYTE_HAVE_IDE    1
#endif
#ifdef CONFIG_SIBYTE_LITTLESUR
#define SIBYTE_BOARD_NAME "BCM91250C2 (LittleSur)"
#define SIBYTE_HAVE_PCMCIA 0
#define SIBYTE_HAVE_IDE    1
#define SIBYTE_DEFAULT_CONSOLE "cfe0"
#endif
#ifdef CONFIG_SIBYTE_CRHONE
#define SIBYTE_BOARD_NAME "BCM91125C (CRhone)"
#define SIBYTE_HAVE_PCMCIA 0
#define SIBYTE_HAVE_IDE    0
#endif
#ifdef CONFIG_SIBYTE_CRHINE
#define SIBYTE_BOARD_NAME "BCM91120C (CRhine)"
#define SIBYTE_HAVE_PCMCIA 0
#define SIBYTE_HAVE_IDE    0
#endif

/* Generic bus chip selects */
#define LEDS_CS         3
#define LEDS_PHYS       0x100a0000

#ifdef SIBYTE_HAVE_IDE
#define IDE_CS          4
#define IDE_PHYS        0x100b0000
#define K_GPIO_GB_IDE   4
#define K_INT_GB_IDE    (K_INT_GPIO_0 + K_GPIO_GB_IDE)
#endif

#ifdef SIBYTE_HAVE_PCMCIA
#define PCMCIA_CS       6
#define PCMCIA_PHYS     0x11000000
#define K_GPIO_PC_READY 9
#define K_INT_PC_READY  (K_INT_GPIO_0 + K_GPIO_PC_READY)
#endif

#endif /* __ASM_SIBYTE_SWARM_H */
