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

#include "linux/sched.h"
#include "asm/ptrace.h"

int putreg(struct task_struct *child, unsigned long regno, 
		  unsigned long value)
{
	child->thread.process_regs.regs[regno >> 2] = value;
	return 0;
}

int poke_user(struct task_struct *child, long addr, long data)
{
	if ((addr & 3) || addr < 0)
		return -EIO;

	if (addr < MAX_REG_OFFSET)
		return putreg(child, addr, data);

	else if((addr >= offsetof(struct user, u_debugreg[0])) &&
		(addr <= offsetof(struct user, u_debugreg[7]))){
		  addr -= offsetof(struct user, u_debugreg[0]);
		  addr = addr >> 2;
		  if((addr == 4) || (addr == 5)) return -EIO;
		  child->thread.arch.debugregs[addr] = data;
		  return 0;
	}
	return -EIO;
}

unsigned long getreg(struct task_struct *child, unsigned long regno)
{
	unsigned long retval = ~0UL;

	retval &= child->thread.process_regs.regs[regno >> 2];
	return retval;
}

int peek_user(struct task_struct *child, long addr, long data)
{
	/* read the word at location addr in the USER area. */
	unsigned long tmp;

	if ((addr & 3) || addr < 0)
		return -EIO;

	tmp = 0;  /* Default return condition */
	if(addr < MAX_REG_OFFSET){
		tmp = getreg(child, addr);
	}
	else if((addr >= offsetof(struct user, u_debugreg[0])) &&
		(addr <= offsetof(struct user, u_debugreg[7]))){
		addr -= offsetof(struct user, u_debugreg[0]);
		addr = addr >> 2;
		tmp = child->thread.arch.debugregs[addr];
	}
	return put_user(tmp, (unsigned long *) data);
}

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * Emacs will notice this stuff at the end of the file and automatically
 * adjust the settings for this buffer only.  This must remain at the end
 * of the file.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-file-style: "linux"
 * End:
 */
