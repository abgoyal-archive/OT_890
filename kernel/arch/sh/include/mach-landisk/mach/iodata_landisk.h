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

#ifndef __ASM_SH_IODATA_LANDISK_H
#define __ASM_SH_IODATA_LANDISK_H

/*
 * linux/include/asm-sh/landisk/iodata_landisk.h
 *
 * Copyright (C) 2000  Atom Create Engineering Co., Ltd.
 *
 * IO-DATA LANDISK support
 */

/* Box specific addresses.  */

#define PA_USB		0xa4000000	/* USB Controller M66590 */

#define PA_ATARST	0xb0000000	/* ATA/FATA Access Control Register */
#define PA_LED		0xb0000001	/* LED Control Register */
#define PA_STATUS	0xb0000002	/* Switch Status Register */
#define PA_SHUTDOWN	0xb0000003	/* Shutdown Control Register */
#define PA_PCIPME	0xb0000004	/* PCI PME Status Register */
#define PA_IMASK	0xb0000005	/* Interrupt Mask Register */
/* 2003.10.31 I-O DATA NSD NWG	add.	for shutdown port clear */
#define PA_PWRINT_CLR	0xb0000006	/* Shutdown Interrupt clear Register */

#define PA_PIDE_OFFSET	0x40		/* CF IDE Offset */
#define PA_SIDE_OFFSET	0x40		/* HDD IDE Offset */

#define IRQ_PCIINTA	5		/* PCI INTA IRQ */
#define IRQ_PCIINTB	6		/* PCI INTB IRQ */
#define IRQ_PCIINDC	7		/* PCI INTC IRQ */
#define IRQ_PCIINTD	8		/* PCI INTD IRQ */
#define IRQ_ATA		9		/* ATA IRQ */
#define IRQ_FATA	10		/* FATA IRQ */
#define IRQ_POWER	11		/* Power Switch IRQ */
#define IRQ_BUTTON	12		/* USL-5P Button IRQ */
#define IRQ_FAULT	13		/* USL-5P Fault  IRQ */

#define __IO_PREFIX landisk
#include <asm/io_generic.h>

#endif  /* __ASM_SH_IODATA_LANDISK_H */

