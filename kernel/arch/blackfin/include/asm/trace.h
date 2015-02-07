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
 * Common header file for blackfin family of processors.
 *
 */

#ifndef _BLACKFIN_TRACE_
#define _BLACKFIN_TRACE_

/* Normally, we use ON, but you can't turn on software expansion until
 * interrupts subsystem is ready
 */

#define BFIN_TRACE_INIT ((CONFIG_DEBUG_BFIN_HWTRACE_COMPRESSION << 4) | 0x03)
#ifdef CONFIG_DEBUG_BFIN_HWTRACE_EXPAND
#define BFIN_TRACE_ON   (BFIN_TRACE_INIT | (CONFIG_DEBUG_BFIN_HWTRACE_EXPAND << 2))
#else
#define BFIN_TRACE_ON   (BFIN_TRACE_INIT)
#endif

#ifndef __ASSEMBLY__
extern unsigned long trace_buff_offset;
extern unsigned long software_trace_buff[];

/* Trace Macros for C files */

#ifdef CONFIG_DEBUG_BFIN_HWTRACE_ON

#define trace_buffer_save(x) \
	do { \
		(x) = bfin_read_TBUFCTL(); \
		bfin_write_TBUFCTL((x) & ~TBUFEN); \
	} while (0)

#define trace_buffer_restore(x) \
	do { \
		bfin_write_TBUFCTL((x));        \
	} while (0)
#else /* DEBUG_BFIN_HWTRACE_ON */

#define trace_buffer_save(x)
#define trace_buffer_restore(x)
#endif /* CONFIG_DEBUG_BFIN_HWTRACE_ON */

#else
/* Trace Macros for Assembly files */

#ifdef CONFIG_DEBUG_BFIN_HWTRACE_ON

#define trace_buffer_stop(preg, dreg)	\
	preg.L = LO(TBUFCTL);		\
	preg.H = HI(TBUFCTL);		\
	dreg = 0x1;			\
	[preg] = dreg;

#define trace_buffer_init(preg, dreg) \
	preg.L = LO(TBUFCTL);         \
	preg.H = HI(TBUFCTL);         \
	dreg = BFIN_TRACE_INIT;       \
	[preg] = dreg;

#define trace_buffer_save(preg, dreg) \
	preg.L = LO(TBUFCTL); \
	preg.H = HI(TBUFCTL); \
	dreg = [preg]; \
	[--sp] = dreg; \
	dreg = 0x1; \
	[preg] = dreg;

#define trace_buffer_restore(preg, dreg) \
	preg.L = LO(TBUFCTL); \
	preg.H = HI(TBUFCTL); \
	dreg = [sp++]; \
	[preg] = dreg;

#else /* CONFIG_DEBUG_BFIN_HWTRACE_ON */

#define trace_buffer_stop(preg, dreg)
#define trace_buffer_init(preg, dreg)
#define trace_buffer_save(preg, dreg)
#define trace_buffer_restore(preg, dreg)

#endif /* CONFIG_DEBUG_BFIN_HWTRACE_ON */

#ifdef CONFIG_DEBUG_BFIN_NO_KERN_HWTRACE
# define DEBUG_HWTRACE_SAVE(preg, dreg)    trace_buffer_save(preg, dreg)
# define DEBUG_HWTRACE_RESTORE(preg, dreg) trace_buffer_restore(preg, dreg)
#else
# define DEBUG_HWTRACE_SAVE(preg, dreg)
# define DEBUG_HWTRACE_RESTORE(preg, dreg)
#endif

#endif /* __ASSEMBLY__ */

#endif				/* _BLACKFIN_TRACE_ */
