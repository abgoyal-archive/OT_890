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
 * Copyright (C) 2007-2008 Advanced Micro Devices, Inc.
 * Author: Joerg Roedel <joerg.roedel@amd.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef __LINUX_IOMMU_H
#define __LINUX_IOMMU_H

#define IOMMU_READ	(1)
#define IOMMU_WRITE	(2)

struct device;

struct iommu_domain {
	void *priv;
};

struct iommu_ops {
	int (*domain_init)(struct iommu_domain *domain);
	void (*domain_destroy)(struct iommu_domain *domain);
	int (*attach_dev)(struct iommu_domain *domain, struct device *dev);
	void (*detach_dev)(struct iommu_domain *domain, struct device *dev);
	int (*map)(struct iommu_domain *domain, unsigned long iova,
		   phys_addr_t paddr, size_t size, int prot);
	void (*unmap)(struct iommu_domain *domain, unsigned long iova,
		      size_t size);
	phys_addr_t (*iova_to_phys)(struct iommu_domain *domain,
				    unsigned long iova);
};

#ifdef CONFIG_IOMMU_API

extern void register_iommu(struct iommu_ops *ops);
extern bool iommu_found(void);
extern struct iommu_domain *iommu_domain_alloc(void);
extern void iommu_domain_free(struct iommu_domain *domain);
extern int iommu_attach_device(struct iommu_domain *domain,
			       struct device *dev);
extern void iommu_detach_device(struct iommu_domain *domain,
				struct device *dev);
extern int iommu_map_range(struct iommu_domain *domain, unsigned long iova,
			   phys_addr_t paddr, size_t size, int prot);
extern void iommu_unmap_range(struct iommu_domain *domain, unsigned long iova,
			      size_t size);
extern phys_addr_t iommu_iova_to_phys(struct iommu_domain *domain,
				      unsigned long iova);

#else /* CONFIG_IOMMU_API */

static inline void register_iommu(struct iommu_ops *ops)
{
}

static inline bool iommu_found(void)
{
	return false;
}

static inline struct iommu_domain *iommu_domain_alloc(void)
{
	return NULL;
}

static inline void iommu_domain_free(struct iommu_domain *domain)
{
}

static inline int iommu_attach_device(struct iommu_domain *domain,
				      struct device *dev)
{
	return -ENODEV;
}

static inline void iommu_detach_device(struct iommu_domain *domain,
				       struct device *dev)
{
}

static inline int iommu_map_range(struct iommu_domain *domain,
				  unsigned long iova, phys_addr_t paddr,
				  size_t size, int prot)
{
	return -ENODEV;
}

static inline void iommu_unmap_range(struct iommu_domain *domain,
				     unsigned long iova, size_t size)
{
}

static inline phys_addr_t iommu_iova_to_phys(struct iommu_domain *domain,
					     unsigned long iova)
{
	return 0;
}

#endif /* CONFIG_IOMMU_API */

#endif /* __LINUX_IOMMU_H */
