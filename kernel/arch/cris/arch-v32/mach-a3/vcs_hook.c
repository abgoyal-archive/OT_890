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
 * Simulator hook mechanism
 */

#include "vcs_hook.h"
#include <asm/io.h>
#include <stdarg.h>

#define HOOK_TRIG_ADDR      0xb7000000
#define HOOK_MEM_BASE_ADDR  0xce000000

static volatile unsigned *hook_base;

#define HOOK_DATA(offset) hook_base[offset]
#define VHOOK_DATA(offset) hook_base[offset]
#define HOOK_TRIG(funcid) \
	do { \
		*((unsigned *) HOOK_TRIG_ADDR) = funcid; \
	} while (0)
#define HOOK_DATA_BYTE(offset) ((unsigned char *)hook_base)[offset]

static void hook_init(void)
{
	static int first = 1;
	if (first) {
		first = 0;
		hook_base = ioremap(HOOK_MEM_BASE_ADDR, 8192);
	}
}

static unsigned hook_trig(unsigned id)
{
	unsigned ret;

	/* preempt_disable(); */

	/* Dummy read from mem to make sure data has propagated to memory
	 * before trigging */
	ret = *hook_base;

	/* trigger hook */
	HOOK_TRIG(id);

	/* wait for call to finish */
	while (VHOOK_DATA(0) > 0) ;

	/* extract return value */

	ret = VHOOK_DATA(1);

	return ret;
}

int hook_call(unsigned id, unsigned pcnt, ...)
{
	va_list ap;
	int i;
	unsigned ret;

	hook_init();

	HOOK_DATA(0) = id;

	va_start(ap, pcnt);
	for (i = 1; i <= pcnt; i++)
		HOOK_DATA(i) = va_arg(ap, unsigned);
	va_end(ap);

	ret = hook_trig(id);

	return ret;
}

int hook_call_str(unsigned id, unsigned size, const char *str)
{
	int i;
	unsigned ret;

	hook_init();

	HOOK_DATA(0) = id;
	HOOK_DATA(1) = size;

	for (i = 0; i < size; i++)
		HOOK_DATA_BYTE(8 + i) = str[i];
	HOOK_DATA_BYTE(8 + i) = 0;

	ret = hook_trig(id);

	return ret;
}

void print_str(const char *str)
{
	int i;
	/* find null at end of string */
	for (i = 1; str[i]; i++) ;
	hook_call(hook_print_str, i, str);
}

void CPU_WATCHDOG_TIMEOUT(unsigned t)
{
}
