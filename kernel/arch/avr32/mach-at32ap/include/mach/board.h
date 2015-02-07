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
 * Platform data definitions.
 */
#ifndef __ASM_ARCH_BOARD_H
#define __ASM_ARCH_BOARD_H

#include <linux/types.h>

#define GPIO_PIN_NONE	(-1)

/*
 * Clock rates for various on-board oscillators. The number of entries
 * in this array is chip-dependent.
 */
extern unsigned long at32_board_osc_rates[];
  
/*
 * This used to add essential system devices, but this is now done
 * automatically. Please don't use it in new board code.
 */
static inline void __deprecated at32_add_system_devices(void)
{

}

#define ATMEL_MAX_UART	4
extern struct platform_device *atmel_default_console_device;

struct atmel_uart_data {
	short		use_dma_tx;	/* use transmit DMA? */
	short		use_dma_rx;	/* use receive DMA? */
	void __iomem	*regs;		/* virtual base address, if any */
};
void at32_map_usart(unsigned int hw_id, unsigned int line);
struct platform_device *at32_add_device_usart(unsigned int id);

struct eth_platform_data {
	u32	phy_mask;
	u8	is_rmii;
};
struct platform_device *
at32_add_device_eth(unsigned int id, struct eth_platform_data *data);

struct spi_board_info;
struct platform_device *
at32_add_device_spi(unsigned int id, struct spi_board_info *b, unsigned int n);

struct atmel_lcdfb_info;
struct platform_device *
at32_add_device_lcdc(unsigned int id, struct atmel_lcdfb_info *data,
		     unsigned long fbmem_start, unsigned long fbmem_len,
		     u64 pin_mask);

struct usba_platform_data;
struct platform_device *
at32_add_device_usba(unsigned int id, struct usba_platform_data *data);

struct ide_platform_data {
	u8      cs;
};
struct platform_device *
at32_add_device_ide(unsigned int id, unsigned int extint,
		    struct ide_platform_data *data);

/* mask says which PWM channels to mux */
struct platform_device *at32_add_device_pwm(u32 mask);

/* depending on what's hooked up, not all SSC pins will be used */
#define	ATMEL_SSC_TK		0x01
#define	ATMEL_SSC_TF		0x02
#define	ATMEL_SSC_TD		0x04
#define	ATMEL_SSC_TX		(ATMEL_SSC_TK | ATMEL_SSC_TF | ATMEL_SSC_TD)

#define	ATMEL_SSC_RK		0x10
#define	ATMEL_SSC_RF		0x20
#define	ATMEL_SSC_RD		0x40
#define	ATMEL_SSC_RX		(ATMEL_SSC_RK | ATMEL_SSC_RF | ATMEL_SSC_RD)

struct platform_device *
at32_add_device_ssc(unsigned int id, unsigned int flags);

struct i2c_board_info;
struct platform_device *at32_add_device_twi(unsigned int id,
					    struct i2c_board_info *b,
					    unsigned int n);

struct mci_platform_data;
struct platform_device *
at32_add_device_mci(unsigned int id, struct mci_platform_data *data);

struct ac97c_platform_data {
	unsigned short dma_rx_periph_id;
	unsigned short dma_tx_periph_id;
	unsigned short dma_controller_id;
	int reset_pin;
};
struct platform_device *
at32_add_device_ac97c(unsigned int id, struct ac97c_platform_data *data);

struct platform_device *at32_add_device_abdac(unsigned int id);
struct platform_device *at32_add_device_psif(unsigned int id);

struct cf_platform_data {
	int	detect_pin;
	int	reset_pin;
	int	vcc_pin;
	int	ready_pin;
	u8	cs;
};
struct platform_device *
at32_add_device_cf(unsigned int id, unsigned int extint,
		struct cf_platform_data *data);

/* NAND / SmartMedia */
struct atmel_nand_data {
	int	enable_pin;	/* chip enable */
	int	det_pin;	/* card detect */
	int	rdy_pin;	/* ready/busy */
	u8	rdy_pin_active_low;	/* rdy_pin value is inverted */
	u8	ale;		/* address line number connected to ALE */
	u8	cle;		/* address line number connected to CLE */
	u8	bus_width_16;	/* buswidth is 16 bit */
	struct mtd_partition *(*partition_info)(int size, int *num_partitions);
};
struct platform_device *
at32_add_device_nand(unsigned int id, struct atmel_nand_data *data);

#endif /* __ASM_ARCH_BOARD_H */
