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

#ifndef B43legacy_LEDS_H_
#define B43legacy_LEDS_H_

struct b43legacy_wldev;

#ifdef CONFIG_B43LEGACY_LEDS

#include <linux/types.h>
#include <linux/leds.h>


#define B43legacy_LED_MAX_NAME_LEN	31

struct b43legacy_led {
	struct b43legacy_wldev *dev;
	/* The LED class device */
	struct led_classdev led_dev;
	/* The index number of the LED. */
	u8 index;
	/* If activelow is true, the LED is ON if the
	 * bit is switched off. */
	bool activelow;
	/* The unique name string for this LED device. */
	char name[B43legacy_LED_MAX_NAME_LEN + 1];
};

#define B43legacy_LED_BEHAVIOUR		0x7F
#define B43legacy_LED_ACTIVELOW		0x80
/* LED behaviour values */
enum b43legacy_led_behaviour {
	B43legacy_LED_OFF,
	B43legacy_LED_ON,
	B43legacy_LED_ACTIVITY,
	B43legacy_LED_RADIO_ALL,
	B43legacy_LED_RADIO_A,
	B43legacy_LED_RADIO_B,
	B43legacy_LED_MODE_BG,
	B43legacy_LED_TRANSFER,
	B43legacy_LED_APTRANSFER,
	B43legacy_LED_WEIRD,
	B43legacy_LED_ASSOC,
	B43legacy_LED_INACTIVE,
};

void b43legacy_leds_init(struct b43legacy_wldev *dev);
void b43legacy_leds_exit(struct b43legacy_wldev *dev);

#else /* CONFIG_B43EGACY_LEDS */
/* LED support disabled */

struct b43legacy_led {
	/* empty */
};

static inline void b43legacy_leds_init(struct b43legacy_wldev *dev)
{
}
static inline void b43legacy_leds_exit(struct b43legacy_wldev *dev)
{
}
#endif /* CONFIG_B43LEGACY_LEDS */

#endif /* B43legacy_LEDS_H_ */
