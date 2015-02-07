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

#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/gfp.h>

/*
 * Device resource management aware IRQ request/free implementation.
 */
struct irq_devres {
	unsigned int irq;
	void *dev_id;
};

static void devm_irq_release(struct device *dev, void *res)
{
	struct irq_devres *this = res;

	free_irq(this->irq, this->dev_id);
}

static int devm_irq_match(struct device *dev, void *res, void *data)
{
	struct irq_devres *this = res, *match = data;

	return this->irq == match->irq && this->dev_id == match->dev_id;
}

/**
 *	devm_request_irq - allocate an interrupt line for a managed device
 *	@dev: device to request interrupt for
 *	@irq: Interrupt line to allocate
 *	@handler: Function to be called when the IRQ occurs
 *	@irqflags: Interrupt type flags
 *	@devname: An ascii name for the claiming device
 *	@dev_id: A cookie passed back to the handler function
 *
 *	Except for the extra @dev argument, this function takes the
 *	same arguments and performs the same function as
 *	request_irq().  IRQs requested with this function will be
 *	automatically freed on driver detach.
 *
 *	If an IRQ allocated with this function needs to be freed
 *	separately, dev_free_irq() must be used.
 */
int devm_request_irq(struct device *dev, unsigned int irq,
		     irq_handler_t handler, unsigned long irqflags,
		     const char *devname, void *dev_id)
{
	struct irq_devres *dr;
	int rc;

	dr = devres_alloc(devm_irq_release, sizeof(struct irq_devres),
			  GFP_KERNEL);
	if (!dr)
		return -ENOMEM;

	rc = request_irq(irq, handler, irqflags, devname, dev_id);
	if (rc) {
		devres_free(dr);
		return rc;
	}

	dr->irq = irq;
	dr->dev_id = dev_id;
	devres_add(dev, dr);

	return 0;
}
EXPORT_SYMBOL(devm_request_irq);

/**
 *	devm_free_irq - free an interrupt
 *	@dev: device to free interrupt for
 *	@irq: Interrupt line to free
 *	@dev_id: Device identity to free
 *
 *	Except for the extra @dev argument, this function takes the
 *	same arguments and performs the same function as free_irq().
 *	This function instead of free_irq() should be used to manually
 *	free IRQs allocated with dev_request_irq().
 */
void devm_free_irq(struct device *dev, unsigned int irq, void *dev_id)
{
	struct irq_devres match_data = { irq, dev_id };

	free_irq(irq, dev_id);
	WARN_ON(devres_destroy(dev, devm_irq_release, devm_irq_match,
			       &match_data));
}
EXPORT_SYMBOL(devm_free_irq);
