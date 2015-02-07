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

#ifndef _ASM_IA64_KEXEC_H
#define _ASM_IA64_KEXEC_H


/* Maximum physical address we can use pages from */
#define KEXEC_SOURCE_MEMORY_LIMIT (-1UL)
/* Maximum address we can reach in physical address mode */
#define KEXEC_DESTINATION_MEMORY_LIMIT (-1UL)
/* Maximum address we can use for the control code buffer */
#define KEXEC_CONTROL_MEMORY_LIMIT TASK_SIZE

#define KEXEC_CONTROL_PAGE_SIZE (8192 + 8192 + 4096)

/* The native architecture */
#define KEXEC_ARCH KEXEC_ARCH_IA_64

#define kexec_flush_icache_page(page) do { \
                unsigned long page_addr = (unsigned long)page_address(page); \
                flush_icache_range(page_addr, page_addr + PAGE_SIZE); \
        } while(0)

extern struct kimage *ia64_kimage;
extern const unsigned int relocate_new_kernel_size;
extern void relocate_new_kernel(unsigned long, unsigned long,
		struct ia64_boot_param *, unsigned long);
static inline void
crash_setup_regs(struct pt_regs *newregs, struct pt_regs *oldregs)
{
}
extern struct resource efi_memmap_res;
extern struct resource boot_param_res;
extern void kdump_smp_send_stop(void);
extern void kdump_smp_send_init(void);
extern void kexec_disable_iosapic(void);
extern void crash_save_this_cpu(void);
struct rsvd_region;
extern unsigned long kdump_find_rsvd_region(unsigned long size,
		struct rsvd_region *rsvd_regions, int n);
extern void kdump_cpu_freeze(struct unw_frame_info *info, void *arg);
extern int kdump_status[];
extern atomic_t kdump_cpu_freezed;
extern atomic_t kdump_in_progress;

#endif /* _ASM_IA64_KEXEC_H */
