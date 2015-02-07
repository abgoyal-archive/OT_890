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

#ifndef	__HPET__
#define	__HPET__ 1

#include <linux/compiler.h>

#ifdef __KERNEL__

/*
 * Offsets into HPET Registers
 */

struct hpet {
	u64 hpet_cap;		/* capabilities */
	u64 res0;		/* reserved */
	u64 hpet_config;	/* configuration */
	u64 res1;		/* reserved */
	u64 hpet_isr;		/* interrupt status reg */
	u64 res2[25];		/* reserved */
	union {			/* main counter */
		u64 _hpet_mc64;
		u32 _hpet_mc32;
		unsigned long _hpet_mc;
	} _u0;
	u64 res3;		/* reserved */
	struct hpet_timer {
		u64 hpet_config;	/* configuration/cap */
		union {		/* timer compare register */
			u64 _hpet_hc64;
			u32 _hpet_hc32;
			unsigned long _hpet_compare;
		} _u1;
		u64 hpet_fsb[2];	/* FSB route */
	} hpet_timers[1];
};

#define	hpet_mc		_u0._hpet_mc
#define	hpet_compare	_u1._hpet_compare

#define	HPET_MAX_TIMERS	(32)
#define	HPET_MAX_IRQ	(32)

/*
 * HPET general capabilities register
 */

#define	HPET_COUNTER_CLK_PERIOD_MASK	(0xffffffff00000000ULL)
#define	HPET_COUNTER_CLK_PERIOD_SHIFT	(32UL)
#define	HPET_VENDOR_ID_MASK		(0x00000000ffff0000ULL)
#define	HPET_VENDOR_ID_SHIFT		(16ULL)
#define	HPET_LEG_RT_CAP_MASK		(0x8000)
#define	HPET_COUNTER_SIZE_MASK		(0x2000)
#define	HPET_NUM_TIM_CAP_MASK		(0x1f00)
#define	HPET_NUM_TIM_CAP_SHIFT		(8ULL)

/*
 * HPET general configuration register
 */

#define	HPET_LEG_RT_CNF_MASK		(2UL)
#define	HPET_ENABLE_CNF_MASK		(1UL)


/*
 * Timer configuration register
 */

#define	Tn_INT_ROUTE_CAP_MASK		(0xffffffff00000000ULL)
#define	Tn_INT_ROUTE_CAP_SHIFT		(32UL)
#define	Tn_FSB_INT_DELCAP_MASK		(0x8000UL)
#define	Tn_FSB_INT_DELCAP_SHIFT		(15)
#define	Tn_FSB_EN_CNF_MASK		(0x4000UL)
#define	Tn_FSB_EN_CNF_SHIFT		(14)
#define	Tn_INT_ROUTE_CNF_MASK		(0x3e00UL)
#define	Tn_INT_ROUTE_CNF_SHIFT		(9)
#define	Tn_32MODE_CNF_MASK		(0x0100UL)
#define	Tn_VAL_SET_CNF_MASK		(0x0040UL)
#define	Tn_SIZE_CAP_MASK		(0x0020UL)
#define	Tn_PER_INT_CAP_MASK		(0x0010UL)
#define	Tn_TYPE_CNF_MASK		(0x0008UL)
#define	Tn_INT_ENB_CNF_MASK		(0x0004UL)
#define	Tn_INT_TYPE_CNF_MASK		(0x0002UL)

/*
 * Timer FSB Interrupt Route Register
 */

#define	Tn_FSB_INT_ADDR_MASK		(0xffffffff00000000ULL)
#define	Tn_FSB_INT_ADDR_SHIFT		(32UL)
#define	Tn_FSB_INT_VAL_MASK		(0x00000000ffffffffULL)

/*
 * exported interfaces
 */

struct hpet_data {
	unsigned long hd_phys_address;
	void __iomem *hd_address;
	unsigned short hd_nirqs;
	unsigned int hd_state;	/* timer allocated */
	unsigned int hd_irq[HPET_MAX_TIMERS];
};

static inline void hpet_reserve_timer(struct hpet_data *hd, int timer)
{
	hd->hd_state |= (1 << timer);
	return;
}

int hpet_alloc(struct hpet_data *);

#endif /* __KERNEL__ */

struct hpet_info {
	unsigned long hi_ireqfreq;	/* Hz */
	unsigned long hi_flags;	/* information */
	unsigned short hi_hpet;
	unsigned short hi_timer;
};

#define HPET_INFO_PERIODIC	0x0010	/* periodic-capable comparator */

#define	HPET_IE_ON	_IO('h', 0x01)	/* interrupt on */
#define	HPET_IE_OFF	_IO('h', 0x02)	/* interrupt off */
#define	HPET_INFO	_IOR('h', 0x03, struct hpet_info)
#define	HPET_EPI	_IO('h', 0x04)	/* enable periodic */
#define	HPET_DPI	_IO('h', 0x05)	/* disable periodic */
#define	HPET_IRQFREQ	_IOW('h', 0x6, unsigned long)	/* IRQFREQ usec */

#endif				/* !__HPET__ */
