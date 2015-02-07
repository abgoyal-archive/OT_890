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

#ifndef _ASM_PARISC_PARISC_DEVICE_H_
#define _ASM_PARISC_PARISC_DEVICE_H_

#include <linux/device.h>

struct parisc_device {
	struct resource hpa;		/* Hard Physical Address */
	struct parisc_device_id id;
	struct parisc_driver *driver;	/* Driver for this device */
	char		name[80];	/* The hardware description */
	int		irq;
	int		aux_irq;	/* Some devices have a second IRQ */

	char		hw_path;        /* The module number on this bus */
	unsigned int	num_addrs;	/* some devices have additional address ranges. */
	unsigned long	*addr;          /* which will be stored here */
 
#ifdef CONFIG_64BIT
	/* parms for pdc_pat_cell_module() call */
	unsigned long	pcell_loc;	/* Physical Cell location */
	unsigned long	mod_index;	/* PAT specific - Misc Module info */

	/* generic info returned from pdc_pat_cell_module() */
	unsigned long	mod_info;	/* PAT specific - Misc Module info */
	unsigned long	pmod_loc;	/* physical Module location */
#endif
	u64		dma_mask;	/* DMA mask for I/O */
	struct device 	dev;
};

struct parisc_driver {
	struct parisc_driver *next;
	char *name; 
	const struct parisc_device_id *id_table;
	int (*probe) (struct parisc_device *dev); /* New device discovered */
	int (*remove) (struct parisc_device *dev);
	struct device_driver drv;
};


#define to_parisc_device(d)	container_of(d, struct parisc_device, dev)
#define to_parisc_driver(d)	container_of(d, struct parisc_driver, drv)
#define parisc_parent(d)	to_parisc_device(d->dev.parent)

static inline const char *parisc_pathname(struct parisc_device *d)
{
	return dev_name(&d->dev);
}

static inline void
parisc_set_drvdata(struct parisc_device *d, void *p)
{
	dev_set_drvdata(&d->dev, p);
}

static inline void *
parisc_get_drvdata(struct parisc_device *d)
{
	return dev_get_drvdata(&d->dev);
}

extern struct bus_type parisc_bus_type;

#endif /*_ASM_PARISC_PARISC_DEVICE_H_*/
