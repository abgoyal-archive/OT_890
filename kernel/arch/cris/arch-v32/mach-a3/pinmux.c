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
 * Allocator for I/O pins. All pins are allocated to GPIO at bootup.
 * Unassigned pins and GPIO pins can be allocated to a fixed interface
 * or the I/O processor instead.
 *
 * Copyright (c) 2005-2007 Axis Communications AB.
 */

#include <linux/init.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/spinlock.h>
#include <hwregs/reg_map.h>
#include <hwregs/reg_rdwr.h>
#include <pinmux.h>
#include <hwregs/pinmux_defs.h>
#include <hwregs/clkgen_defs.h>

#undef DEBUG

#define PINS 80
#define PORT_PINS 32
#define PORTS 3

static char pins[PINS];
static DEFINE_SPINLOCK(pinmux_lock);

static void crisv32_pinmux_set(int port);

int
crisv32_pinmux_init(void)
{
	static int initialized;

	if (!initialized) {
		initialized = 1;
		REG_WR_INT(pinmux, regi_pinmux, rw_hwprot, 0);
		crisv32_pinmux_alloc(PORT_A, 0, 31, pinmux_gpio);
		crisv32_pinmux_alloc(PORT_B, 0, 31, pinmux_gpio);
		crisv32_pinmux_alloc(PORT_C, 0, 15, pinmux_gpio);
	}

	return 0;
}

int
crisv32_pinmux_alloc(int port, int first_pin, int last_pin, enum pin_mode mode)
{
	int i;
	unsigned long flags;

	crisv32_pinmux_init();

	if (port >= PORTS)
		return -EINVAL;

	spin_lock_irqsave(&pinmux_lock, flags);

	for (i = first_pin; i <= last_pin; i++) {
		if ((pins[port * PORT_PINS + i] != pinmux_none) &&
		    (pins[port * PORT_PINS + i] != pinmux_gpio) &&
		    (pins[port * PORT_PINS + i] != mode)) {
			spin_unlock_irqrestore(&pinmux_lock, flags);
#ifdef DEBUG
			panic("Pinmux alloc failed!\n");
#endif
			return -EPERM;
		}
	}

	for (i = first_pin; i <= last_pin; i++)
		pins[port * PORT_PINS + i] = mode;

	crisv32_pinmux_set(port);

	spin_unlock_irqrestore(&pinmux_lock, flags);

	return 0;
}

