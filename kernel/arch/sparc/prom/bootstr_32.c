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
 * bootstr.c:  Boot string/argument acquisition from the PROM.
 *
 * Copyright(C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#include <linux/string.h>
#include <asm/oplib.h>
#include <linux/init.h>

#define BARG_LEN  256
static char barg_buf[BARG_LEN] = { 0 };
static char fetched __initdata = 0;

char * __init
prom_getbootargs(void)
{
	int iter;
	char *cp, *arg;

	/* This check saves us from a panic when bootfd patches args. */
	if (fetched) {
		return barg_buf;
	}

	switch(prom_vers) {
	case PROM_V0:
		cp = barg_buf;
		/* Start from 1 and go over fd(0,0,0)kernel */
		for(iter = 1; iter < 8; iter++) {
			arg = (*(romvec->pv_v0bootargs))->argv[iter];
			if(arg == 0) break;
			while(*arg != 0) {
				/* Leave place for space and null. */
				if(cp >= barg_buf + BARG_LEN-2){
					/* We might issue a warning here. */
					break;
				}
				*cp++ = *arg++;
			}
			*cp++ = ' ';
		}
		*cp = 0;
		break;
	case PROM_V2:
	case PROM_V3:
		/*
		 * V3 PROM cannot supply as with more than 128 bytes
		 * of an argument. But a smart bootstrap loader can.
		 */
		strlcpy(barg_buf, *romvec->pv_v2bootargs.bootargs, sizeof(barg_buf));
		break;
	default:
		break;
	}

	fetched = 1;
	return barg_buf;
}
