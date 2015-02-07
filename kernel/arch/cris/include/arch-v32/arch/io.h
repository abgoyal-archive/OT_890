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

#ifndef _ASM_ARCH_CRIS_IO_H
#define _ASM_ARCH_CRIS_IO_H

#include <linux/spinlock.h>
#include <hwregs/reg_map.h>
#include <hwregs/reg_rdwr.h>
#include <hwregs/gio_defs.h>

enum crisv32_io_dir
{
  crisv32_io_dir_in = 0,
  crisv32_io_dir_out = 1
};

struct crisv32_ioport
{
  volatile unsigned long *oe;
  volatile unsigned long *data;
  volatile unsigned long *data_in;
  unsigned int pin_count;
  spinlock_t lock;
};

struct crisv32_iopin
{
  struct crisv32_ioport* port;
  int bit;
};

extern struct crisv32_ioport crisv32_ioports[];

extern struct crisv32_iopin crisv32_led1_green;
extern struct crisv32_iopin crisv32_led1_red;
extern struct crisv32_iopin crisv32_led2_green;
extern struct crisv32_iopin crisv32_led2_red;
extern struct crisv32_iopin crisv32_led3_green;
extern struct crisv32_iopin crisv32_led3_red;

extern struct crisv32_iopin crisv32_led_net0_green;
extern struct crisv32_iopin crisv32_led_net0_red;
extern struct crisv32_iopin crisv32_led_net1_green;
extern struct crisv32_iopin crisv32_led_net1_red;

static inline void crisv32_io_set(struct crisv32_iopin *iopin, int val)
{
	unsigned long flags;
	spin_lock_irqsave(&iopin->port->lock, flags);

	if (val)
		*iopin->port->data |= iopin->bit;
	else
		*iopin->port->data &= ~iopin->bit;

	spin_unlock_irqrestore(&iopin->port->lock, flags);
}

static inline void crisv32_io_set_dir(struct crisv32_iopin* iopin,
			       enum crisv32_io_dir dir)
{
	unsigned long flags;
	spin_lock_irqsave(&iopin->port->lock, flags);

	if (dir == crisv32_io_dir_in)
		*iopin->port->oe &= ~iopin->bit;
	else
		*iopin->port->oe |= iopin->bit;

	spin_unlock_irqrestore(&iopin->port->lock, flags);
}

static inline int crisv32_io_rd(struct crisv32_iopin* iopin)
{
	return ((*iopin->port->data_in & iopin->bit) ? 1 : 0);
}

int crisv32_io_get(struct crisv32_iopin* iopin,
                   unsigned int port, unsigned int pin);
int crisv32_io_get_name(struct crisv32_iopin* iopin,
			const char *name);

#define CRIS_LED_OFF    0x00
#define CRIS_LED_GREEN  0x01
#define CRIS_LED_RED    0x02
#define CRIS_LED_ORANGE (CRIS_LED_GREEN | CRIS_LED_RED)

#if (defined(CONFIG_ETRAX_NBR_LED_GRP_ONE) || defined(CONFIG_ETRAX_NBR_LED_GRP_TWO))
#define CRIS_LED_NETWORK_GRP0_SET(x)                          \
	do {                                             \
		CRIS_LED_NETWORK_GRP0_SET_G((x) & CRIS_LED_GREEN); \
		CRIS_LED_NETWORK_GRP0_SET_R((x) & CRIS_LED_RED);   \
	} while (0)
#else
#define CRIS_LED_NETWORK_GRP0_SET(x) while (0) {}
#endif

#define CRIS_LED_NETWORK_GRP0_SET_G(x) \
	crisv32_io_set(&crisv32_led_net0_green, !(x));

#define CRIS_LED_NETWORK_GRP0_SET_R(x) \
	crisv32_io_set(&crisv32_led_net0_red, !(x));

#if defined(CONFIG_ETRAX_NBR_LED_GRP_TWO)
#define CRIS_LED_NETWORK_GRP1_SET(x)                          \
	do {                                             \
		CRIS_LED_NETWORK_GRP1_SET_G((x) & CRIS_LED_GREEN); \
		CRIS_LED_NETWORK_GRP1_SET_R((x) & CRIS_LED_RED);   \
	} while (0)
#else
#define CRIS_LED_NETWORK_GRP1_SET(x) while (0) {}
#endif

#define CRIS_LED_NETWORK_GRP1_SET_G(x) \
	crisv32_io_set(&crisv32_led_net1_green, !(x));

#define CRIS_LED_NETWORK_GRP1_SET_R(x) \
	crisv32_io_set(&crisv32_led_net1_red, !(x));

#define CRIS_LED_ACTIVE_SET(x)                           \
	do {                                        \
		CRIS_LED_ACTIVE_SET_G((x) & CRIS_LED_GREEN);  \
		CRIS_LED_ACTIVE_SET_R((x) & CRIS_LED_RED);    \
	} while (0)

#define CRIS_LED_ACTIVE_SET_G(x) \
	crisv32_io_set(&crisv32_led2_green, !(x));
#define CRIS_LED_ACTIVE_SET_R(x) \
	crisv32_io_set(&crisv32_led2_red, !(x));
#define CRIS_LED_DISK_WRITE(x) \
         do{\
		crisv32_io_set(&crisv32_led3_green, !(x)); \
		crisv32_io_set(&crisv32_led3_red, !(x));   \
        }while(0)
#define CRIS_LED_DISK_READ(x) \
	crisv32_io_set(&crisv32_led3_green, !(x));

#endif
