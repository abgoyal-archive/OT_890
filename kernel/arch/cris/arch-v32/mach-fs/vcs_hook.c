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
 * Call simulator hook. This is the part running in the
 * simulated program.
 */

#include "vcs_hook.h"
#include <stdarg.h>
#include <arch-v32/hwregs/reg_map.h>
#include <arch-v32/hwregs/intr_vect_defs.h>

#define HOOK_TRIG_ADDR     0xb7000000	/* hook cvlog model reg address */
#define HOOK_MEM_BASE_ADDR 0xa0000000	/* csp4 (shared mem) base addr */

#define HOOK_DATA(offset) ((unsigned *)HOOK_MEM_BASE_ADDR)[offset]
#define VHOOK_DATA(offset) ((volatile unsigned *)HOOK_MEM_BASE_ADDR)[offset]
#define HOOK_TRIG(funcid) \
	do { \
		*((unsigned *) HOOK_TRIG_ADDR) = funcid; \
	} while (0)
#define HOOK_DATA_BYTE(offset) ((unsigned char *)HOOK_MEM_BASE_ADDR)[offset]

int hook_call(unsigned id, unsigned pcnt, ...)
{
	va_list ap;
	unsigned i;
	unsigned ret;
#ifdef USING_SOS
	PREEMPT_OFF_SAVE();
#endif

	/* pass parameters */
	HOOK_DATA(0) = id;

	/* Have to make hook_print_str a special case since we call with a
	 * parameter of byte type. Should perhaps be a separate
	 * hook_call. */

	if (id == hook_print_str) {
		int i;
		char *str;

		HOOK_DATA(1) = pcnt;

		va_start(ap, pcnt);
		str = (char *)va_arg(ap, unsigned);

		for (i = 0; i != pcnt; i++)
			HOOK_DATA_BYTE(8 + i) = str[i];

		HOOK_DATA_BYTE(8 + i) = 0;	/* null byte */
	} else {
		va_start(ap, pcnt);
		for (i = 1; i <= pcnt; i++)
			HOOK_DATA(i) = va_arg(ap, unsigned);
		va_end(ap);
	}

	/* read from mem to make sure data has propagated to memory before
	 * trigging */
	ret = *((volatile unsigned *)HOOK_MEM_BASE_ADDR);

	/* trigger hook */
	HOOK_TRIG(id);

	/* wait for call to finish */
	while (VHOOK_DATA(0) > 0) ;

	/* extract return value */

	ret = VHOOK_DATA(1);

#ifdef USING_SOS
	PREEMPT_RESTORE();
#endif
	return ret;
}

unsigned hook_buf(unsigned i)
{
	return (HOOK_DATA(i));
}

void print_str(const char *str)
{
	int i;
	/* find null at end of string */
	for (i = 1; str[i]; i++) ;
	hook_call(hook_print_str, i, str);
}

void CPU_KICK_DOG(void)
{
	(void)hook_call(hook_kick_dog, 0);
}

void CPU_WATCHDOG_TIMEOUT(unsigned t)
{
	(void)hook_call(hook_dog_timeout, 1, t);
}

