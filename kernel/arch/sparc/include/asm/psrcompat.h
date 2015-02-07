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

#ifndef _SPARC64_PSRCOMPAT_H
#define _SPARC64_PSRCOMPAT_H

#include <asm/pstate.h>

/* Old 32-bit PSR fields for the compatibility conversion code. */
#define PSR_CWP     0x0000001f         /* current window pointer     */
#define PSR_ET      0x00000020         /* enable traps field         */
#define PSR_PS      0x00000040         /* previous privilege level   */
#define PSR_S       0x00000080         /* current privilege level    */
#define PSR_PIL     0x00000f00         /* processor interrupt level  */
#define PSR_EF      0x00001000         /* enable floating point      */
#define PSR_EC      0x00002000         /* enable co-processor        */
#define PSR_SYSCALL 0x00004000         /* inside of a syscall        */
#define PSR_LE      0x00008000         /* SuperSparcII little-endian */
#define PSR_ICC     0x00f00000         /* integer condition codes    */
#define PSR_C       0x00100000         /* carry bit                  */
#define PSR_V       0x00200000         /* overflow bit               */
#define PSR_Z       0x00400000         /* zero bit                   */
#define PSR_N       0x00800000         /* negative bit               */
#define PSR_VERS    0x0f000000         /* cpu-version field          */
#define PSR_IMPL    0xf0000000         /* cpu-implementation field   */

#define PSR_V8PLUS  0xff000000         /* fake impl/ver, meaning a 64bit CPU is present */
#define PSR_XCC	    0x000f0000         /* if PSR_V8PLUS, this is %xcc */

static inline unsigned int tstate_to_psr(unsigned long tstate)
{
	return ((tstate & TSTATE_CWP)			|
		PSR_S					|
		((tstate & TSTATE_ICC) >> 12)		|
		((tstate & TSTATE_XCC) >> 20)		|
		((tstate & TSTATE_SYSCALL) ? PSR_SYSCALL : 0) |
		PSR_V8PLUS);
}

static inline unsigned long psr_to_tstate_icc(unsigned int psr)
{
	unsigned long tstate = ((unsigned long)(psr & PSR_ICC)) << 12;
	if ((psr & (PSR_VERS|PSR_IMPL)) == PSR_V8PLUS)
		tstate |= ((unsigned long)(psr & PSR_XCC)) << 20;
	return tstate;
}

#endif /* !(_SPARC64_PSRCOMPAT_H) */
