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
 * arch/arm/mach-ixp2000/include/mach/system.h
 *
 * Copyright (C) 2002 Intel Corp.
 * Copyricht (C) 2003-2005 MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <mach/hardware.h>
#include <asm/mach-types.h>

static inline void arch_idle(void)
{
	cpu_do_idle();
}

static inline void arch_reset(char mode)
{
	local_irq_disable();

	/*
	 * Reset flash banking register so that we are pointing at
	 * RedBoot bank.
	 */
	if (machine_is_ixdp2401()) {
		ixp2000_reg_write(IXDP2X01_CPLD_FLASH_REG,
					((0 >> IXDP2X01_FLASH_WINDOW_BITS)
						| IXDP2X01_CPLD_FLASH_INTERN));
		ixp2000_reg_wrb(IXDP2X01_CPLD_RESET_REG, 0xffffffff);
	}

	/*
	 * On IXDP2801 we need to write this magic sequence to the CPLD
	 * to cause a complete reset of the CPU and all external devices
	 * and move the flash bank register back to 0.
	 */
	if (machine_is_ixdp2801() || machine_is_ixdp28x5()) {
		unsigned long reset_reg = *IXDP2X01_CPLD_RESET_REG;

		reset_reg = 0x55AA0000 | (reset_reg & 0x0000FFFF);
		ixp2000_reg_write(IXDP2X01_CPLD_RESET_REG, reset_reg);
		ixp2000_reg_wrb(IXDP2X01_CPLD_RESET_REG, 0x80000000);
	}

	ixp2000_reg_wrb(IXP2000_RESET0, RSTALL);
}
