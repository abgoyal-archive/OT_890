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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2004 by Ralf Baechle
 *
 * The cpustart method is a PMC-Sierra's function to start the secondary CPU.
 * Stock PMON 2000 has the smpfork, semlock and semunlock methods instead.
 */
#ifndef _ASM_PMON_H
#define _ASM_PMON_H

struct callvectors {
	int	(*open) (char*, int, int);
	int	(*close) (int);
	int	(*read) (int, void*, int);
	int	(*write) (int, void*, int);
	off_t	(*lseek) (int, off_t, int);
	int	(*printf) (const char*, ...);
	void	(*cacheflush) (void);
	char*	(*gets) (char*);
	union {
		int	(*smpfork) (unsigned long cp, char *sp);
		int	(*cpustart) (long, void (*)(void), void *, long);
	} _s;
	int	(*semlock) (int sem);
	void	(*semunlock) (int sem);
};

extern struct callvectors *debug_vectors;

#define pmon_open(name, flags, mode)	debug_vectors->open(name, flage, mode)
#define pmon_close(fd)			debug_vectors->close(fd)
#define pmon_read(fd, buf, count)	debug_vectors->read(fd, buf, count)
#define pmon_write(fd, buf, count)	debug_vectors->write(fd, buf, count)
#define pmon_lseek(fd, off, whence)	debug_vectors->lseek(fd, off, whence)
#define pmon_printf(fmt...)		debug_vectors->printf(fmt)
#define pmon_cacheflush()		debug_vectors->cacheflush()
#define pmon_gets(s)			debug_vectors->gets(s)
#define pmon_cpustart(n, f, sp, gp)	debug_vectors->_s.cpustart(n, f, sp, gp)
#define pmon_smpfork(cp, sp)		debug_vectors->_s.smpfork(cp, sp)
#define pmon_semlock(sem)		debug_vectors->semlock(sem)
#define pmon_semunlock(sem)		debug_vectors->semunlock(sem)

#endif /* _ASM_PMON_H */
