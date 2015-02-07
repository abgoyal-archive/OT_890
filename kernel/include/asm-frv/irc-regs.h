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

/* irc-regs.h: on-chip interrupt controller registers
 *
 * Copyright (C) 2003 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ASM_IRC_REGS
#define _ASM_IRC_REGS

#define __reg(ADDR) (*(volatile unsigned long *)(ADDR))

#define __get_TM0()	({ __reg(0xfeff9800); })
#define __get_TM1()	({ __reg(0xfeff9808); })
#define __set_TM1(V)	do { __reg(0xfeff9808) = (V); mb(); } while(0)

#define __set_TM1x(XI,V)			\
do {						\
	int shift = (XI) * 2 + 16;		\
	unsigned long tm1 = __reg(0xfeff9808);	\
	tm1 &= ~(0x3 << shift);			\
	tm1 |= (V) << shift;			\
	__reg(0xfeff9808) = tm1;		\
	mb();					\
} while(0)

#define __get_RS(C)	({ (__reg(0xfeff9810) >> ((C)+16)) & 1; })

#define __clr_RC(C)	do { __reg(0xfeff9818) = 1 << ((C)+16); mb(); } while(0)

#define __get_MASK(C)	({ (__reg(0xfeff9820) >> ((C)+16)) & 1; })
#define __set_MASK(C)	do { __reg(0xfeff9820) |=  1 << ((C)+16); mb(); } while(0)
#define __clr_MASK(C)	do { __reg(0xfeff9820) &=  ~(1 << ((C)+16)); mb(); } while(0)

#define __get_MASK_all() __get_MASK(0)
#define __set_MASK_all() __set_MASK(0)
#define __clr_MASK_all() __clr_MASK(0)

#define __get_IRL()	({ (__reg(0xfeff9828) >> 16) & 0xf; })
#define __clr_IRL()	do { __reg(0xfeff9828) = 0x100000; mb(); } while(0)

#define __get_IRR(N)	({ __reg(0xfeff9840 + (N) * 8); })
#define __set_IRR(N,V)	do { __reg(0xfeff9840 + (N) * 8) = (V); } while(0)

#define __get_IITMR(N)	({ __reg(0xfeff9880 + (N) * 8); })
#define __set_IITMR(N,V) do { __reg(0xfeff9880 + (N) * 8) = (V); } while(0)


#endif /* _ASM_IRC_REGS */
