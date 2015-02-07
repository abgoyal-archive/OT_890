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
 * Picvue PVC160206 display driver
 *
 * Brian Murphy <brian.murphy@eicon.com>
 *
 */
struct pvc_defs {
	volatile u32 *reg;
	u32 data_shift;
	u32 data_mask;
	u32 e;
	u32 rw;
	u32 rs;
};

extern struct pvc_defs *picvue;

#define PVC_NLINES		2
#define PVC_DISPMEM		80
#define PVC_LINELEN		PVC_DISPMEM / PVC_NLINES
#define PVC_VISIBLE_CHARS	16

void pvc_write_string(const unsigned char *str, u8 addr, int line);
void pvc_write_string_centered(const unsigned char *str, int line);
void pvc_dump_string(const unsigned char *str);

#define BM_SIZE			8
#define MAX_PROGRAMMABLE_CHARS	8
int pvc_program_cg(int charnum, u8 bitmap[BM_SIZE]);

void pvc_dispcnt(u8 cmd);
#define  DISP_OFF	0
#define  DISP_ON	(1 << 2)
#define  CUR_ON		(1 << 1)
#define  CUR_BLINK	(1 << 0)

void pvc_move(u8 cmd);
#define  DISPLAY	(1 << 3)
#define  CURSOR		0
#define  RIGHT		(1 << 2)
#define  LEFT		0

void pvc_clear(void);
void pvc_home(void);

