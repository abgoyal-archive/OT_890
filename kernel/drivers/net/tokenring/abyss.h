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
 * abyss.h: Header for the abyss tms380tr module
 *
 * Authors:
 * - Adam Fritzler
 */

#ifndef __LINUX_MADGETR_H
#define __LINUX_MADGETR_H

#ifdef __KERNEL__

/*
 * For Madge Smart 16/4 PCI Mk2.  Since we increment the base address
 * to get everything correct for the TMS SIF, we do these as negatives
 * as they fall below the SIF in addressing.
 */
#define PCIBM2_INT_STATUS_REG          ((short)-15)/* 0x01 */
#define PCIBM2_INT_CONTROL_REG         ((short)-14)/* 0x02 */
#define PCIBM2_RESET_REG               ((short)-12)/* 0x04 */
#define PCIBM2_SEEPROM_REG             ((short)-9) /* 0x07 */

#define PCIBM2_INT_CONTROL_REG_SINTEN           0x02
#define PCIBM2_INT_CONTROL_REG_PCI_ERR_ENABLE   0x80
#define PCIBM2_INT_STATUS_REG_PCI_ERR           0x80

#define PCIBM2_RESET_REG_CHIP_NRES              0x01
#define PCIBM2_RESET_REG_FIFO_NRES              0x02
#define PCIBM2_RESET_REG_SIF_NRES               0x04

#define PCIBM2_FIFO_THRESHOLD   0x21
#define PCIBM2_BURST_LENGTH     0x22

/*
 * Bits in PCIBM2_SEEPROM_REG.
 */
#define AT24_ENABLE             0x04
#define AT24_DATA               0x02
#define AT24_CLOCK              0x01

/*
 * AT24 Commands.
 */
#define AT24_WRITE              0xA0
#define AT24_READ               0xA1

/*
 * Addresses in AT24 SEEPROM.
 */
#define PCIBM2_SEEPROM_BIA          0x12
#define PCIBM2_SEEPROM_RING_SPEED   0x18
#define PCIBM2_SEEPROM_RAM_SIZE     0x1A
#define PCIBM2_SEEPROM_HWF1         0x1C
#define PCIBM2_SEEPROM_HWF2         0x1E


#endif /* __KERNEL__ */
#endif /* __LINUX_MADGETR_H */
