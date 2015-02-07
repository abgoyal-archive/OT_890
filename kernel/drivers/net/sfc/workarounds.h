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

/****************************************************************************
 * Driver for Solarflare Solarstorm network controllers and boards
 * Copyright 2006-2008 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
 */

#ifndef EFX_WORKAROUNDS_H
#define EFX_WORKAROUNDS_H

/*
 * Hardware workarounds.
 * Bug numbers are from Solarflare's Bugzilla.
 */

#define EFX_WORKAROUND_ALWAYS(efx) 1
#define EFX_WORKAROUND_FALCON_A(efx) (falcon_rev(efx) <= FALCON_REV_A1)
#define EFX_WORKAROUND_10G(efx) EFX_IS10G(efx)
#define EFX_WORKAROUND_SFT9001(efx) ((efx)->phy_type == PHY_TYPE_SFT9001A || \
				     (efx)->phy_type == PHY_TYPE_SFT9001B)

/* XAUI resets if link not detected */
#define EFX_WORKAROUND_5147 EFX_WORKAROUND_ALWAYS
/* RX PCIe double split performance issue */
#define EFX_WORKAROUND_7575 EFX_WORKAROUND_ALWAYS
/* Bit-bashed I2C reads cause performance drop */
#define EFX_WORKAROUND_7884 EFX_WORKAROUND_10G
/* TX pkt parser problem with <= 16 byte TXes */
#define EFX_WORKAROUND_9141 EFX_WORKAROUND_ALWAYS
/* TX_EV_PKT_ERR can be caused by a dangling TX descriptor
 * or a PCIe error (bug 11028) */
#define EFX_WORKAROUND_10727 EFX_WORKAROUND_ALWAYS
/* Transmit flow control may get disabled */
#define EFX_WORKAROUND_11482 EFX_WORKAROUND_ALWAYS
/* Flush events can take a very long time to appear */
#define EFX_WORKAROUND_11557 EFX_WORKAROUND_ALWAYS

/* Spurious parity errors in TSORT buffers */
#define EFX_WORKAROUND_5129 EFX_WORKAROUND_FALCON_A
/* iSCSI parsing errors */
#define EFX_WORKAROUND_5583 EFX_WORKAROUND_FALCON_A
/* RX events go missing */
#define EFX_WORKAROUND_5676 EFX_WORKAROUND_FALCON_A
/* RX_RESET on A1 */
#define EFX_WORKAROUND_6555 EFX_WORKAROUND_FALCON_A
/* Increase filter depth to avoid RX_RESET */
#define EFX_WORKAROUND_7244 EFX_WORKAROUND_FALCON_A
/* Flushes may never complete */
#define EFX_WORKAROUND_7803 EFX_WORKAROUND_FALCON_A
/* Leak overlength packets rather than free */
#define EFX_WORKAROUND_8071 EFX_WORKAROUND_FALCON_A

/* Need to send XNP pages for 100BaseT */
#define EFX_WORKAROUND_13204 EFX_WORKAROUND_SFT9001
/* Don't restart AN in near-side loopback */
#define EFX_WORKAROUND_15195 EFX_WORKAROUND_SFT9001

#endif /* EFX_WORKAROUNDS_H */
