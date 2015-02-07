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
 * Copyright (C) 2000 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
 */

#ifndef __SYS_SIGCONTEXT_PPC_H
#define __SYS_SIGCONTEXT_PPC_H

#define DSISR_WRITE 0x02000000

#define SC_FAULT_ADDR(sc) ({ \
		struct sigcontext *_sc = (sc); \
		long retval = -1; \
		switch (_sc->regs->trap) { \
		case 0x300: \
			/* data exception */ \
			retval = _sc->regs->dar; \
			break; \
		case 0x400: \
			/* instruction exception */ \
			retval = _sc->regs->nip; \
			break; \
		default: \
			panic("SC_FAULT_ADDR: unhandled trap type\n"); \
		} \
		retval; \
	})

#define SC_FAULT_WRITE(sc) ({ \
		struct sigcontext *_sc = (sc); \
		long retval = -1; \
		switch (_sc->regs->trap) { \
		case 0x300: \
			/* data exception */ \
			retval = !!(_sc->regs->dsisr & DSISR_WRITE); \
			break; \
		case 0x400: \
			/* instruction exception: not a write */ \
			retval = 0; \
			break; \
		default: \
			panic("SC_FAULT_ADDR: unhandled trap type\n"); \
		} \
		retval; \
	})

#define SC_IP(sc) ((sc)->regs->nip)
#define SC_SP(sc) ((sc)->regs->gpr[1])
#define SEGV_IS_FIXABLE(sc) (1)

#endif

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
