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
 * Defines an spu hypervisor abstraction layer.
 *
 *  Copyright 2006 Sony Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#if !defined(_SPU_PRIV1_H)
#define _SPU_PRIV1_H
#if defined(__KERNEL__)

#include <linux/types.h>

struct spu;
struct spu_context;

/* access to priv1 registers */

struct spu_priv1_ops {
	void (*int_mask_and) (struct spu *spu, int class, u64 mask);
	void (*int_mask_or) (struct spu *spu, int class, u64 mask);
	void (*int_mask_set) (struct spu *spu, int class, u64 mask);
	u64 (*int_mask_get) (struct spu *spu, int class);
	void (*int_stat_clear) (struct spu *spu, int class, u64 stat);
	u64 (*int_stat_get) (struct spu *spu, int class);
	void (*cpu_affinity_set) (struct spu *spu, int cpu);
	u64 (*mfc_dar_get) (struct spu *spu);
	u64 (*mfc_dsisr_get) (struct spu *spu);
	void (*mfc_dsisr_set) (struct spu *spu, u64 dsisr);
	void (*mfc_sdr_setup) (struct spu *spu);
	void (*mfc_sr1_set) (struct spu *spu, u64 sr1);
	u64 (*mfc_sr1_get) (struct spu *spu);
	void (*mfc_tclass_id_set) (struct spu *spu, u64 tclass_id);
	u64 (*mfc_tclass_id_get) (struct spu *spu);
	void (*tlb_invalidate) (struct spu *spu);
	void (*resource_allocation_groupID_set) (struct spu *spu, u64 id);
	u64 (*resource_allocation_groupID_get) (struct spu *spu);
	void (*resource_allocation_enable_set) (struct spu *spu, u64 enable);
	u64 (*resource_allocation_enable_get) (struct spu *spu);
};

extern const struct spu_priv1_ops* spu_priv1_ops;

static inline void
spu_int_mask_and (struct spu *spu, int class, u64 mask)
{
	spu_priv1_ops->int_mask_and(spu, class, mask);
}

static inline void
spu_int_mask_or (struct spu *spu, int class, u64 mask)
{
	spu_priv1_ops->int_mask_or(spu, class, mask);
}

static inline void
spu_int_mask_set (struct spu *spu, int class, u64 mask)
{
	spu_priv1_ops->int_mask_set(spu, class, mask);
}

static inline u64
spu_int_mask_get (struct spu *spu, int class)
{
	return spu_priv1_ops->int_mask_get(spu, class);
}

static inline void
spu_int_stat_clear (struct spu *spu, int class, u64 stat)
{
	spu_priv1_ops->int_stat_clear(spu, class, stat);
}

static inline u64
spu_int_stat_get (struct spu *spu, int class)
{
	return spu_priv1_ops->int_stat_get (spu, class);
}

static inline void
spu_cpu_affinity_set (struct spu *spu, int cpu)
{
	spu_priv1_ops->cpu_affinity_set(spu, cpu);
}

static inline u64
spu_mfc_dar_get (struct spu *spu)
{
	return spu_priv1_ops->mfc_dar_get(spu);
}

static inline u64
spu_mfc_dsisr_get (struct spu *spu)
{
	return spu_priv1_ops->mfc_dsisr_get(spu);
}

static inline void
spu_mfc_dsisr_set (struct spu *spu, u64 dsisr)
{
	spu_priv1_ops->mfc_dsisr_set(spu, dsisr);
}

static inline void
spu_mfc_sdr_setup (struct spu *spu)
{
	spu_priv1_ops->mfc_sdr_setup(spu);
}

static inline void
spu_mfc_sr1_set (struct spu *spu, u64 sr1)
{
	spu_priv1_ops->mfc_sr1_set(spu, sr1);
}

static inline u64
spu_mfc_sr1_get (struct spu *spu)
{
	return spu_priv1_ops->mfc_sr1_get(spu);
}

static inline void
spu_mfc_tclass_id_set (struct spu *spu, u64 tclass_id)
{
	spu_priv1_ops->mfc_tclass_id_set(spu, tclass_id);
}

static inline u64
spu_mfc_tclass_id_get (struct spu *spu)
{
	return spu_priv1_ops->mfc_tclass_id_get(spu);
}

static inline void
spu_tlb_invalidate (struct spu *spu)
{
	spu_priv1_ops->tlb_invalidate(spu);
}

static inline void
spu_resource_allocation_groupID_set (struct spu *spu, u64 id)
{
	spu_priv1_ops->resource_allocation_groupID_set(spu, id);
}

static inline u64
spu_resource_allocation_groupID_get (struct spu *spu)
{
	return spu_priv1_ops->resource_allocation_groupID_get(spu);
}

static inline void
spu_resource_allocation_enable_set (struct spu *spu, u64 enable)
{
	spu_priv1_ops->resource_allocation_enable_set(spu, enable);
}

static inline u64
spu_resource_allocation_enable_get (struct spu *spu)
{
	return spu_priv1_ops->resource_allocation_enable_get(spu);
}

/* spu management abstraction */

struct spu_management_ops {
	int (*enumerate_spus)(int (*fn)(void *data));
	int (*create_spu)(struct spu *spu, void *data);
	int (*destroy_spu)(struct spu *spu);
	void (*enable_spu)(struct spu_context *ctx);
	void (*disable_spu)(struct spu_context *ctx);
	int (*init_affinity)(void);
};

extern const struct spu_management_ops* spu_management_ops;

static inline int
spu_enumerate_spus (int (*fn)(void *data))
{
	return spu_management_ops->enumerate_spus(fn);
}

static inline int
spu_create_spu (struct spu *spu, void *data)
{
	return spu_management_ops->create_spu(spu, data);
}

static inline int
spu_destroy_spu (struct spu *spu)
{
	return spu_management_ops->destroy_spu(spu);
}

static inline int
spu_init_affinity (void)
{
	return spu_management_ops->init_affinity();
}

static inline void
spu_enable_spu (struct spu_context *ctx)
{
	spu_management_ops->enable_spu(ctx);
}

static inline void
spu_disable_spu (struct spu_context *ctx)
{
	spu_management_ops->disable_spu(ctx);
}

/*
 * The declarations folowing are put here for convenience
 * and only intended to be used by the platform setup code.
 */

extern const struct spu_priv1_ops spu_priv1_mmio_ops;
extern const struct spu_priv1_ops spu_priv1_beat_ops;

extern const struct spu_management_ops spu_management_of_ops;

#endif /* __KERNEL__ */
#endif
