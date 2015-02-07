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

u32 scx200_gpio_configure(unsigned index, u32 set, u32 clear);

extern unsigned scx200_gpio_base;
extern unsigned long scx200_gpio_shadow[2];
extern struct nsc_gpio_ops scx200_gpio_ops;

#define scx200_gpio_present() (scx200_gpio_base!=0)

/* Definitions to make sure I do the same thing in all functions */
#define __SCx200_GPIO_BANK unsigned bank = index>>5
#define __SCx200_GPIO_IOADDR unsigned short ioaddr = scx200_gpio_base+0x10*bank
#define __SCx200_GPIO_SHADOW unsigned long *shadow = scx200_gpio_shadow+bank
#define __SCx200_GPIO_INDEX index &= 31

#define __SCx200_GPIO_OUT __asm__ __volatile__("outsl":"=mS" (shadow):"d" (ioaddr), "0" (shadow))

/* returns the value of the GPIO pin */

static inline int scx200_gpio_get(unsigned index) {
	__SCx200_GPIO_BANK;
	__SCx200_GPIO_IOADDR + 0x04;
	__SCx200_GPIO_INDEX;
		
	return (inl(ioaddr) & (1<<index)) ? 1 : 0;
}

/* return the value driven on the GPIO signal (the value that will be
   driven if the GPIO is configured as an output, it might not be the
   state of the GPIO right now if the GPIO is configured as an input) */

static inline int scx200_gpio_current(unsigned index) {
        __SCx200_GPIO_BANK;
	__SCx200_GPIO_INDEX;
		
	return (scx200_gpio_shadow[bank] & (1<<index)) ? 1 : 0;
}

/* drive the GPIO signal high */

static inline void scx200_gpio_set_high(unsigned index) {
	__SCx200_GPIO_BANK;
	__SCx200_GPIO_IOADDR;
	__SCx200_GPIO_SHADOW;
	__SCx200_GPIO_INDEX;
	set_bit(index, shadow);	/* __set_bit()? */
	__SCx200_GPIO_OUT;
}

/* drive the GPIO signal low */

static inline void scx200_gpio_set_low(unsigned index) {
	__SCx200_GPIO_BANK;
	__SCx200_GPIO_IOADDR;
	__SCx200_GPIO_SHADOW;
	__SCx200_GPIO_INDEX;
	clear_bit(index, shadow); /* __clear_bit()? */
	__SCx200_GPIO_OUT;
}

/* drive the GPIO signal to state */

static inline void scx200_gpio_set(unsigned index, int state) {
	__SCx200_GPIO_BANK;
	__SCx200_GPIO_IOADDR;
	__SCx200_GPIO_SHADOW;
	__SCx200_GPIO_INDEX;
	if (state)
		set_bit(index, shadow);
	else
		clear_bit(index, shadow);
	__SCx200_GPIO_OUT;
}

/* toggle the GPIO signal */
static inline void scx200_gpio_change(unsigned index) {
	__SCx200_GPIO_BANK;
	__SCx200_GPIO_IOADDR;
	__SCx200_GPIO_SHADOW;
	__SCx200_GPIO_INDEX;
	change_bit(index, shadow);
	__SCx200_GPIO_OUT;
}

#undef __SCx200_GPIO_BANK
#undef __SCx200_GPIO_IOADDR
#undef __SCx200_GPIO_SHADOW
#undef __SCx200_GPIO_INDEX
#undef __SCx200_GPIO_OUT
