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

/******************************************************************************
*******************************************************************************
**
**  Copyright (C) 2005-2007 Red Hat, Inc.  All rights reserved.
**
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
**
*******************************************************************************
******************************************************************************/

#ifndef __LOCK_DOT_H__
#define __LOCK_DOT_H__

void dlm_dump_rsb(struct dlm_rsb *r);
void dlm_print_lkb(struct dlm_lkb *lkb);
void dlm_receive_message_saved(struct dlm_ls *ls, struct dlm_message *ms);
void dlm_receive_buffer(union dlm_packet *p, int nodeid);
int dlm_modes_compat(int mode1, int mode2);
void dlm_put_rsb(struct dlm_rsb *r);
void dlm_hold_rsb(struct dlm_rsb *r);
int dlm_put_lkb(struct dlm_lkb *lkb);
void dlm_scan_rsbs(struct dlm_ls *ls);
int dlm_lock_recovery_try(struct dlm_ls *ls);
void dlm_unlock_recovery(struct dlm_ls *ls);
void dlm_scan_timeout(struct dlm_ls *ls);
void dlm_adjust_timeouts(struct dlm_ls *ls);

int dlm_purge_locks(struct dlm_ls *ls);
void dlm_purge_mstcpy_locks(struct dlm_rsb *r);
void dlm_grant_after_purge(struct dlm_ls *ls);
int dlm_recover_waiters_post(struct dlm_ls *ls);
void dlm_recover_waiters_pre(struct dlm_ls *ls);
int dlm_recover_master_copy(struct dlm_ls *ls, struct dlm_rcom *rc);
int dlm_recover_process_copy(struct dlm_ls *ls, struct dlm_rcom *rc);

int dlm_user_request(struct dlm_ls *ls, struct dlm_user_args *ua, int mode,
	uint32_t flags, void *name, unsigned int namelen,
	unsigned long timeout_cs);
int dlm_user_convert(struct dlm_ls *ls, struct dlm_user_args *ua_tmp,
	int mode, uint32_t flags, uint32_t lkid, char *lvb_in,
	unsigned long timeout_cs);
int dlm_user_unlock(struct dlm_ls *ls, struct dlm_user_args *ua_tmp,
	uint32_t flags, uint32_t lkid, char *lvb_in);
int dlm_user_cancel(struct dlm_ls *ls,  struct dlm_user_args *ua_tmp,
	uint32_t flags, uint32_t lkid);
int dlm_user_purge(struct dlm_ls *ls, struct dlm_user_proc *proc,
	int nodeid, int pid);
int dlm_user_deadlock(struct dlm_ls *ls, uint32_t flags, uint32_t lkid);
void dlm_clear_proc_locks(struct dlm_ls *ls, struct dlm_user_proc *proc);

static inline int is_master(struct dlm_rsb *r)
{
	return !r->res_nodeid;
}

static inline void lock_rsb(struct dlm_rsb *r)
{
	mutex_lock(&r->res_mutex);
}

static inline void unlock_rsb(struct dlm_rsb *r)
{
	mutex_unlock(&r->res_mutex);
}

#endif

