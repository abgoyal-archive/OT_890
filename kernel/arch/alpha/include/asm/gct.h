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

#ifndef __ALPHA_GCT_H
#define __ALPHA_GCT_H

typedef u64 gct_id;
typedef u64 gct6_handle;

typedef struct __gct6_node {
	u8 type;	
	u8 subtype;
	u16 size;
	u32 hd_extension;
	gct6_handle owner;
	gct6_handle active_user;
	gct_id id;
	u64 flags;
	u16 rev;
	u16 change_counter;
	u16 max_child;
	u16 reserved1;
	gct6_handle saved_owner;
	gct6_handle affinity;
	gct6_handle parent;
	gct6_handle next;
	gct6_handle prev;
	gct6_handle child;
	u64 fw_flags;
	u64 os_usage;
	u64 fru_id;
	u32 checksum;
	u32 magic;	/* 'GLXY' */
} gct6_node;

typedef struct {
	u8 type;	
	u8 subtype;
	void (*callout)(gct6_node *);
} gct6_search_struct;

#define GCT_NODE_MAGIC	  0x59584c47	/* 'GLXY' */

/* 
 * node types 
 */
#define GCT_TYPE_HOSE			0x0E

/*
 * node subtypes
 */
#define GCT_SUBTYPE_IO_PORT_MODULE	0x2C

#define GCT_NODE_PTR(off) ((gct6_node *)((char *)hwrpb + 		\
					 hwrpb->frut_offset + 		\
					 (gct6_handle)(off)))		\

int gct6_find_nodes(gct6_node *, gct6_search_struct *);

#endif /* __ALPHA_GCT_H */

