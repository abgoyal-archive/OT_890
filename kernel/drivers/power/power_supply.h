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
 *  Functions private to power supply class
 *
 *  Copyright © 2007  Anton Vorontsov <cbou@mail.ru>
 *  Copyright © 2004  Szabolcs Gyurko
 *  Copyright © 2003  Ian Molton <spyro@f2s.com>
 *
 *  Modified: 2004, Oct     Szabolcs Gyurko
 *
 *  You may use this code as per GPL version 2
 */

#ifdef CONFIG_SYSFS

extern int power_supply_create_attrs(struct power_supply *psy);
extern void power_supply_remove_attrs(struct power_supply *psy);
extern int power_supply_uevent(struct device *dev, struct kobj_uevent_env *env);

#else

static inline int power_supply_create_attrs(struct power_supply *psy)
{ return 0; }
static inline void power_supply_remove_attrs(struct power_supply *psy) {}
#define power_supply_uevent NULL

#endif /* CONFIG_SYSFS */

#ifdef CONFIG_LEDS_TRIGGERS

extern void power_supply_update_leds(struct power_supply *psy);
extern int power_supply_create_triggers(struct power_supply *psy);
extern void power_supply_remove_triggers(struct power_supply *psy);

#else

static inline void power_supply_update_leds(struct power_supply *psy) {}
static inline int power_supply_create_triggers(struct power_supply *psy)
{ return 0; }
static inline void power_supply_remove_triggers(struct power_supply *psy) {}

#endif /* CONFIG_LEDS_TRIGGERS */
