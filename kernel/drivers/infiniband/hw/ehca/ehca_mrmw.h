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
 *  IBM eServer eHCA Infiniband device driver for Linux on POWER
 *
 *  MR/MW declarations and inline functions
 *
 *  Authors: Dietmar Decker <ddecker@de.ibm.com>
 *           Christoph Raisch <raisch@de.ibm.com>
 *
 *  Copyright (c) 2005 IBM Corporation
 *
 *  All rights reserved.
 *
 *  This source code is distributed under a dual license of GPL v2.0 and OpenIB
 *  BSD.
 *
 * OpenIB BSD License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials
 * provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _EHCA_MRMW_H_
#define _EHCA_MRMW_H_

int ehca_reg_mr(struct ehca_shca *shca,
		struct ehca_mr *e_mr,
		u64 *iova_start,
		u64 size,
		int acl,
		struct ehca_pd *e_pd,
		struct ehca_mr_pginfo *pginfo,
		u32 *lkey,
		u32 *rkey);

int ehca_reg_mr_rpages(struct ehca_shca *shca,
		       struct ehca_mr *e_mr,
		       struct ehca_mr_pginfo *pginfo);

int ehca_rereg_mr(struct ehca_shca *shca,
		  struct ehca_mr *e_mr,
		  u64 *iova_start,
		  u64 size,
		  int mr_access_flags,
		  struct ehca_pd *e_pd,
		  struct ehca_mr_pginfo *pginfo,
		  u32 *lkey,
		  u32 *rkey);

int ehca_unmap_one_fmr(struct ehca_shca *shca,
		       struct ehca_mr *e_fmr);

int ehca_reg_smr(struct ehca_shca *shca,
		 struct ehca_mr *e_origmr,
		 struct ehca_mr *e_newmr,
		 u64 *iova_start,
		 int acl,
		 struct ehca_pd *e_pd,
		 u32 *lkey,
		 u32 *rkey);

int ehca_reg_internal_maxmr(struct ehca_shca *shca,
			    struct ehca_pd *e_pd,
			    struct ehca_mr **maxmr);

int ehca_reg_maxmr(struct ehca_shca *shca,
		   struct ehca_mr *e_newmr,
		   u64 *iova_start,
		   int acl,
		   struct ehca_pd *e_pd,
		   u32 *lkey,
		   u32 *rkey);

int ehca_dereg_internal_maxmr(struct ehca_shca *shca);

int ehca_mr_chk_buf_and_calc_size(struct ib_phys_buf *phys_buf_array,
				  int num_phys_buf,
				  u64 *iova_start,
				  u64 *size);

int ehca_fmr_check_page_list(struct ehca_mr *e_fmr,
			     u64 *page_list,
			     int list_len);

int ehca_set_pagebuf(struct ehca_mr_pginfo *pginfo,
		     u32 number,
		     u64 *kpage);

int ehca_mr_is_maxmr(u64 size,
		     u64 *iova_start);

void ehca_mrmw_map_acl(int ib_acl,
		       u32 *hipz_acl);

void ehca_mrmw_set_pgsize_hipz_acl(u32 pgsize, u32 *hipz_acl);

void ehca_mrmw_reverse_map_acl(const u32 *hipz_acl,
			       int *ib_acl);

void ehca_mr_deletenew(struct ehca_mr *mr);

#endif  /*_EHCA_MRMW_H_*/
