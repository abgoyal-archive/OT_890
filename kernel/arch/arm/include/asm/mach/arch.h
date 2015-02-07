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
 *  arch/arm/include/asm/mach/arch.h
 *
 *  Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASSEMBLY__

struct tag;
struct meminfo;
struct sys_timer;

struct machine_desc {
	/*
	 * Note! The first four elements are used
	 * by assembler code in head.S, head-common.S
	 */
	unsigned int		nr;		/* architecture number	*/
	unsigned int		phys_io;	/* start of physical io	*/
	unsigned int		io_pg_offst;	/* byte offset for io 
						 * page tabe entry	*/

	const char		*name;		/* architecture name	*/
	unsigned long		boot_params;	/* tagged list		*/

	unsigned int		video_start;	/* start of video RAM	*/
	unsigned int		video_end;	/* end of video RAM	*/

	unsigned int		reserve_lp0 :1;	/* never has lp0	*/
	unsigned int		reserve_lp1 :1;	/* never has lp1	*/
	unsigned int		reserve_lp2 :1;	/* never has lp2	*/
	unsigned int		soft_reboot :1;	/* soft reboot		*/
	void			(*fixup)(struct machine_desc *,
					 struct tag *, char **,
					 struct meminfo *);
	void			(*map_io)(void);/* IO mapping function	*/
	void			(*init_irq)(void);
	struct sys_timer	*timer;		/* system tick timer	*/
	void			(*init_machine)(void);
};

/*
 * Set of macros to define architecture features.  This is built into
 * a table by the linker.
 */
#define MACHINE_START(_type,_name)			\
static const struct machine_desc __mach_desc_##_type	\
 __used							\
 __attribute__((__section__(".arch.info.init"))) = {	\
	.nr		= MACH_TYPE_##_type,		\
	.name		= _name,

#define MACHINE_END				\
};

#endif
