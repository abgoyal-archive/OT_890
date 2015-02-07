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

/*********************************************************************
 *                
 * Filename:      irqueue.h
 * Version:       0.3
 * Description:   General queue implementation
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Tue Jun  9 13:26:50 1998
 * Modified at:   Thu Oct  7 13:25:16 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (C) 1998-1999, Aage Kvalnes <aage@cs.uit.no>
 *     Copyright (c) 1998, Dag Brattli
 *     All Rights Reserved.
 *      
 *     This code is taken from the Vortex Operating System written by Aage
 *     Kvalnes and has been ported to Linux and Linux/IR by Dag Brattli
 *
 *     This program is free software; you can redistribute it and/or 
 *     modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation; either version 2 of 
 *     the License, or (at your option) any later version.
 *  
 *     Neither Dag Brattli nor University of Tromsø admit liability nor
 *     provide warranty for any of this software. This material is 
 *     provided "AS-IS" and at no charge.
 *     
 ********************************************************************/

#include <linux/types.h>
#include <linux/spinlock.h>

#ifndef IRDA_QUEUE_H
#define IRDA_QUEUE_H

#define NAME_SIZE      32

/*
 * Hash types (some flags can be xored)
 * See comments in irqueue.c for which one to use...
 */
#define HB_NOLOCK	0	/* No concurent access prevention */
#define HB_LOCK		1	/* Prevent concurent write with global lock */

/*
 * Hash defines
 */
#define HASHBIN_SIZE   8
#define HASHBIN_MASK   0x7

#ifndef IRDA_ALIGN 
#define IRDA_ALIGN __attribute__((aligned))
#endif

#define Q_NULL { NULL, NULL, "", 0 }

typedef void (*FREE_FUNC)(void *arg);

struct irda_queue {
	struct irda_queue *q_next;
	struct irda_queue *q_prev;

	char   q_name[NAME_SIZE];
	long   q_hash;			/* Must be able to cast a (void *) */
};
typedef struct irda_queue irda_queue_t;

typedef struct hashbin_t {
	__u32      magic;
	int        hb_type;
	int        hb_size;
	spinlock_t hb_spinlock;		/* HB_LOCK - Can be used by the user */

	irda_queue_t* hb_queue[HASHBIN_SIZE] IRDA_ALIGN;

	irda_queue_t* hb_current;
} hashbin_t;

hashbin_t *hashbin_new(int type);
int      hashbin_delete(hashbin_t* hashbin, FREE_FUNC func);
int      hashbin_clear(hashbin_t* hashbin, FREE_FUNC free_func);
void     hashbin_insert(hashbin_t* hashbin, irda_queue_t* entry, long hashv, 
			const char* name);
void*    hashbin_remove(hashbin_t* hashbin, long hashv, const char* name);
void*    hashbin_remove_first(hashbin_t *hashbin);
void*	 hashbin_remove_this( hashbin_t* hashbin, irda_queue_t* entry);
void*    hashbin_find(hashbin_t* hashbin, long hashv, const char* name);
void*    hashbin_lock_find(hashbin_t* hashbin, long hashv, const char* name);
void*    hashbin_find_next(hashbin_t* hashbin, long hashv, const char* name,
			   void ** pnext);
irda_queue_t *hashbin_get_first(hashbin_t *hashbin);
irda_queue_t *hashbin_get_next(hashbin_t *hashbin);

#define HASHBIN_GET_SIZE(hashbin) hashbin->hb_size

#endif
