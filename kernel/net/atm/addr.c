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

/* net/atm/addr.c - Local ATM address registry */

/* Written 1995-2000 by Werner Almesberger, EPFL LRC/ICA */

#include <linux/atm.h>
#include <linux/atmdev.h>
#include <asm/uaccess.h>

#include "signaling.h"
#include "addr.h"

static int check_addr(const struct sockaddr_atmsvc *addr)
{
	int i;

	if (addr->sas_family != AF_ATMSVC)
		return -EAFNOSUPPORT;
	if (!*addr->sas_addr.pub)
		return *addr->sas_addr.prv ? 0 : -EINVAL;
	for (i = 1; i < ATM_E164_LEN + 1; i++)	/* make sure it's \0-terminated */
		if (!addr->sas_addr.pub[i])
			return 0;
	return -EINVAL;
}

static int identical(const struct sockaddr_atmsvc *a, const struct sockaddr_atmsvc *b)
{
	if (*a->sas_addr.prv)
		if (memcmp(a->sas_addr.prv, b->sas_addr.prv, ATM_ESA_LEN))
			return 0;
	if (!*a->sas_addr.pub)
		return !*b->sas_addr.pub;
	if (!*b->sas_addr.pub)
		return 0;
	return !strcmp(a->sas_addr.pub, b->sas_addr.pub);
}

static void notify_sigd(const struct atm_dev *dev)
{
	struct sockaddr_atmpvc pvc;

	pvc.sap_addr.itf = dev->number;
	sigd_enq(NULL, as_itf_notify, NULL, &pvc, NULL);
}

void atm_reset_addr(struct atm_dev *dev, enum atm_addr_type_t atype)
{
	unsigned long flags;
	struct atm_dev_addr *this, *p;
	struct list_head *head;

	spin_lock_irqsave(&dev->lock, flags);
	if (atype == ATM_ADDR_LECS)
		head = &dev->lecs;
	else
		head = &dev->local;
	list_for_each_entry_safe(this, p, head, entry) {
		list_del(&this->entry);
		kfree(this);
	}
	spin_unlock_irqrestore(&dev->lock, flags);
	if (head == &dev->local)
		notify_sigd(dev);
}

int atm_add_addr(struct atm_dev *dev, const struct sockaddr_atmsvc *addr,
		 enum atm_addr_type_t atype)
{
	unsigned long flags;
	struct atm_dev_addr *this;
	struct list_head *head;
	int error;

	error = check_addr(addr);
	if (error)
		return error;
	spin_lock_irqsave(&dev->lock, flags);
	if (atype == ATM_ADDR_LECS)
		head = &dev->lecs;
	else
		head = &dev->local;
	list_for_each_entry(this, head, entry) {
		if (identical(&this->addr, addr)) {
			spin_unlock_irqrestore(&dev->lock, flags);
			return -EEXIST;
		}
	}
	this = kmalloc(sizeof(struct atm_dev_addr), GFP_ATOMIC);
	if (!this) {
		spin_unlock_irqrestore(&dev->lock, flags);
		return -ENOMEM;
	}
	this->addr = *addr;
	list_add(&this->entry, head);
	spin_unlock_irqrestore(&dev->lock, flags);
	if (head == &dev->local)
		notify_sigd(dev);
	return 0;
}

int atm_del_addr(struct atm_dev *dev, const struct sockaddr_atmsvc *addr,
		 enum atm_addr_type_t atype)
{
	unsigned long flags;
	struct atm_dev_addr *this;
	struct list_head *head;
	int error;

	error = check_addr(addr);
	if (error)
		return error;
	spin_lock_irqsave(&dev->lock, flags);
	if (atype == ATM_ADDR_LECS)
		head = &dev->lecs;
	else
		head = &dev->local;
	list_for_each_entry(this, head, entry) {
		if (identical(&this->addr, addr)) {
			list_del(&this->entry);
			spin_unlock_irqrestore(&dev->lock, flags);
			kfree(this);
			if (head == &dev->local)
				notify_sigd(dev);
			return 0;
		}
	}
	spin_unlock_irqrestore(&dev->lock, flags);
	return -ENOENT;
}

int atm_get_addr(struct atm_dev *dev, struct sockaddr_atmsvc __user * buf,
		 size_t size, enum atm_addr_type_t atype)
{
	unsigned long flags;
	struct atm_dev_addr *this;
	struct list_head *head;
	int total = 0, error;
	struct sockaddr_atmsvc *tmp_buf, *tmp_bufp;

	spin_lock_irqsave(&dev->lock, flags);
	if (atype == ATM_ADDR_LECS)
		head = &dev->lecs;
	else
		head = &dev->local;
	list_for_each_entry(this, head, entry)
	    total += sizeof(struct sockaddr_atmsvc);
	tmp_buf = tmp_bufp = kmalloc(total, GFP_ATOMIC);
	if (!tmp_buf) {
		spin_unlock_irqrestore(&dev->lock, flags);
		return -ENOMEM;
	}
	list_for_each_entry(this, head, entry)
	    memcpy(tmp_bufp++, &this->addr, sizeof(struct sockaddr_atmsvc));
	spin_unlock_irqrestore(&dev->lock, flags);
	error = total > size ? -E2BIG : total;
	if (copy_to_user(buf, tmp_buf, total < size ? total : size))
		error = -EFAULT;
	kfree(tmp_buf);
	return error;
}
