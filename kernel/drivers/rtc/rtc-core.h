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

#ifdef CONFIG_RTC_INTF_DEV

extern void __init rtc_dev_init(void);
extern void __exit rtc_dev_exit(void);
extern void rtc_dev_prepare(struct rtc_device *rtc);
extern void rtc_dev_add_device(struct rtc_device *rtc);
extern void rtc_dev_del_device(struct rtc_device *rtc);

#else

static inline void rtc_dev_init(void)
{
}

static inline void rtc_dev_exit(void)
{
}

static inline void rtc_dev_prepare(struct rtc_device *rtc)
{
}

static inline void rtc_dev_add_device(struct rtc_device *rtc)
{
}

static inline void rtc_dev_del_device(struct rtc_device *rtc)
{
}

#endif

#ifdef CONFIG_RTC_INTF_PROC

extern void rtc_proc_add_device(struct rtc_device *rtc);
extern void rtc_proc_del_device(struct rtc_device *rtc);

#else

static inline void rtc_proc_add_device(struct rtc_device *rtc)
{
}

static inline void rtc_proc_del_device(struct rtc_device *rtc)
{
}

#endif

#ifdef CONFIG_RTC_INTF_SYSFS

extern void __init rtc_sysfs_init(struct class *);
extern void rtc_sysfs_add_device(struct rtc_device *rtc);
extern void rtc_sysfs_del_device(struct rtc_device *rtc);

#else

static inline void rtc_sysfs_init(struct class *rtc)
{
}

static inline void rtc_sysfs_add_device(struct rtc_device *rtc)
{
}

static inline void rtc_sysfs_del_device(struct rtc_device *rtc)
{
}

#endif
