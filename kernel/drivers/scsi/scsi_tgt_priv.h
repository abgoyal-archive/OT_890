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

struct scsi_cmnd;
struct scsi_lun;
struct Scsi_Host;
struct task_struct;

/* tmp - will replace with SCSI logging stuff */
#define eprintk(fmt, args...)					\
do {								\
	printk("%s(%d) " fmt, __func__, __LINE__, ##args);	\
} while (0)

#define dprintk(fmt, args...)
/* #define dprintk eprintk */

extern void scsi_tgt_if_exit(void);
extern int scsi_tgt_if_init(void);

extern int scsi_tgt_uspace_send_cmd(struct scsi_cmnd *cmd, u64 it_nexus_id,
				    struct scsi_lun *lun, u64 tag);
extern int scsi_tgt_uspace_send_status(struct scsi_cmnd *cmd, u64 it_nexus_id,
				       u64 tag);
extern int scsi_tgt_kspace_exec(int host_no, u64 it_nexus_id, int result, u64 tag,
				unsigned long uaddr, u32 len,
				unsigned long sense_uaddr, u32 sense_len, u8 rw);
extern int scsi_tgt_uspace_send_tsk_mgmt(int host_no, u64 it_nexus_id,
					 int function, u64 tag,
					 struct scsi_lun *scsilun, void *data);
extern int scsi_tgt_kspace_tsk_mgmt(int host_no, u64 it_nexus_id,
				    u64 mid, int result);
extern int scsi_tgt_uspace_send_it_nexus_request(int host_no, u64 it_nexus_id,
						 int function, char *initiator);
extern int scsi_tgt_kspace_it_nexus_rsp(int host_no, u64 it_nexus_id, int result);
