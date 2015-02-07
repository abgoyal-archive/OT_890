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

#ifndef __S390_VDSO_H__
#define __S390_VDSO_H__

#ifdef __KERNEL__

/* Default link addresses for the vDSOs */
#define VDSO32_LBASE	0
#define VDSO64_LBASE	0

#define VDSO_VERSION_STRING	LINUX_2.6.26

#ifndef __ASSEMBLY__

/*
 * Note about the vdso_data and vdso_per_cpu_data structures:
 *
 * NEVER USE THEM IN USERSPACE CODE DIRECTLY. The layout of the
 * structure is supposed to be known only to the function in the vdso
 * itself and may change without notice.
 */

struct vdso_data {
	__u64 tb_update_count;		/* Timebase atomicity ctr	0x00 */
	__u64 xtime_tod_stamp;		/* TOD clock for xtime		0x08 */
	__u64 xtime_clock_sec;		/* Kernel time			0x10 */
	__u64 xtime_clock_nsec;		/*				0x18 */
	__u64 wtom_clock_sec;		/* Wall to monotonic clock	0x20 */
	__u64 wtom_clock_nsec;		/*				0x28 */
	__u32 tz_minuteswest;		/* Minutes west of Greenwich	0x30 */
	__u32 tz_dsttime;		/* Type of dst correction	0x34 */
	__u32 ectg_available;
};

struct vdso_per_cpu_data {
	__u64 ectg_timer_base;
	__u64 ectg_user_time;
};

extern struct vdso_data *vdso_data;

#ifdef CONFIG_64BIT
int vdso_alloc_per_cpu(int cpu, struct _lowcore *lowcore);
void vdso_free_per_cpu(int cpu, struct _lowcore *lowcore);
#endif

#endif /* __ASSEMBLY__ */

#endif /* __KERNEL__ */

#endif /* __S390_VDSO_H__ */
