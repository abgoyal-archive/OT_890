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

#ifndef __MYRI10GE_MCP_GEN_HEADER_H__
#define __MYRI10GE_MCP_GEN_HEADER_H__


#define MCP_HEADER_PTR_OFFSET  0x3c

#define MCP_TYPE_MX 0x4d582020	/* "MX  " */
#define MCP_TYPE_PCIE 0x70636965	/* "PCIE" pcie-only MCP */
#define MCP_TYPE_ETH 0x45544820	/* "ETH " */
#define MCP_TYPE_MCP0 0x4d435030	/* "MCP0" */
#define MCP_TYPE_DFLT 0x20202020	/* "    " */

struct mcp_gen_header {
	/* the first 4 fields are filled at compile time */
	unsigned header_length;
	__be32 mcp_type;
	char version[128];
	unsigned mcp_private;	/* pointer to mcp-type specific structure */

	/* filled by the MCP at run-time */
	unsigned sram_size;
	unsigned string_specs;	/* either the original STRING_SPECS or a superset */
	unsigned string_specs_len;

	/* Fields above this comment are guaranteed to be present.
	 *
	 * Fields below this comment are extensions added in later versions
	 * of this struct, drivers should compare the header_length against
	 * offsetof(field) to check wether a given MCP implements them.
	 *
	 * Never remove any field.  Keep everything naturally align.
	 */

	/* Specifies if the running mcp is mcp0, 1, or 2. */
	unsigned char mcp_index;
	unsigned char disable_rabbit;
	unsigned char unaligned_tlp;
	unsigned char pcie_link_algo;
	unsigned counters_addr;
	unsigned copy_block_info;	/* for small mcps loaded with "lload -d" */
	unsigned short handoff_id_major;	/* must be equal */
	unsigned short handoff_id_caps;	/* bitfield: new mcp must have superset */
	unsigned msix_table_addr;	/* start address of msix table in firmware */
	unsigned bss_addr;	/* start of bss */
	unsigned features;
	/* 8 */
};

#endif				/* __MYRI10GE_MCP_GEN_HEADER_H__ */
