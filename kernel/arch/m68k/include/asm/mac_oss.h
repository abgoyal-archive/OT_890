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
 *	OSS
 *
 *	This is used in place of VIA2 on the IIfx.
 */

#define OSS_BASE	(0x50f1a000)

/*
 * Interrupt level offsets for mac_oss->irq_level
 */

#define OSS_NUBUS0	0
#define OSS_NUBUS1	1
#define OSS_NUBUS2	2
#define OSS_NUBUS3	3
#define OSS_NUBUS4	4
#define OSS_NUBUS5	5
#define OSS_IOPISM	6
#define OSS_IOPSCC	7
#define OSS_SOUND	8
#define OSS_SCSI	9
#define OSS_60HZ	10
#define OSS_VIA1	11
#define OSS_UNUSED1	12
#define OSS_UNUSED2	13
#define OSS_PARITY	14
#define OSS_UNUSED3	15

#define OSS_NUM_SOURCES	16

/*
 * Pending interrupt bits in mac_oss->irq_pending
 */

#define OSS_IP_NUBUS0	0x0001
#define OSS_IP_NUBUS1	0x0002
#define OSS_IP_NUBUS2	0x0004
#define OSS_IP_NUBUS3	0x0008
#define OSS_IP_NUBUS4	0x0010
#define OSS_IP_NUBUS5	0x0020
#define OSS_IP_IOPISM	0x0040
#define OSS_IP_IOPSCC	0x0080
#define OSS_IP_SOUND	0x0100
#define OSS_IP_SCSI	0x0200
#define OSS_IP_60HZ	0x0400
#define OSS_IP_VIA1	0x0800
#define OSS_IP_UNUSED1	0x1000
#define OSS_IP_UNUSED2	0x2000
#define OSS_IP_PARITY	0x4000
#define OSS_IP_UNUSED3	0x8000

#define OSS_IP_NUBUS (OSS_IP_NUBUS0|OSS_IP_NUBUS1|OSS_IP_NUBUS2|OSS_IP_NUBUS3|OSS_IP_NUBUS4|OSS_IP_NUBUS5)

/*
 * Rom Control Register
 */

#define OSS_POWEROFF	0x80

/*
 * OSS Interrupt levels for various sub-systems
 *
 * This mapping is layed out with two things in mind: first, we try to keep
 * things on their own levels to avoid having to do double-dispatches. Second,
 * the levels match as closely as possible the alternate IRQ mapping mode (aka
 * "A/UX mode") available on some VIA machines.
 */

#define OSS_IRQLEV_DISABLED	0
#define OSS_IRQLEV_IOPISM	1	/* ADB? */
#define OSS_IRQLEV_SCSI		IRQ_AUTO_2
#define OSS_IRQLEV_NUBUS	IRQ_AUTO_3	/* keep this on its own level */
#define OSS_IRQLEV_IOPSCC	IRQ_AUTO_4	/* matches VIA alternate mapping */
#define OSS_IRQLEV_SOUND	IRQ_AUTO_5	/* matches VIA alternate mapping */
#define OSS_IRQLEV_60HZ		6	/* matches VIA alternate mapping */
#define OSS_IRQLEV_VIA1		IRQ_AUTO_6	/* matches VIA alternate mapping */
#define OSS_IRQLEV_PARITY	7	/* matches VIA alternate mapping */

#ifndef __ASSEMBLY__

struct mac_oss {
    __u8  irq_level[0x10];	/* [0x000-0x00f] Interrupt levels */
    __u8  padding0[0x1F2];	/* [0x010-0x201] IO space filler */
    __u16 irq_pending;		/* [0x202-0x203] pending interrupts bits */
    __u8  rom_ctrl;		/* [0x204-0x204] ROM cntl reg (for poweroff) */
    __u8  padding1[0x2];	/* [0x205-0x206] currently unused by A/UX */
    __u8  ack_60hz;		/* [0x207-0x207] 60 Hz ack. */
};

extern volatile struct mac_oss *oss;
extern int oss_present;

#endif /* __ASSEMBLY__ */
