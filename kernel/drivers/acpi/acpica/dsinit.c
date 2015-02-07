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
 *
 * Module Name: dsinit - Object initialization namespace walk
 *
 *****************************************************************************/

/*
 * Copyright (C) 2000 - 2008, Intel Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 */

#include <acpi/acpi.h>
#include "accommon.h"
#include "acdispat.h"
#include "acnamesp.h"
#include "actables.h"

#define _COMPONENT          ACPI_DISPATCHER
ACPI_MODULE_NAME("dsinit")

/* Local prototypes */
static acpi_status
acpi_ds_init_one_object(acpi_handle obj_handle,
			u32 level, void *context, void **return_value);

/*******************************************************************************
 *
 * FUNCTION:    acpi_ds_init_one_object
 *
 * PARAMETERS:  obj_handle      - Node for the object
 *              Level           - Current nesting level
 *              Context         - Points to a init info struct
 *              return_value    - Not used
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Callback from acpi_walk_namespace. Invoked for every object
 *              within the namespace.
 *
 *              Currently, the only objects that require initialization are:
 *              1) Methods
 *              2) Operation Regions
 *
 ******************************************************************************/

static acpi_status
acpi_ds_init_one_object(acpi_handle obj_handle,
			u32 level, void *context, void **return_value)
{
	struct acpi_init_walk_info *info =
	    (struct acpi_init_walk_info *)context;
	struct acpi_namespace_node *node =
	    (struct acpi_namespace_node *)obj_handle;
	acpi_object_type type;
	acpi_status status;

	ACPI_FUNCTION_ENTRY();

	/*
	 * We are only interested in NS nodes owned by the table that
	 * was just loaded
	 */
	if (node->owner_id != info->owner_id) {
		return (AE_OK);
	}

	info->object_count++;

	/* And even then, we are only interested in a few object types */

	type = acpi_ns_get_type(obj_handle);

	switch (type) {
	case ACPI_TYPE_REGION:

		status = acpi_ds_initialize_region(obj_handle);
		if (ACPI_FAILURE(status)) {
			ACPI_EXCEPTION((AE_INFO, status,
					"During Region initialization %p [%4.4s]",
					obj_handle,
					acpi_ut_get_node_name(obj_handle)));
		}

		info->op_region_count++;
		break;

	case ACPI_TYPE_METHOD:

		info->method_count++;
		break;

	case ACPI_TYPE_DEVICE:

		info->device_count++;
		break;

	default:
		break;
	}

	/*
	 * We ignore errors from above, and always return OK, since
	 * we don't want to abort the walk on a single error.
	 */
	return (AE_OK);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ds_initialize_objects
 *
 * PARAMETERS:  table_desc      - Descriptor for parent ACPI table
 *              start_node      - Root of subtree to be initialized.
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Walk the namespace starting at "StartNode" and perform any
 *              necessary initialization on the objects found therein
 *
 ******************************************************************************/

acpi_status
acpi_ds_initialize_objects(u32 table_index,
			   struct acpi_namespace_node * start_node)
{
	acpi_status status;
	struct acpi_init_walk_info info;
	struct acpi_table_header *table;
	acpi_owner_id owner_id;

	ACPI_FUNCTION_TRACE(ds_initialize_objects);

	status = acpi_tb_get_owner_id(table_index, &owner_id);
	if (ACPI_FAILURE(status)) {
		return_ACPI_STATUS(status);
	}

	ACPI_DEBUG_PRINT((ACPI_DB_DISPATCH,
			  "**** Starting initialization of namespace objects ****\n"));
	ACPI_DEBUG_PRINT_RAW((ACPI_DB_INIT, "Parsing all Control Methods:"));

	info.method_count = 0;
	info.op_region_count = 0;
	info.object_count = 0;
	info.device_count = 0;
	info.table_index = table_index;
	info.owner_id = owner_id;

	/* Walk entire namespace from the supplied root */

	status = acpi_walk_namespace(ACPI_TYPE_ANY, start_node, ACPI_UINT32_MAX,
				     acpi_ds_init_one_object, &info, NULL);
	if (ACPI_FAILURE(status)) {
		ACPI_EXCEPTION((AE_INFO, status, "During WalkNamespace"));
	}

	status = acpi_get_table_by_index(table_index, &table);
	if (ACPI_FAILURE(status)) {
		return_ACPI_STATUS(status);
	}

	ACPI_DEBUG_PRINT_RAW((ACPI_DB_INIT,
			      "\nTable [%4.4s](id %4.4X) - %hd Objects with %hd Devices %hd Methods %hd Regions\n",
			      table->signature, owner_id, info.object_count,
			      info.device_count, info.method_count,
			      info.op_region_count));

	ACPI_DEBUG_PRINT((ACPI_DB_DISPATCH,
			  "%hd Methods, %hd Regions\n", info.method_count,
			  info.op_region_count));

	return_ACPI_STATUS(AE_OK);
}
