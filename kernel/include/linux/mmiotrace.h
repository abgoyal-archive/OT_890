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

#ifndef MMIOTRACE_H
#define MMIOTRACE_H

#include <linux/types.h>
#include <linux/list.h>

struct kmmio_probe;
struct pt_regs;

typedef void (*kmmio_pre_handler_t)(struct kmmio_probe *,
				struct pt_regs *, unsigned long addr);
typedef void (*kmmio_post_handler_t)(struct kmmio_probe *,
				unsigned long condition, struct pt_regs *);

struct kmmio_probe {
	struct list_head list; /* kmmio internal list */
	unsigned long addr; /* start location of the probe point */
	unsigned long len; /* length of the probe region */
	kmmio_pre_handler_t pre_handler; /* Called before addr is executed. */
	kmmio_post_handler_t post_handler; /* Called after addr is executed */
	void *private;
};

/* kmmio is active by some kmmio_probes? */
static inline int is_kmmio_active(void)
{
	extern unsigned int kmmio_count;
	return kmmio_count;
}

extern int register_kmmio_probe(struct kmmio_probe *p);
extern void unregister_kmmio_probe(struct kmmio_probe *p);

/* Called from page fault handler. */
extern int kmmio_handler(struct pt_regs *regs, unsigned long addr);

#ifdef CONFIG_MMIOTRACE
/* Called from ioremap.c */
extern void mmiotrace_ioremap(resource_size_t offset, unsigned long size,
							void __iomem *addr);
extern void mmiotrace_iounmap(volatile void __iomem *addr);

/* For anyone to insert markers. Remember trailing newline. */
extern int mmiotrace_printk(const char *fmt, ...)
				__attribute__ ((format (printf, 1, 2)));
#else
static inline void mmiotrace_ioremap(resource_size_t offset,
					unsigned long size, void __iomem *addr)
{
}

static inline void mmiotrace_iounmap(volatile void __iomem *addr)
{
}

static inline int mmiotrace_printk(const char *fmt, ...)
				__attribute__ ((format (printf, 1, 0)));

static inline int mmiotrace_printk(const char *fmt, ...)
{
	return 0;
}
#endif /* CONFIG_MMIOTRACE */

enum mm_io_opcode {
	MMIO_READ = 0x1,     /* struct mmiotrace_rw */
	MMIO_WRITE = 0x2,    /* struct mmiotrace_rw */
	MMIO_PROBE = 0x3,    /* struct mmiotrace_map */
	MMIO_UNPROBE = 0x4,  /* struct mmiotrace_map */
	MMIO_UNKNOWN_OP = 0x5, /* struct mmiotrace_rw */
};

struct mmiotrace_rw {
	resource_size_t phys;	/* PCI address of register */
	unsigned long value;
	unsigned long pc;	/* optional program counter */
	int map_id;
	unsigned char opcode;	/* one of MMIO_{READ,WRITE,UNKNOWN_OP} */
	unsigned char width;	/* size of register access in bytes */
};

struct mmiotrace_map {
	resource_size_t phys;	/* base address in PCI space */
	unsigned long virt;	/* base virtual address */
	unsigned long len;	/* mapping size */
	int map_id;
	unsigned char opcode;	/* MMIO_PROBE or MMIO_UNPROBE */
};

/* in kernel/trace/trace_mmiotrace.c */
extern void enable_mmiotrace(void);
extern void disable_mmiotrace(void);
extern void mmio_trace_rw(struct mmiotrace_rw *rw);
extern void mmio_trace_mapping(struct mmiotrace_map *map);
extern int mmio_trace_printk(const char *fmt, va_list args);

#endif /* MMIOTRACE_H */
