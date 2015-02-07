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

#ifndef __LINUX_SPI_MMC_SPI_H
#define __LINUX_SPI_MMC_SPI_H

#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/interrupt.h>

struct mmc_host;

/* Put this in platform_data of a device being used to manage an MMC/SD
 * card slot.  (Modeled after PXA mmc glue; see that for usage examples.)
 *
 * REVISIT This is not a spi-specific notion.  Any card slot should be
 * able to handle it.  If the MMC core doesn't adopt this kind of notion,
 * switch the "struct device *" parameters over to "struct spi_device *".
 */
struct mmc_spi_platform_data {
	/* driver activation and (optional) card detect irq hookup */
	int (*init)(struct device *,
		irqreturn_t (*)(int, void *),
		void *);
	void (*exit)(struct device *, void *);

	/* sense switch on sd cards */
	int (*get_ro)(struct device *);

	/*
	 * If board does not use CD interrupts, driver can optimize polling
	 * using this function.
	 */
	int (*get_cd)(struct device *);

	/* Capabilities to pass into mmc core (e.g. MMC_CAP_NEEDS_POLL). */
	unsigned long caps;

	/* how long to debounce card detect, in msecs */
	u16 detect_delay;

	/* power management */
	u16 powerup_msecs;		/* delay of up to 250 msec */
	u32 ocr_mask;			/* available voltages */
	void (*setpower)(struct device *, unsigned int maskval);
};

#ifdef CONFIG_OF
extern struct mmc_spi_platform_data *mmc_spi_get_pdata(struct spi_device *spi);
extern void mmc_spi_put_pdata(struct spi_device *spi);
#else
static inline struct mmc_spi_platform_data *
mmc_spi_get_pdata(struct spi_device *spi)
{
	return spi->dev.platform_data;
}
static inline void mmc_spi_put_pdata(struct spi_device *spi) {}
#endif /* CONFIG_OF */

#endif /* __LINUX_SPI_MMC_SPI_H */
