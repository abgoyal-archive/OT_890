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

#ifndef __ARCH_ORION5X_MPP_H
#define __ARCH_ORION5X_MPP_H

enum orion5x_mpp_type {
	/*
	 * This MPP is unused.
	 */
	MPP_UNUSED,

	/*
	 * This MPP pin is used as a generic GPIO pin.  Valid for
	 * MPPs 0-15 and device bus data pins 16-31.  On 5182, also
	 * valid for MPPs 16-19.
	 */
	MPP_GPIO,

	/*
	 * This MPP is used as PCIe_RST_OUTn pin.  Valid for
	 * MPP 0 only.
	 */
	MPP_PCIE_RST_OUTn,

	/*
	 * This MPP is used as PCI arbiter pin (REQn/GNTn).
	 * Valid for MPPs 0-7 only.
	 */
	MPP_PCI_ARB,

	/*
	 * This MPP is used as PCI_PMEn pin.  Valid for MPP 2 only.
	 */
	MPP_PCI_PMEn,

	/*
	 * This MPP is used as GigE half-duplex (COL, CRS) or GMII
	 * (RXERR, CRS, TXERR, TXD[7:4], RXD[7:4]) pin.  Valid for
	 * MPPs 8-19 only.
	 */
	MPP_GIGE,

	/*
	 * This MPP is used as NAND REn/WEn pin.  Valid for MPPs
	 * 4-7 and 12-17 only, and only on the 5181l/5182/5281.
	 */
	MPP_NAND,

	/*
	 * This MPP is used as a PCI clock output pin.  Valid for
	 * MPPs 6-7 only, and only on the 5181l.
	 */
	MPP_PCI_CLK,

	/*
	 * This MPP is used as a SATA presence/activity LED.
	 * Valid for MPPs 4-7 and 12-15 only, and only on the 5182.
	 */
	MPP_SATA_LED,

	/*
	 * This MPP is used as UART1 RXD/TXD/CTSn/RTSn pin.
	 * Valid for MPPs 16-19 only.
	 */
	MPP_UART,
};

struct orion5x_mpp_mode {
	int			mpp;
	enum orion5x_mpp_type	type;
};

void orion5x_mpp_conf(struct orion5x_mpp_mode *mode);


#endif
