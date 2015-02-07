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
 * QLogic iSCSI HBA Driver
 * Copyright (c)  2003-2006 QLogic Corporation
 *
 * See LICENSE.qla4xxx for copyright and licensing details.
 */

#ifndef __QLA4x_GBL_H
#define	__QLA4x_GBL_H

struct iscsi_cls_conn;

void qla4xxx_hw_reset(struct scsi_qla_host *ha);
int ql4xxx_lock_drvr_wait(struct scsi_qla_host *a);
int qla4xxx_send_tgts(struct scsi_qla_host *ha, char *ip, uint16_t port);
int qla4xxx_send_command_to_isp(struct scsi_qla_host *ha, struct srb * srb);
int qla4xxx_initialize_adapter(struct scsi_qla_host * ha,
			       uint8_t renew_ddb_list);
int qla4xxx_soft_reset(struct scsi_qla_host *ha);
irqreturn_t qla4xxx_intr_handler(int irq, void *dev_id);

void qla4xxx_free_ddb_list(struct scsi_qla_host * ha);
void qla4xxx_process_aen(struct scsi_qla_host * ha, uint8_t process_aen);

int qla4xxx_get_dhcp_ip_address(struct scsi_qla_host * ha);
int qla4xxx_relogin_device(struct scsi_qla_host * ha,
			   struct ddb_entry * ddb_entry);
int qla4xxx_reset_lun(struct scsi_qla_host * ha, struct ddb_entry * ddb_entry,
		      int lun);
int qla4xxx_reset_target(struct scsi_qla_host * ha,
			 struct ddb_entry * ddb_entry);
int qla4xxx_get_flash(struct scsi_qla_host * ha, dma_addr_t dma_addr,
		      uint32_t offset, uint32_t len);
int qla4xxx_get_firmware_status(struct scsi_qla_host * ha);
int qla4xxx_get_firmware_state(struct scsi_qla_host * ha);
int qla4xxx_initialize_fw_cb(struct scsi_qla_host * ha);

/* FIXME: Goodness!  this really wants a small struct to hold the
 * parameters. On x86 the args will get passed on the stack! */
int qla4xxx_get_fwddb_entry(struct scsi_qla_host *ha,
			    uint16_t fw_ddb_index,
			    struct dev_db_entry *fw_ddb_entry,
			    dma_addr_t fw_ddb_entry_dma,
			    uint32_t *num_valid_ddb_entries,
			    uint32_t *next_ddb_index,
			    uint32_t *fw_ddb_device_state,
			    uint32_t *conn_err_detail,
			    uint16_t *tcp_source_port_num,
			    uint16_t *connection_id);

int qla4xxx_set_ddb_entry(struct scsi_qla_host * ha, uint16_t fw_ddb_index,
			  dma_addr_t fw_ddb_entry_dma);

void qla4xxx_mark_device_missing(struct scsi_qla_host *ha,
				 struct ddb_entry *ddb_entry);
u16 rd_nvram_word(struct scsi_qla_host * ha, int offset);
void qla4xxx_get_crash_record(struct scsi_qla_host * ha);
struct ddb_entry *qla4xxx_alloc_sess(struct scsi_qla_host *ha);
int qla4xxx_add_sess(struct ddb_entry *);
void qla4xxx_destroy_sess(struct ddb_entry *ddb_entry);
int qla4xxx_is_nvram_configuration_valid(struct scsi_qla_host * ha);
int qla4xxx_get_fw_version(struct scsi_qla_host * ha);
void qla4xxx_interrupt_service_routine(struct scsi_qla_host * ha,
				       uint32_t intr_status);
int qla4xxx_init_rings(struct scsi_qla_host * ha);
struct srb * qla4xxx_del_from_active_array(struct scsi_qla_host *ha,
					uint32_t index);
void qla4xxx_srb_compl(struct scsi_qla_host *ha, struct srb *srb);
int qla4xxx_reinitialize_ddb_list(struct scsi_qla_host * ha);
int qla4xxx_process_ddb_changed(struct scsi_qla_host * ha,
				uint32_t fw_ddb_index, uint32_t state);
void qla4xxx_dump_buffer(void *b, uint32_t size);
int qla4xxx_send_marker_iocb(struct scsi_qla_host *ha,
	struct ddb_entry *ddb_entry, int lun, uint16_t mrkr_mod);

extern int ql4xextended_error_logging;
extern int ql4xdiscoverywait;
extern int ql4xdontresethba;
extern int ql4_mod_unload;
#endif /* _QLA4x_GBL_H */
