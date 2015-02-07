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

#ifndef __LINUX_ATMEL_PWM_H
#define __LINUX_ATMEL_PWM_H

/**
 * struct pwm_channel - driver handle to a PWM channel
 * @regs: base of this channel's registers
 * @index: number of this channel (0..31)
 * @mck: base clock rate, which can be prescaled and maybe subdivided
 *
 * Drivers initialize a pwm_channel structure using pwm_channel_alloc().
 * Then they configure its clock rate (derived from MCK), alignment,
 * polarity, and duty cycle by writing directly to the channel registers,
 * before enabling the channel by calling pwm_channel_enable().
 *
 * After emitting a PWM signal for the desired length of time, drivers
 * may then pwm_channel_disable() or pwm_channel_free().  Both of these
 * disable the channel, but when it's freed the IRQ is deconfigured and
 * the channel must later be re-allocated and reconfigured.
 *
 * Note that if the period or duty cycle need to be changed while the
 * PWM channel is operating, drivers must use the PWM_CUPD double buffer
 * mechanism, either polling until they change or getting implicitly
 * notified through a once-per-period interrupt handler.
 */
struct pwm_channel {
	void __iomem	*regs;
	unsigned	index;
	unsigned long	mck;
};

extern int pwm_channel_alloc(int index, struct pwm_channel *ch);
extern int pwm_channel_free(struct pwm_channel *ch);

extern int pwm_clk_alloc(unsigned prescale, unsigned div);
extern void pwm_clk_free(unsigned clk);

extern int __pwm_channel_onoff(struct pwm_channel *ch, int enabled);

#define pwm_channel_enable(ch)	__pwm_channel_onoff((ch), 1)
#define pwm_channel_disable(ch)	__pwm_channel_onoff((ch), 0)

/* periodic interrupts, mostly for CUPD changes to period or cycle */
extern int pwm_channel_handler(struct pwm_channel *ch,
		void (*handler)(struct pwm_channel *ch));

/* per-channel registers (banked at pwm_channel->regs) */
#define PWM_CMR		0x00		/* mode register */
#define		PWM_CPR_CPD	(1 << 10)	/* set: CUPD modifies period */
#define		PWM_CPR_CPOL	(1 << 9)	/* set: idle high */
#define		PWM_CPR_CALG	(1 << 8)	/* set: center align */
#define		PWM_CPR_CPRE	(0xf << 0)	/* mask: rate is mck/(2^pre) */
#define		PWM_CPR_CLKA	(0xb << 0)	/* rate CLKA */
#define		PWM_CPR_CLKB	(0xc << 0)	/* rate CLKB */
#define PWM_CDTY	0x04		/* duty cycle (max of CPRD) */
#define PWM_CPRD	0x08		/* period (count up from zero) */
#define PWM_CCNT	0x0c		/* counter (20 bits?) */
#define PWM_CUPD	0x10		/* update CPRD (or CDTY) next period */

static inline void
pwm_channel_writel(struct pwm_channel *pwmc, unsigned offset, u32 val)
{
	__raw_writel(val, pwmc->regs + offset);
}

static inline u32 pwm_channel_readl(struct pwm_channel *pwmc, unsigned offset)
{
	return __raw_readl(pwmc->regs + offset);
}

#endif /* __LINUX_ATMEL_PWM_H */
