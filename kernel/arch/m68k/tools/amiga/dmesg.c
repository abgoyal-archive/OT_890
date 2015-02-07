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
 *  linux/arch/m68k/tools/amiga/dmesg.c -- Retrieve the kernel messages stored
 *					   in Chip RAM with the kernel command
 *					   line option `debug=mem'.
 *
 *  © Copyright 1996 by Geert Uytterhoeven <geert@linux-m68k.org>
 *
 *
 *  Usage:
 *
 *	dmesg
 *	dmesg <CHIPMEM_END>
 *
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of the Linux
 *  distribution for more details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define CHIPMEM_START	0x00000000
#define CHIPMEM_END	0x00200000	/* overridden by argv[1] */

#define SAVEKMSG_MAGIC1	0x53415645	/* 'SAVE' */
#define SAVEKMSG_MAGIC2	0x4B4D5347	/* 'KMSG' */

struct savekmsg {
    u_long magic1;	/* SAVEKMSG_MAGIC1 */
    u_long magic2;	/* SAVEKMSG_MAGIC2 */
    u_long magicptr;	/* address of magic1 */
    u_long size;
    char data[0];
};


int main(int argc, char *argv[])
{
    u_long start = CHIPMEM_START, end = CHIPMEM_END, p;
    int found = 0;
    struct savekmsg *m = NULL;

    if (argc >= 2)
	end = strtoul(argv[1], NULL, 0);
    printf("Searching for SAVEKMSG magic...\n");
    for (p = start; p <= end-sizeof(struct savekmsg); p += 4) {
	m = (struct savekmsg *)p;
	if ((m->magic1 == SAVEKMSG_MAGIC1) && (m->magic2 == SAVEKMSG_MAGIC2) &&
	    (m->magicptr == p)) {
	    found = 1;
	    break;
	}
    }
    if (!found)
	printf("Not found\n");
    else {
	printf("Found %ld bytes at 0x%08lx\n", m->size, (u_long)&m->data);
	puts(">>>>>>>>>>>>>>>>>>>>");
	fflush(stdout);
	write(1, &m->data, m->size);
	fflush(stdout);
	puts("<<<<<<<<<<<<<<<<<<<<");
    }
    return(0);
}
