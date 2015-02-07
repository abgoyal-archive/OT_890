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
 * palloc.c:  Memory allocation from the Sun PROM.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#include <asm/openprom.h>
#include <asm/oplib.h>

/* You should not call these routines after memory management
 * has been initialized in the kernel, if fact you should not
 * use these if at all possible in the kernel.  They are mainly
 * to be used for a bootloader for temporary allocations which
 * it will free before jumping into the kernel it has loaded.
 *
 * Also, these routines don't work on V0 proms, only V2 and later.
 */

/* Allocate a chunk of memory of size 'num_bytes' giving a suggestion
 * of virtual_hint as the preferred virtual base address of this chunk.
 * There are no guarantees that you will get the allocation, or that
 * the prom will abide by your "hint".  So check your return value.
 */
char *
prom_alloc(char *virtual_hint, unsigned int num_bytes)
{
	if(prom_vers == PROM_V0) return (char *) 0x0;
	if(num_bytes == 0x0) return (char *) 0x0;
	return (*(romvec->pv_v2devops.v2_dumb_mem_alloc))(virtual_hint, num_bytes);
}

/* Free a previously allocated chunk back to the prom at virtual address
 * 'vaddr' of size 'num_bytes'.  NOTE: This vaddr is not the hint you
 * used for the allocation, but the virtual address the prom actually
 * returned to you.  They may be have been the same, they may have not,
 * doesn't matter.
 */
void
prom_free(char *vaddr, unsigned int num_bytes)
{
	if((prom_vers == PROM_V0) || (num_bytes == 0x0)) return;
	(*(romvec->pv_v2devops.v2_dumb_mem_free))(vaddr, num_bytes);
	return;
}
