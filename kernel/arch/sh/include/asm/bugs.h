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

#ifndef __ASM_SH_BUGS_H
#define __ASM_SH_BUGS_H

/*
 * This is included by init/main.c to check for architecture-dependent bugs.
 *
 * Needs:
 *	void check_bugs(void);
 */

/*
 * I don't know of any Super-H bugs yet.
 */

#include <asm/processor.h>

static void __init check_bugs(void)
{
	extern unsigned long loops_per_jiffy;
	char *p = &init_utsname()->machine[2]; /* "sh" */

	current_cpu_data.loops_per_jiffy = loops_per_jiffy;

	switch (current_cpu_data.type) {
	case CPU_SH7619:
		*p++ = '2';
		break;
	case CPU_SH7201 ... CPU_MXG:
		*p++ = '2';
		*p++ = 'a';
		break;
	case CPU_SH7705 ... CPU_SH7729:
		*p++ = '3';
		break;
	case CPU_SH7750 ... CPU_SH4_501:
		*p++ = '4';
		break;
	case CPU_SH7763 ... CPU_SHX3:
		*p++ = '4';
		*p++ = 'a';
		break;
	case CPU_SH7343 ... CPU_SH7366:
		*p++ = '4';
		*p++ = 'a';
		*p++ = 'l';
		*p++ = '-';
		*p++ = 'd';
		*p++ = 's';
		*p++ = 'p';
		break;
	case CPU_SH5_101 ... CPU_SH5_103:
		*p++ = '6';
		*p++ = '4';
		break;
	case CPU_SH_NONE:
		/*
		 * Specifically use CPU_SH_NONE rather than default:,
		 * so we're able to have the compiler whine about
		 * unhandled enumerations.
		 */
		break;
	}

	printk("CPU: %s\n", get_cpu_subtype(&current_cpu_data));

#ifndef __LITTLE_ENDIAN__
	/* 'eb' means 'Endian Big' */
	*p++ = 'e';
	*p++ = 'b';
#endif
	*p = '\0';
}
#endif /* __ASM_SH_BUGS_H */
