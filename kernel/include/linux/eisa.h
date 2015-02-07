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

#ifndef _LINUX_EISA_H
#define _LINUX_EISA_H

#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/mod_devicetable.h>

#define EISA_MAX_SLOTS 8

#define EISA_MAX_RESOURCES 4

/* A few EISA constants/offsets... */

#define EISA_DMA1_STATUS            8
#define EISA_INT1_CTRL           0x20
#define EISA_INT1_MASK           0x21
#define EISA_INT2_CTRL           0xA0
#define EISA_INT2_MASK           0xA1
#define EISA_DMA2_STATUS         0xD0
#define EISA_DMA2_WRITE_SINGLE   0xD4
#define EISA_EXT_NMI_RESET_CTRL 0x461
#define EISA_INT1_EDGE_LEVEL    0x4D0
#define EISA_INT2_EDGE_LEVEL    0x4D1
#define EISA_VENDOR_ID_OFFSET   0xC80
#define EISA_CONFIG_OFFSET      0xC84

#define EISA_CONFIG_ENABLED         1
#define EISA_CONFIG_FORCED          2

/* There is not much we can say about an EISA device, apart from
 * signature, slot number, and base address. dma_mask is set by
 * default to parent device mask..*/

struct eisa_device {
	struct eisa_device_id id;
	int                   slot;
	int                   state;
	unsigned long         base_addr;
	struct resource       res[EISA_MAX_RESOURCES];
	u64                   dma_mask;
	struct device         dev; /* generic device */
#ifdef CONFIG_EISA_NAMES
	char		      pretty_name[50];
#endif
};

#define to_eisa_device(n) container_of(n, struct eisa_device, dev)

static inline int eisa_get_region_index (void *addr)
{
	unsigned long x = (unsigned long) addr;

	x &= 0xc00;
	return (x >> 12);
}

struct eisa_driver {
	const struct eisa_device_id *id_table;
	struct device_driver         driver;
};

#define to_eisa_driver(drv) container_of(drv,struct eisa_driver, driver)

/* These external functions are only available when EISA support is enabled. */
#ifdef CONFIG_EISA

extern struct bus_type eisa_bus_type;
int eisa_driver_register (struct eisa_driver *edrv);
void eisa_driver_unregister (struct eisa_driver *edrv);

#else /* !CONFIG_EISA */

static inline int eisa_driver_register (struct eisa_driver *edrv) { return 0; }
static inline void eisa_driver_unregister (struct eisa_driver *edrv) { }

#endif /* !CONFIG_EISA */

/* Mimics pci.h... */
static inline void *eisa_get_drvdata (struct eisa_device *edev)
{
        return edev->dev.driver_data;
}

static inline void eisa_set_drvdata (struct eisa_device *edev, void *data)
{
        edev->dev.driver_data = data;
}

/* The EISA root device. There's rumours about machines with multiple
 * busses (PA-RISC ?), so we try to handle that. */

struct eisa_root_device {
	struct device   *dev;	 /* Pointer to bridge device */
	struct resource *res;
	unsigned long    bus_base_addr;
	int		 slots;  /* Max slot number */
	int		 force_probe; /* Probe even when no slot 0 */
	u64		 dma_mask; /* from bridge device */
	int              bus_nr; /* Set by eisa_root_register */
	struct resource  eisa_root_res;	/* ditto */
};

int eisa_root_register (struct eisa_root_device *root);

#ifdef CONFIG_EISA
extern int EISA_bus;
#else
# define EISA_bus 0
#endif

#endif
