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

#ifndef _BLACKFIN_H_
#define _BLACKFIN_H_

#include <mach/anomaly.h>

#ifndef __ASSEMBLY__

/* SSYNC implementation for C file */
static inline void SSYNC(void)
{
	int _tmp;
	if (ANOMALY_05000312)
		__asm__ __volatile__(
			"cli %0;"
			"nop;"
			"nop;"
			"ssync;"
			"sti %0;"
			: "=d" (_tmp)
		);
	else if (ANOMALY_05000244)
		__asm__ __volatile__(
			"nop;"
			"nop;"
			"nop;"
			"ssync;"
		);
	else
		__asm__ __volatile__("ssync;");
}

/* CSYNC implementation for C file */
static inline void CSYNC(void)
{
	int _tmp;
	if (ANOMALY_05000312)
		__asm__ __volatile__(
			"cli %0;"
			"nop;"
			"nop;"
			"csync;"
			"sti %0;"
			: "=d" (_tmp)
		);
	else if (ANOMALY_05000244)
		__asm__ __volatile__(
			"nop;"
			"nop;"
			"nop;"
			"csync;"
		);
	else
		__asm__ __volatile__("csync;");
}

#else  /* __ASSEMBLY__ */

#define LO(con32) ((con32) & 0xFFFF)
#define lo(con32) ((con32) & 0xFFFF)
#define HI(con32) (((con32) >> 16) & 0xFFFF)
#define hi(con32) (((con32) >> 16) & 0xFFFF)

/* SSYNC & CSYNC implementations for assembly files */

#define ssync(x) SSYNC(x)
#define csync(x) CSYNC(x)

#if ANOMALY_05000312
#define SSYNC(scratch) cli scratch; nop; nop; SSYNC; sti scratch;
#define CSYNC(scratch) cli scratch; nop; nop; CSYNC; sti scratch;

#elif ANOMALY_05000244
#define SSYNC(scratch) nop; nop; nop; SSYNC;
#define CSYNC(scratch) nop; nop; nop; CSYNC;

#else
#define SSYNC(scratch) SSYNC;
#define CSYNC(scratch) CSYNC;

#endif /* ANOMALY_05000312 & ANOMALY_05000244 handling */

#endif /* __ASSEMBLY__ */

#include <mach/blackfin.h>
#include <asm/bfin-global.h>

#endif				/* _BLACKFIN_H_ */
