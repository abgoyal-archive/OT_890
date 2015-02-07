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
 *  drivers/s390/cio/airq.c
 *    Support for adapter interruptions
 *
 *    Copyright IBM Corp. 1999,2007
 *    Author(s): Ingo Adlung <adlung@de.ibm.com>
 *		 Cornelia Huck <cornelia.huck@de.ibm.com>
 *		 Arnd Bergmann <arndb@de.ibm.com>
 *		 Peter Oberparleiter <peter.oberparleiter@de.ibm.com>
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>

#include <asm/airq.h>
#include <asm/isc.h>

#include "cio.h"
#include "cio_debug.h"

#define NR_AIRQS		32
#define NR_AIRQS_PER_WORD	sizeof(unsigned long)
#define NR_AIRQ_WORDS		(NR_AIRQS / NR_AIRQS_PER_WORD)

union indicator_t {
	unsigned long word[NR_AIRQ_WORDS];
	unsigned char byte[NR_AIRQS];
} __attribute__((packed));

struct airq_t {
	adapter_int_handler_t handler;
	void *drv_data;
};

static union indicator_t indicators[MAX_ISC];
static struct airq_t *airqs[MAX_ISC][NR_AIRQS];

static int register_airq(struct airq_t *airq, u8 isc)
{
	int i;

	for (i = 0; i < NR_AIRQS; i++)
		if (!cmpxchg(&airqs[isc][i], NULL, airq))
			return i;
	return -ENOMEM;
}

/**
 * s390_register_adapter_interrupt() - register adapter interrupt handler
 * @handler: adapter handler to be registered
 * @drv_data: driver data passed with each call to the handler
 * @isc: isc for which the handler should be called
 *
 * Returns:
 *  Pointer to the indicator to be used on success
 *  ERR_PTR() if registration failed
 */
void *s390_register_adapter_interrupt(adapter_int_handler_t handler,
				      void *drv_data, u8 isc)
{
	struct airq_t *airq;
	char dbf_txt[16];
	int ret;

	if (isc > MAX_ISC)
		return ERR_PTR(-EINVAL);
	airq = kmalloc(sizeof(struct airq_t), GFP_KERNEL);
	if (!airq) {
		ret = -ENOMEM;
		goto out;
	}
	airq->handler = handler;
	airq->drv_data = drv_data;

	ret = register_airq(airq, isc);
out:
	snprintf(dbf_txt, sizeof(dbf_txt), "rairq:%d", ret);
	CIO_TRACE_EVENT(4, dbf_txt);
	if (ret < 0) {
		kfree(airq);
		return ERR_PTR(ret);
	} else
		return &indicators[isc].byte[ret];
}
EXPORT_SYMBOL(s390_register_adapter_interrupt);

/**
 * s390_unregister_adapter_interrupt - unregister adapter interrupt handler
 * @ind: indicator for which the handler is to be unregistered
 * @isc: interruption subclass
 */
void s390_unregister_adapter_interrupt(void *ind, u8 isc)
{
	struct airq_t *airq;
	char dbf_txt[16];
	int i;

	i = (int) ((addr_t) ind) - ((addr_t) &indicators[isc].byte[0]);
	snprintf(dbf_txt, sizeof(dbf_txt), "urairq:%d", i);
	CIO_TRACE_EVENT(4, dbf_txt);
	indicators[isc].byte[i] = 0;
	airq = xchg(&airqs[isc][i], NULL);
	/*
	 * Allow interrupts to complete. This will ensure that the airq handle
	 * is no longer referenced by any interrupt handler.
	 */
	synchronize_sched();
	kfree(airq);
}
EXPORT_SYMBOL(s390_unregister_adapter_interrupt);

#define INDICATOR_MASK	(0xffUL << ((NR_AIRQS_PER_WORD - 1) * 8))

void do_adapter_IO(u8 isc)
{
	int w;
	int i;
	unsigned long word;
	struct airq_t *airq;

	/*
	 * Access indicator array in word-sized chunks to minimize storage
	 * fetch operations.
	 */
	for (w = 0; w < NR_AIRQ_WORDS; w++) {
		word = indicators[isc].word[w];
		i = w * NR_AIRQS_PER_WORD;
		/*
		 * Check bytes within word for active indicators.
		 */
		while (word) {
			if (word & INDICATOR_MASK) {
				airq = airqs[isc][i];
				if (likely(airq))
					airq->handler(&indicators[isc].byte[i],
						      airq->drv_data);
				else
					/*
					 * Reset ill-behaved indicator.
					 */
					indicators[isc].byte[i] = 0;
			}
			word <<= 8;
			i++;
		}
	}
}
