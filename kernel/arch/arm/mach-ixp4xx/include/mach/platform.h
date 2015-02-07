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
 * arch/arm/mach-ixp4xx/include/mach/platform.h
 *
 * Constants and functions that are useful to IXP4xx platform-specific code
 * and device drivers.
 *
 * Copyright (C) 2004 MontaVista Software, Inc.
 */

#ifndef __ASM_ARCH_HARDWARE_H__
#error "Do not include this directly, instead #include <mach/hardware.h>"
#endif

#ifndef __ASSEMBLY__

#include <asm/types.h>

#ifndef	__ARMEB__
#define	REG_OFFSET	0
#else
#define	REG_OFFSET	3
#endif

/*
 * Expansion bus memory regions
 */
#define IXP4XX_EXP_BUS_BASE_PHYS	(0x50000000)

/*
 * The expansion bus on the IXP4xx can be configured for either 16 or
 * 32MB windows and the CS offset for each region changes based on the
 * current configuration. This means that we cannot simply hardcode
 * each offset. ixp4xx_sys_init() looks at the expansion bus configuration
 * as setup by the bootloader to determine our window size.
 */
extern unsigned long ixp4xx_exp_bus_size;

#define	IXP4XX_EXP_BUS_BASE(region)\
		(IXP4XX_EXP_BUS_BASE_PHYS + ((region) * ixp4xx_exp_bus_size))

#define IXP4XX_EXP_BUS_END(region)\
		(IXP4XX_EXP_BUS_BASE(region) + ixp4xx_exp_bus_size - 1)

/* Those macros can be used to adjust timing and configure
 * other features for each region.
 */

#define IXP4XX_EXP_BUS_RECOVERY_T(x)	(((x) & 0x0f) << 16)
#define IXP4XX_EXP_BUS_HOLD_T(x)	(((x) & 0x03) << 20)
#define IXP4XX_EXP_BUS_STROBE_T(x)	(((x) & 0x0f) << 22)
#define IXP4XX_EXP_BUS_SETUP_T(x)	(((x) & 0x03) << 26)
#define IXP4XX_EXP_BUS_ADDR_T(x)	(((x) & 0x03) << 28)
#define IXP4XX_EXP_BUS_SIZE(x)		(((x) & 0x0f) << 10)
#define IXP4XX_EXP_BUS_CYCLES(x)	(((x) & 0x03) << 14)

#define IXP4XX_EXP_BUS_CS_EN		(1L << 31)
#define IXP4XX_EXP_BUS_BYTE_RD16	(1L << 6)
#define IXP4XX_EXP_BUS_HRDY_POL		(1L << 5)
#define IXP4XX_EXP_BUS_MUX_EN		(1L << 4)
#define IXP4XX_EXP_BUS_SPLT_EN		(1L << 3)
#define IXP4XX_EXP_BUS_WR_EN		(1L << 1)
#define IXP4XX_EXP_BUS_BYTE_EN		(1L << 0)

#define IXP4XX_EXP_BUS_CYCLES_INTEL	0x00
#define IXP4XX_EXP_BUS_CYCLES_MOTOROLA	0x01
#define IXP4XX_EXP_BUS_CYCLES_HPI	0x02

#define IXP4XX_FLASH_WRITABLE	(0x2)
#define IXP4XX_FLASH_DEFAULT	(0xbcd23c40)
#define IXP4XX_FLASH_WRITE	(0xbcd23c42)

/*
 * Clock Speed Definitions.
 */
#define IXP4XX_PERIPHERAL_BUS_CLOCK 	(66) /* 66Mhzi APB BUS   */ 
#define IXP4XX_UART_XTAL        	14745600

/*
 * This structure provide a means for the board setup code
 * to give information to th pata_ixp4xx driver. It is
 * passed as platform_data.
 */
struct ixp4xx_pata_data {
	volatile u32	*cs0_cfg;
	volatile u32	*cs1_cfg;
	unsigned long	cs0_bits;
	unsigned long	cs1_bits;
	void __iomem	*cs0;
	void __iomem	*cs1;
};

struct sys_timer;

#define IXP4XX_ETH_NPEA		0x00
#define IXP4XX_ETH_NPEB		0x10
#define IXP4XX_ETH_NPEC		0x20

/* Information about built-in Ethernet MAC interfaces */
struct eth_plat_info {
	u8 phy;		/* MII PHY ID, 0 - 31 */
	u8 rxq;		/* configurable, currently 0 - 31 only */
	u8 txreadyq;
	u8 hwaddr[6];
};

/* Information about built-in HSS (synchronous serial) interfaces */
struct hss_plat_info {
	int (*set_clock)(int port, unsigned int clock_type);
	int (*open)(int port, void *pdev,
		    void (*set_carrier_cb)(void *pdev, int carrier));
	void (*close)(int port, void *pdev);
	u8 txreadyq;
};

/*
 * Frequency of clock used for primary clocksource
 */
extern unsigned long ixp4xx_timer_freq;

/*
 * Functions used by platform-level setup code
 */
extern void ixp4xx_map_io(void);
extern void ixp4xx_init_irq(void);
extern void ixp4xx_sys_init(void);
extern void ixp4xx_timer_init(void);
extern struct sys_timer ixp4xx_timer;
extern void ixp4xx_pci_preinit(void);
struct pci_sys_data;
extern int ixp4xx_setup(int nr, struct pci_sys_data *sys);
extern struct pci_bus *ixp4xx_scan_bus(int nr, struct pci_sys_data *sys);

/*
 * GPIO-functions
 */
/*
 * The following converted to the real HW bits the gpio_line_config
 */
/* GPIO pin types */
#define IXP4XX_GPIO_OUT 		0x1
#define IXP4XX_GPIO_IN  		0x2

/* GPIO signal types */
#define IXP4XX_GPIO_LOW			0
#define IXP4XX_GPIO_HIGH		1

/* GPIO Clocks */
#define IXP4XX_GPIO_CLK_0		14
#define IXP4XX_GPIO_CLK_1		15

static inline void gpio_line_config(u8 line, u32 direction)
{
	if (direction == IXP4XX_GPIO_IN)
		*IXP4XX_GPIO_GPOER |= (1 << line);
	else
		*IXP4XX_GPIO_GPOER &= ~(1 << line);
}

static inline void gpio_line_get(u8 line, int *value)
{
	*value = (*IXP4XX_GPIO_GPINR >> line) & 0x1;
}

static inline void gpio_line_set(u8 line, int value)
{
	if (value == IXP4XX_GPIO_HIGH)
	    *IXP4XX_GPIO_GPOUTR |= (1 << line);
	else if (value == IXP4XX_GPIO_LOW)
	    *IXP4XX_GPIO_GPOUTR &= ~(1 << line);
}

#endif // __ASSEMBLY__

