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
 * IBM ASM Service Processor Device Driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) IBM Corporation, 2004
 *
 * Author: Max Asböck <amax@us.ibm.com>
 *
 */

#include "ibmasm.h"
#include "dot_command.h"

/**
 * Dispatch an incoming message to the specific handler for the message.
 * Called from interrupt context.
 */
void ibmasm_receive_message(struct service_processor *sp, void *message, int message_size)
{
	u32 size;
	struct dot_command_header *header = (struct dot_command_header *)message;

	if (message_size == 0)
		return;

	size = get_dot_command_size(message);
	if (size == 0)
		return;

	if (size > message_size)
		size = message_size;

	switch (header->type) {
	case sp_event:
		ibmasm_receive_event(sp, message, size);
		break;
	case sp_command_response:
		ibmasm_receive_command_response(sp, message, size);
		break;
	case sp_heartbeat:
		ibmasm_receive_heartbeat(sp, message, size);
		break;
	default:
		dev_err(sp->dev, "Received unknown message from service processor\n");
	}
}


#define INIT_BUFFER_SIZE 32


/**
 * send the 4.3.5.10 dot command (driver VPD) to the service processor
 */
int ibmasm_send_driver_vpd(struct service_processor *sp)
{
	struct command *command;
	struct dot_command_header *header;
	u8 *vpd_command;
	u8 *vpd_data;
	int result = 0;

	command = ibmasm_new_command(sp, INIT_BUFFER_SIZE);
	if (command == NULL)
		return -ENOMEM;

	header = (struct dot_command_header *)command->buffer;
	header->type                = sp_write;
	header->command_size        = 4;
	header->data_size           = 16;
	header->status              = 0;
	header->reserved            = 0;

	vpd_command = command->buffer + sizeof(struct dot_command_header);
	vpd_command[0] = 0x4;
	vpd_command[1] = 0x3;
	vpd_command[2] = 0x5;
	vpd_command[3] = 0xa;

	vpd_data = vpd_command + header->command_size;
	vpd_data[0] = 0;
	strcat(vpd_data, IBMASM_DRIVER_VPD);
	vpd_data[10] = 0;
	vpd_data[15] = 0;

	ibmasm_exec_command(sp, command);
	ibmasm_wait_for_response(command, IBMASM_CMD_TIMEOUT_NORMAL);

	if (command->status != IBMASM_CMD_COMPLETE)
		result = -ENODEV;

	command_put(command);

	return result;
}

struct os_state_command {
	struct dot_command_header	header;
	unsigned char			command[3];
	unsigned char			data;
};

/**
 * send the 4.3.6 dot command (os state) to the service processor
 * During driver init this function is called with os state "up".
 * This causes the service processor to start sending heartbeats the
 * driver.
 * During driver exit the function is called with os state "down",
 * causing the service processor to stop the heartbeats.
 */
int ibmasm_send_os_state(struct service_processor *sp, int os_state)
{
	struct command *cmd;
	struct os_state_command *os_state_cmd;
	int result = 0;

	cmd = ibmasm_new_command(sp, sizeof(struct os_state_command));
	if (cmd == NULL)
		return -ENOMEM;

	os_state_cmd = (struct os_state_command *)cmd->buffer;
	os_state_cmd->header.type		= sp_write;
	os_state_cmd->header.command_size	= 3;
	os_state_cmd->header.data_size		= 1;
	os_state_cmd->header.status		= 0;
	os_state_cmd->command[0]		= 4;
	os_state_cmd->command[1]		= 3;
	os_state_cmd->command[2]		= 6;
	os_state_cmd->data			= os_state;

	ibmasm_exec_command(sp, cmd);
	ibmasm_wait_for_response(cmd, IBMASM_CMD_TIMEOUT_NORMAL);

	if (cmd->status != IBMASM_CMD_COMPLETE)
		result = -ENODEV;

	command_put(cmd);
	return result;
}
