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

#ifndef B43legacy_DEBUGFS_H_
#define B43legacy_DEBUGFS_H_

struct b43legacy_wldev;
struct b43legacy_txstatus;

enum b43legacy_dyndbg { /* Dynamic debugging features */
	B43legacy_DBG_XMITPOWER,
	B43legacy_DBG_DMAOVERFLOW,
	B43legacy_DBG_DMAVERBOSE,
	B43legacy_DBG_PWORK_FAST,
	B43legacy_DBG_PWORK_STOP,
	__B43legacy_NR_DYNDBG,
};


#ifdef CONFIG_B43LEGACY_DEBUG

struct dentry;

#define B43legacy_NR_LOGGED_TXSTATUS	100

struct b43legacy_txstatus_log {
	struct b43legacy_txstatus *log;
	int end;
	spinlock_t lock;	/* lock for debugging */
};

struct b43legacy_dfs_file {
	struct dentry *dentry;
	char *buffer;
	size_t data_len;
};

struct b43legacy_dfsentry {
	struct b43legacy_wldev *dev;
	struct dentry *subdir;

	struct b43legacy_dfs_file file_tsf;
	struct b43legacy_dfs_file file_ucode_regs;
	struct b43legacy_dfs_file file_shm;
	struct b43legacy_dfs_file file_txstat;
	struct b43legacy_dfs_file file_txpower_g;
	struct b43legacy_dfs_file file_restart;
	struct b43legacy_dfs_file file_loctls;

	struct b43legacy_txstatus_log txstatlog;

	/* Enabled/Disabled list for the dynamic debugging features. */
	u32 dyn_debug[__B43legacy_NR_DYNDBG];
	/* Dentries for the dynamic debugging entries. */
	struct dentry *dyn_debug_dentries[__B43legacy_NR_DYNDBG];
};

int b43legacy_debug(struct b43legacy_wldev *dev,
		    enum b43legacy_dyndbg feature);

void b43legacy_debugfs_init(void);
void b43legacy_debugfs_exit(void);
void b43legacy_debugfs_add_device(struct b43legacy_wldev *dev);
void b43legacy_debugfs_remove_device(struct b43legacy_wldev *dev);
void b43legacy_debugfs_log_txstat(struct b43legacy_wldev *dev,
				  const struct b43legacy_txstatus *status);

#else /* CONFIG_B43LEGACY_DEBUG*/

static inline
int b43legacy_debug(struct b43legacy_wldev *dev,
		    enum b43legacy_dyndbg feature)
{
	return 0;
}

static inline
void b43legacy_debugfs_init(void) { }
static inline
void b43legacy_debugfs_exit(void) { }
static inline
void b43legacy_debugfs_add_device(struct b43legacy_wldev *dev) { }
static inline
void b43legacy_debugfs_remove_device(struct b43legacy_wldev *dev) { }
static inline
void b43legacy_debugfs_log_txstat(struct b43legacy_wldev *dev,
				  const struct b43legacy_txstatus *status)
				  { }

#endif /* CONFIG_B43LEGACY_DEBUG*/

#endif /* B43legacy_DEBUGFS_H_ */
