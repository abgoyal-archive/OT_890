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

#ifndef __PCR_H
#define __PCR_H

struct pcr_ops {
	u64 (*read)(void);
	void (*write)(u64);
};
extern const struct pcr_ops *pcr_ops;

extern void deferred_pcr_work_irq(int irq, struct pt_regs *regs);
extern void schedule_deferred_pcr_work(void);

#define PCR_PIC_PRIV		0x00000001 /* PIC access is privileged */
#define PCR_STRACE		0x00000002 /* Trace supervisor events  */
#define PCR_UTRACE		0x00000004 /* Trace user events        */
#define PCR_N2_HTRACE		0x00000008 /* Trace hypervisor events  */
#define PCR_N2_TOE_OV0		0x00000010 /* Trap if PIC 0 overflows  */
#define PCR_N2_TOE_OV1		0x00000020 /* Trap if PIC 1 overflows  */
#define PCR_N2_MASK0		0x00003fc0
#define PCR_N2_MASK0_SHIFT	6
#define PCR_N2_SL0		0x0003c000
#define PCR_N2_SL0_SHIFT	14
#define PCR_N2_OV0		0x00040000
#define PCR_N2_MASK1		0x07f80000
#define PCR_N2_MASK1_SHIFT	19
#define PCR_N2_SL1		0x78000000
#define PCR_N2_SL1_SHIFT	27
#define PCR_N2_OV1		0x80000000

extern unsigned int picl_shift;

/* In order to commonize as much of the implementation as
 * possible, we use PICH as our counter.  Mostly this is
 * to accomodate Niagara-1 which can only count insn cycles
 * in PICH.
 */
static inline u64 picl_value(unsigned int nmi_hz)
{
	u32 delta = local_cpu_data().clock_tick / (nmi_hz << picl_shift);

	return ((u64)((0 - delta) & 0xffffffff)) << 32;
}

extern u64 pcr_enable;

#endif /* __PCR_H */
