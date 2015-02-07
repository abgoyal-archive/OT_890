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

#ifndef _UNWIND_H_
#define _UNWIND_H_

#include <linux/list.h>

/* From ABI specifications */
struct unwind_table_entry {
	unsigned int region_start;
	unsigned int region_end;
	unsigned int Cannot_unwind:1; /* 0 */
	unsigned int Millicode:1;	/* 1 */
	unsigned int Millicode_save_sr0:1;	/* 2 */
	unsigned int Region_description:2;	/* 3..4 */
	unsigned int reserved1:1;	/* 5 */
	unsigned int Entry_SR:1;	/* 6 */
	unsigned int Entry_FR:4;	/* number saved *//* 7..10 */
	unsigned int Entry_GR:5;	/* number saved *//* 11..15 */
	unsigned int Args_stored:1;	/* 16 */
	unsigned int Variable_Frame:1;	/* 17 */
	unsigned int Separate_Package_Body:1;	/* 18 */
	unsigned int Frame_Extension_Millicode:1;	/* 19 */
	unsigned int Stack_Overflow_Check:1;	/* 20 */
	unsigned int Two_Instruction_SP_Increment:1;	/* 21 */
	unsigned int Ada_Region:1;	/* 22 */
	unsigned int cxx_info:1;	/* 23 */
	unsigned int cxx_try_catch:1;	/* 24 */
	unsigned int sched_entry_seq:1;	/* 25 */
	unsigned int reserved2:1;	/* 26 */
	unsigned int Save_SP:1;	/* 27 */
	unsigned int Save_RP:1;	/* 28 */
	unsigned int Save_MRP_in_frame:1;	/* 29 */
	unsigned int extn_ptr_defined:1;	/* 30 */
	unsigned int Cleanup_defined:1;	/* 31 */
	
	unsigned int MPE_XL_interrupt_marker:1;	/* 0 */
	unsigned int HP_UX_interrupt_marker:1;	/* 1 */
	unsigned int Large_frame:1;	/* 2 */
	unsigned int Pseudo_SP_Set:1;	/* 3 */
	unsigned int reserved4:1;	/* 4 */
	unsigned int Total_frame_size:27;	/* 5..31 */
};

struct unwind_table {
	struct list_head list;
	const char *name;
	unsigned long gp;
	unsigned long base_addr;
	unsigned long start;
	unsigned long end;
	const struct unwind_table_entry *table;
	unsigned long length;
};

struct unwind_frame_info {
	struct task_struct *t;
	/* Eventually we would like to be able to get at any of the registers
	   available; but for now we only try to get the sp and ip for each
	   frame */
	/* struct pt_regs regs; */
	unsigned long sp, ip, rp, r31;
	unsigned long prev_sp, prev_ip;
};

struct unwind_table *
unwind_table_add(const char *name, unsigned long base_addr, 
		 unsigned long gp, void *start, void *end);
void
unwind_table_remove(struct unwind_table *table);

void unwind_frame_init(struct unwind_frame_info *info, struct task_struct *t, 
		       struct pt_regs *regs);
void unwind_frame_init_from_blocked_task(struct unwind_frame_info *info, struct task_struct *t);
void unwind_frame_init_running(struct unwind_frame_info *info, struct pt_regs *regs);
int unwind_once(struct unwind_frame_info *info);
int unwind_to_user(struct unwind_frame_info *info);

int unwind_init(void);

#endif
