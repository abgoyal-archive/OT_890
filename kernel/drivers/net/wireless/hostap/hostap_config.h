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

#ifndef HOSTAP_CONFIG_H
#define HOSTAP_CONFIG_H

/* In the previous versions of Host AP driver, support for user space version
 * of IEEE 802.11 management (hostapd) used to be disabled in the default
 * configuration. From now on, support for hostapd is always included and it is
 * possible to disable kernel driver version of IEEE 802.11 management with a
 * separate define, PRISM2_NO_KERNEL_IEEE80211_MGMT. */
/* #define PRISM2_NO_KERNEL_IEEE80211_MGMT */

/* Maximum number of events handler per one interrupt */
#define PRISM2_MAX_INTERRUPT_EVENTS 20

/* Include code for downloading firmware images into volatile RAM. */
#define PRISM2_DOWNLOAD_SUPPORT

/* Allow kernel configuration to enable download support. */
#if !defined(PRISM2_DOWNLOAD_SUPPORT) && defined(CONFIG_HOSTAP_FIRMWARE)
#define PRISM2_DOWNLOAD_SUPPORT
#endif

/* Allow kernel configuration to enable non-volatile download support. */
#ifdef CONFIG_HOSTAP_FIRMWARE_NVRAM
#define PRISM2_NON_VOLATILE_DOWNLOAD
#endif

/* Save low-level I/O for debugging. This should not be enabled in normal use.
 */
/* #define PRISM2_IO_DEBUG */

/* Following defines can be used to remove unneeded parts of the driver, e.g.,
 * to limit the size of the kernel module. Definitions can be added here in
 * hostap_config.h or they can be added to make command with EXTRA_CFLAGS,
 * e.g.,
 * 'make pccard EXTRA_CFLAGS="-DPRISM2_NO_DEBUG -DPRISM2_NO_PROCFS_DEBUG"'
 */

/* Do not include debug messages into the driver */
/* #define PRISM2_NO_DEBUG */

/* Do not include /proc/net/prism2/wlan#/{registers,debug} */
/* #define PRISM2_NO_PROCFS_DEBUG */

/* Do not include station functionality (i.e., allow only Master (Host AP) mode
 */
/* #define PRISM2_NO_STATION_MODES */

#endif /* HOSTAP_CONFIG_H */
