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
 * linux/include/asm-generic/topology.h
 *
 * Written by: Matthew Dobson, IBM Corporation
 *
 * Copyright (C) 2002, IBM Corp.
 *
 * All rights reserved.          
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Send feedback to <colpatch@us.ibm.com>
 */
#ifndef _ASM_GENERIC_TOPOLOGY_H
#define _ASM_GENERIC_TOPOLOGY_H

#ifndef	CONFIG_NUMA

/* Other architectures wishing to use this simple topology API should fill
   in the below functions as appropriate in their own <asm/topology.h> file. */
#ifndef cpu_to_node
#define cpu_to_node(cpu)	((void)(cpu),0)
#endif
#ifndef parent_node
#define parent_node(node)	((void)(node),0)
#endif
#ifndef node_to_cpumask
#define node_to_cpumask(node)	((void)node, cpu_online_map)
#endif
#ifndef cpumask_of_node
#define cpumask_of_node(node)	((void)node, cpu_online_mask)
#endif
#ifndef node_to_first_cpu
#define node_to_first_cpu(node)	((void)(node),0)
#endif
#ifndef pcibus_to_node
#define pcibus_to_node(bus)	((void)(bus), -1)
#endif

#ifndef pcibus_to_cpumask
#define pcibus_to_cpumask(bus)	(pcibus_to_node(bus) == -1 ? \
					CPU_MASK_ALL : \
					node_to_cpumask(pcibus_to_node(bus)) \
				)
#endif

#ifndef cpumask_of_pcibus
#define cpumask_of_pcibus(bus)	(pcibus_to_node(bus) == -1 ?		\
				 cpu_all_mask :				\
				 cpumask_of_node(pcibus_to_node(bus)))
#endif

#endif	/* CONFIG_NUMA */

/*
 * returns pointer to cpumask for specified node
 * Deprecated: use "const struct cpumask *mask = cpumask_of_node(node)"
 */
#ifndef node_to_cpumask_ptr

#define	node_to_cpumask_ptr(v, node) 					\
		cpumask_t _##v = node_to_cpumask(node);			\
		const cpumask_t *v = &_##v

#define node_to_cpumask_ptr_next(v, node)				\
			  _##v = node_to_cpumask(node)
#endif

#endif /* _ASM_GENERIC_TOPOLOGY_H */
