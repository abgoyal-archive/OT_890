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

#ifndef _SPARC64_MDESC_H
#define _SPARC64_MDESC_H

#include <linux/types.h>
#include <linux/cpumask.h>
#include <asm/prom.h>

struct mdesc_handle;

/* Machine description operations are to be surrounded by grab and
 * release calls.  The mdesc_handle returned from the grab is
 * the first argument to all of the operational calls that work
 * on mdescs.
 */
extern struct mdesc_handle *mdesc_grab(void);
extern void mdesc_release(struct mdesc_handle *);

#define MDESC_NODE_NULL		(~(u64)0)

extern u64 mdesc_node_by_name(struct mdesc_handle *handle,
			      u64 from_node, const char *name);
#define mdesc_for_each_node_by_name(__hdl, __node, __name) \
	for (__node = mdesc_node_by_name(__hdl, MDESC_NODE_NULL, __name); \
	     (__node) != MDESC_NODE_NULL; \
	     __node = mdesc_node_by_name(__hdl, __node, __name))

/* Access to property values returned from mdesc_get_property() are
 * only valid inside of a mdesc_grab()/mdesc_release() sequence.
 * Once mdesc_release() is called, the memory backed up by these
 * pointers may reference freed up memory.
 *
 * Therefore callers must make copies of any property values
 * they need.
 *
 * These same rules apply to mdesc_node_name().
 */
extern const void *mdesc_get_property(struct mdesc_handle *handle,
				      u64 node, const char *name, int *lenp);
extern const char *mdesc_node_name(struct mdesc_handle *hp, u64 node);

/* MD arc iteration, the standard sequence is:
 *
 *	unsigned long arc;
 *	mdesc_for_each_arc(arc, handle, node, MDESC_ARC_TYPE_{FWD,BACK}) {
 *		unsigned long target = mdesc_arc_target(handle, arc);
 *		...
 *	}
 */

#define MDESC_ARC_TYPE_FWD	"fwd"
#define MDESC_ARC_TYPE_BACK	"back"

extern u64 mdesc_next_arc(struct mdesc_handle *handle, u64 from,
			  const char *arc_type);
#define mdesc_for_each_arc(__arc, __hdl, __node, __type) \
	for (__arc = mdesc_next_arc(__hdl, __node, __type); \
	     (__arc) != MDESC_NODE_NULL; \
	     __arc = mdesc_next_arc(__hdl, __arc, __type))

extern u64 mdesc_arc_target(struct mdesc_handle *hp, u64 arc);

extern void mdesc_update(void);

struct mdesc_notifier_client {
	void (*add)(struct mdesc_handle *handle, u64 node);
	void (*remove)(struct mdesc_handle *handle, u64 node);

	const char			*node_name;
	struct mdesc_notifier_client	*next;
};

extern void mdesc_register_notifier(struct mdesc_notifier_client *client);

extern void mdesc_fill_in_cpu_data(cpumask_t mask);

extern void sun4v_mdesc_init(void);

#endif