int
crisv32_pinmux_alloc_fixed(enum fixed_function function)
{
	int ret = -EINVAL;
	char saved[sizeof pins];
	unsigned long flags;

	spin_lock_irqsave(&pinmux_lock, flags);

	/* Save internal data for recovery */
	memcpy(saved, pins, sizeof pins);

	crisv32_pinmux_init(); /* must be done before we read rw_hwprot */

	reg_pinmux_rw_hwprot hwprot = REG_RD(pinmux, regi_pinmux, rw_hwprot);
	reg_clkgen_rw_clk_ctrl clk_ctrl = REG_RD(clkgen, regi_clkgen,
		rw_clk_ctrl);

	switch (function) {
	case pinmux_eth:
		clk_ctrl.eth = regk_clkgen_yes;
		clk_ctrl.dma0_1_eth = regk_clkgen_yes;
		ret = crisv32_pinmux_alloc(PORT_B, 8, 23, pinmux_fixed);
		ret |= crisv32_pinmux_alloc(PORT_B, 24, 25, pinmux_fixed);
		hwprot.eth = hwprot.eth_mdio = regk_pinmux_yes;
		break;
	case pinmux_geth:
		ret = crisv32_pinmux_alloc(PORT_B, 0, 7, pinmux_fixed);
		hwprot.geth = regk_pinmux_yes;
		break;
	case pinmux_tg_cmos:
		clk_ctrl.ccd_tg_100 = clk_ctrl.ccd_tg_200 = regk_clkgen_yes;
		ret = crisv32_pinmux_alloc(PORT_B, 27, 29, pinmux_fixed);
		hwprot.tg_clk = regk_pinmux_yes;
		break;
	case pinmux_tg_ccd:
		clk_ctrl.ccd_tg_100 = clk_ctrl.ccd_tg_200 = regk_clkgen_yes;
		ret = crisv32_pinmux_alloc(PORT_B, 27, 31, pinmux_fixed);
		ret |= crisv32_pinmux_alloc(PORT_C, 0, 15, pinmux_fixed);
		hwprot.tg = hwprot.tg_clk = regk_pinmux_yes;
		break;
	case pinmux_vout:
		clk_ctrl.strdma0_2_video = regk_clkgen_yes;
		ret = crisv32_pinmux_alloc(PORT_A, 8, 18, pinmux_fixed);
		hwprot.vout = hwprot.vout_sync = regk_pinmux_yes;
		break;
	case pinmux_ser1:
		clk_ctrl.sser_ser_dma6_7 = regk_clkgen_yes;
		ret = crisv32_pinmux_alloc(PORT_A, 24, 25, pinmux_fixed);
		hwprot.ser1 = regk_pinmux_yes;
		break;
	case pinmux_ser2:
		clk_ctrl.sser_ser_dma6_7 = regk_clkgen_yes;
		ret = crisv32_pinmux_alloc(PORT_A, 26, 27, pinmux_fixed);
		hwprot.ser2 = regk_pinmux_yes;
		break;
	case pinmux_ser3:
		clk_ctrl.sser_ser_dma6_7 = regk_clkgen_yes;
		ret = crisv32_pinmux_alloc(PORT_A, 28, 29, pinmux_fixed);
		hwprot.ser3 = regk_pinmux_yes;
		break;
	case pinmux_ser4:
		clk_ctrl.sser_ser_dma6_7 = regk_clkgen_yes;
		ret = crisv32_pinmux_alloc(PORT_A, 30, 31, pinmux_fixed);
		hwprot.ser4 = regk_pinmux_yes;
		break;
	case pinmux_sser:
		clk_ctrl.sser_ser_dma6_7 = regk_clkgen_yes;
		ret = crisv32_pinmux_alloc(PORT_A, 19, 23, pinmux_fixed);
		hwprot.sser = regk_pinmux_yes;
		break;
	case pinmux_pio:
		hwprot.pio = regk_pinmux_yes;
		ret = 0;
		break;
	case pinmux_pwm0:
		ret = crisv32_pinmux_alloc(PORT_A, 30, 30, pinmux_fixed);
		hwprot.pwm0 = regk_pinmux_yes;
		break;
	case pinmux_pwm1:
		ret = crisv32_pinmux_alloc(PORT_A, 31, 31, pinmux_fixed);
		hwprot.pwm1 = regk_pinmux_yes;
		break;
	case pinmux_pwm2:
		ret = crisv32_pinmux_alloc(PORT_B, 26, 26, pinmux_fixed);
		hwprot.pwm2 = regk_pinmux_yes;
		break;
	case pinmux_i2c0:
		ret = crisv32_pinmux_alloc(PORT_A, 0, 1, pinmux_fixed);
		hwprot.i2c0 = regk_pinmux_yes;
		break;
	case pinmux_i2c1:
		ret = crisv32_pinmux_alloc(PORT_A, 2, 3, pinmux_fixed);
		hwprot.i2c1 = regk_pinmux_yes;
		break;
	case pinmux_i2c1_3wire:
		ret = crisv32_pinmux_alloc(PORT_A, 2, 3, pinmux_fixed);
		ret |= crisv32_pinmux_alloc(PORT_A, 7, 7, pinmux_fixed);
		hwprot.i2c1 = hwprot.i2c1_sen = regk_pinmux_yes;
		break;
	case pinmux_i2c1_sda1:
		ret = crisv32_pinmux_alloc(PORT_A, 2, 4, pinmux_fixed);
		hwprot.i2c1 = hwprot.i2c1_sda1 = regk_pinmux_yes;
		break;
	case pinmux_i2c1_sda2:
		ret = crisv32_pinmux_alloc(PORT_A, 2, 3, pinmux_fixed);
		ret |= crisv32_pinmux_alloc(PORT_A, 5, 5, pinmux_fixed);
		hwprot.i2c1 = hwprot.i2c1_sda2 = regk_pinmux_yes;
		break;
	case pinmux_i2c1_sda3:
		ret = crisv32_pinmux_alloc(PORT_A, 2, 3, pinmux_fixed);
		ret |= crisv32_pinmux_alloc(PORT_A, 6, 6, pinmux_fixed);
		hwprot.i2c1 = hwprot.i2c1_sda3 = regk_pinmux_yes;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	if (!ret) {
		REG_WR(pinmux, regi_pinmux, rw_hwprot, hwprot);
		REG_WR(clkgen, regi_clkgen, rw_clk_ctrl, clk_ctrl);
	} else
		memcpy(pins, saved, sizeof pins);

  spin_unlock_irqrestore(&pinmux_lock, flags);

  return ret;
}

void
crisv32_pinmux_set(int port)
{
	int i;
	int gpio_val = 0;
	int iop_val = 0;
	int pin = port * PORT_PINS;

	for (i = 0; (i < PORT_PINS) && (pin < PINS); i++, pin++) {
		if (pins[pin] == pinmux_gpio)
			gpio_val |= (1 << i);
		else if (pins[pin] == pinmux_iop)
			iop_val |= (1 << i);
	}

	REG_WRITE(int, regi_pinmux + REG_RD_ADDR_pinmux_rw_gio_pa + 4 * port,
		gpio_val);
	REG_WRITE(int, regi_pinmux + REG_RD_ADDR_pinmux_rw_iop_pa + 4 * port,
		iop_val);

#ifdef DEBUG
       crisv32_pinmux_dump();
#endif
}

int
crisv32_pinmux_dealloc(int port, int first_pin, int last_pin)
{
	int i;
	unsigned long flags;

	crisv32_pinmux_init();

	if (port > PORTS)
		return -EINVAL;

	spin_lock_irqsave(&pinmux_lock, flags);

	for (i = first_pin; i <= last_pin; i++)
		pins[port * PORT_PINS + i] = pinmux_none;

	crisv32_pinmux_set(port);
	spin_unlock_irqrestore(&pinmux_lock, flags);

	return 0;
}

int
crisv32_pinmux_dealloc_fixed(enum fixed_function function)
{
	int ret = -EINVAL;
	char saved[sizeof pins];
	unsigned long flags;

	spin_lock_irqsave(&pinmux_lock, flags);

	/* Save internal data for recovery */
	memcpy(saved, pins, sizeof pins);

	crisv32_pinmux_init(); /* must be done before we read rw_hwprot */

	reg_pinmux_rw_hwprot hwprot = REG_RD(pinmux, regi_pinmux, rw_hwprot);

	switch (function) {
	case pinmux_eth:
		ret = crisv32_pinmux_dealloc(PORT_B, 8, 23);
		ret |= crisv32_pinmux_dealloc(PORT_B, 24, 25);
		ret |= crisv32_pinmux_dealloc(PORT_B, 0, 7);
		hwprot.eth = hwprot.eth_mdio = hwprot.geth = regk_pinmux_no;
		break;
	case pinmux_tg_cmos:
		ret = crisv32_pinmux_dealloc(PORT_B, 27, 29);
		hwprot.tg_clk = regk_pinmux_no;
		break;
	case pinmux_tg_ccd:
		ret = crisv32_pinmux_dealloc(PORT_B, 27, 31);
		ret |= crisv32_pinmux_dealloc(PORT_C, 0, 15);
		hwprot.tg = hwprot.tg_clk = regk_pinmux_no;
		break;
	case pinmux_vout:
		ret = crisv32_pinmux_dealloc(PORT_A, 8, 18);
		hwprot.vout = hwprot.vout_sync = regk_pinmux_no;
		break;
	case pinmux_ser1:
		ret = crisv32_pinmux_dealloc(PORT_A, 24, 25);
		hwprot.ser1 = regk_pinmux_no;
		break;
	case pinmux_ser2:
		ret = crisv32_pinmux_dealloc(PORT_A, 26, 27);
		hwprot.ser2 = regk_pinmux_no;
		break;
	case pinmux_ser3:
		ret = crisv32_pinmux_dealloc(PORT_A, 28, 29);
		hwprot.ser3 = regk_pinmux_no;
		break;
	case pinmux_ser4:
		ret = crisv32_pinmux_dealloc(PORT_A, 30, 31);
		hwprot.ser4 = regk_pinmux_no;
		break;
	case pinmux_sser:
		ret = crisv32_pinmux_dealloc(PORT_A, 19, 23);
		hwprot.sser = regk_pinmux_no;
		break;
	case pinmux_pwm0:
		ret = crisv32_pinmux_dealloc(PORT_A, 30, 30);
		hwprot.pwm0 = regk_pinmux_no;
		break;
	case pinmux_pwm1:
		ret = crisv32_pinmux_dealloc(PORT_A, 31, 31);
		hwprot.pwm1 = regk_pinmux_no;
		break;
	case pinmux_pwm2:
		ret = crisv32_pinmux_dealloc(PORT_B, 26, 26);
		hwprot.pwm2 = regk_pinmux_no;
		break;
	case pinmux_i2c0:
		ret = crisv32_pinmux_dealloc(PORT_A, 0, 1);
		hwprot.i2c0 = regk_pinmux_no;
		break;
	case pinmux_i2c1:
		ret = crisv32_pinmux_dealloc(PORT_A, 2, 3);
		hwprot.i2c1 = regk_pinmux_no;
		break;
	case pinmux_i2c1_3wire:
		ret = crisv32_pinmux_dealloc(PORT_A, 2, 3);
		ret |= crisv32_pinmux_dealloc(PORT_A, 7, 7);
		hwprot.i2c1 = hwprot.i2c1_sen = regk_pinmux_no;
		break;
	case pinmux_i2c1_sda1:
		ret = crisv32_pinmux_dealloc(PORT_A, 2, 4);
		hwprot.i2c1_sda1 = regk_pinmux_no;
		break;
	case pinmux_i2c1_sda2:
		ret = crisv32_pinmux_dealloc(PORT_A, 2, 3);
		ret |= crisv32_pinmux_dealloc(PORT_A, 5, 5);
		hwprot.i2c1_sda2 = regk_pinmux_no;
		break;
	case pinmux_i2c1_sda3:
		ret = crisv32_pinmux_dealloc(PORT_A, 2, 3);
		ret |= crisv32_pinmux_dealloc(PORT_A, 6, 6);
		hwprot.i2c1_sda3 = regk_pinmux_no;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	if (!ret)
		REG_WR(pinmux, regi_pinmux, rw_hwprot, hwprot);
	else
		memcpy(pins, saved, sizeof pins);

  spin_unlock_irqrestore(&pinmux_lock, flags);

  return ret;
}

void
crisv32_pinmux_dump(void)
{
	int i, j;
	int pin = 0;

	crisv32_pinmux_init();

	for (i = 0; i < PORTS; i++) {
		pin++;
		printk(KERN_DEBUG "Port %c\n", 'A'+i);
		for (j = 0; (j < PORT_PINS) && (pin < PINS); j++, pin++)
			printk(KERN_DEBUG
				"  Pin %d = %d\n", j, pins[i * PORT_PINS + j]);
	}
}

__initcall(crisv32_pinmux_init);
