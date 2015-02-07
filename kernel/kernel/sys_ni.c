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


#include <linux/linkage.h>
#include <linux/errno.h>

#include <asm/unistd.h>

/*  we can't #include <linux/syscalls.h> here,
    but tell gcc to not warn with -Wmissing-prototypes  */
asmlinkage long sys_ni_syscall(void);

/*
 * Non-implemented system calls get redirected here.
 */
asmlinkage long sys_ni_syscall(void)
{
	return -ENOSYS;
}

cond_syscall(sys_nfsservctl);
cond_syscall(sys_quotactl);
cond_syscall(sys32_quotactl);
cond_syscall(sys_acct);
cond_syscall(sys_lookup_dcookie);
cond_syscall(sys_swapon);
cond_syscall(sys_swapoff);
cond_syscall(sys_kexec_load);
cond_syscall(compat_sys_kexec_load);
cond_syscall(sys_init_module);
cond_syscall(sys_delete_module);
cond_syscall(sys_socketpair);
cond_syscall(sys_bind);
cond_syscall(sys_listen);
cond_syscall(sys_accept);
cond_syscall(sys_accept4);
cond_syscall(sys_connect);
cond_syscall(sys_getsockname);
cond_syscall(sys_getpeername);
cond_syscall(sys_sendto);
cond_syscall(sys_send);
cond_syscall(sys_recvfrom);
cond_syscall(sys_recv);
cond_syscall(sys_socket);
cond_syscall(sys_setsockopt);
cond_syscall(compat_sys_setsockopt);
cond_syscall(sys_getsockopt);
cond_syscall(compat_sys_getsockopt);
cond_syscall(sys_shutdown);
cond_syscall(sys_sendmsg);
cond_syscall(compat_sys_sendmsg);
cond_syscall(sys_recvmsg);
cond_syscall(compat_sys_recvmsg);
cond_syscall(sys_socketcall);
cond_syscall(sys_futex);
cond_syscall(compat_sys_futex);
cond_syscall(sys_set_robust_list);
cond_syscall(compat_sys_set_robust_list);
cond_syscall(sys_get_robust_list);
cond_syscall(compat_sys_get_robust_list);
cond_syscall(sys_epoll_create);
cond_syscall(sys_epoll_create1);
cond_syscall(sys_epoll_ctl);
cond_syscall(sys_epoll_wait);
cond_syscall(sys_epoll_pwait);
cond_syscall(compat_sys_epoll_pwait);
cond_syscall(sys_semget);
cond_syscall(sys_semop);
cond_syscall(sys_semtimedop);
cond_syscall(sys_semctl);
cond_syscall(sys_msgget);
cond_syscall(sys_msgsnd);
cond_syscall(sys_msgrcv);
cond_syscall(sys_msgctl);
cond_syscall(sys_shmget);
cond_syscall(sys_shmat);
cond_syscall(sys_shmdt);
cond_syscall(sys_shmctl);
cond_syscall(sys_mq_open);
cond_syscall(sys_mq_unlink);
cond_syscall(sys_mq_timedsend);
cond_syscall(sys_mq_timedreceive);
cond_syscall(sys_mq_notify);
cond_syscall(sys_mq_getsetattr);
cond_syscall(compat_sys_mq_open);
cond_syscall(compat_sys_mq_timedsend);
cond_syscall(compat_sys_mq_timedreceive);
cond_syscall(compat_sys_mq_notify);
cond_syscall(compat_sys_mq_getsetattr);
cond_syscall(sys_mbind);
cond_syscall(sys_get_mempolicy);
cond_syscall(sys_set_mempolicy);
cond_syscall(compat_sys_mbind);
cond_syscall(compat_sys_get_mempolicy);
cond_syscall(compat_sys_set_mempolicy);
cond_syscall(sys_add_key);
cond_syscall(sys_request_key);
cond_syscall(sys_keyctl);
cond_syscall(compat_sys_keyctl);
cond_syscall(compat_sys_socketcall);
cond_syscall(sys_inotify_init);
cond_syscall(sys_inotify_init1);
cond_syscall(sys_inotify_add_watch);
cond_syscall(sys_inotify_rm_watch);
cond_syscall(sys_migrate_pages);
cond_syscall(sys_move_pages);
cond_syscall(sys_chown16);
cond_syscall(sys_fchown16);
cond_syscall(sys_getegid16);
cond_syscall(sys_geteuid16);
cond_syscall(sys_getgid16);
cond_syscall(sys_getgroups16);
cond_syscall(sys_getresgid16);
cond_syscall(sys_getresuid16);
cond_syscall(sys_getuid16);
cond_syscall(sys_lchown16);
cond_syscall(sys_setfsgid16);
cond_syscall(sys_setfsuid16);
cond_syscall(sys_setgid16);
cond_syscall(sys_setgroups16);
cond_syscall(sys_setregid16);
cond_syscall(sys_setresgid16);
cond_syscall(sys_setresuid16);
cond_syscall(sys_setreuid16);
cond_syscall(sys_setuid16);
cond_syscall(sys_vm86old);
cond_syscall(sys_vm86);
cond_syscall(compat_sys_ipc);
cond_syscall(compat_sys_sysctl);
cond_syscall(sys_flock);
cond_syscall(sys_io_setup);
cond_syscall(sys_io_destroy);
cond_syscall(sys_io_submit);
cond_syscall(sys_io_cancel);
cond_syscall(sys_io_getevents);
cond_syscall(sys_syslog);

/* arch-specific weak syscall entries */
cond_syscall(sys_pciconfig_read);
cond_syscall(sys_pciconfig_write);
cond_syscall(sys_pciconfig_iobase);
cond_syscall(sys32_ipc);
cond_syscall(sys32_sysctl);
cond_syscall(ppc_rtas);
cond_syscall(sys_spu_run);
cond_syscall(sys_spu_create);
cond_syscall(sys_subpage_prot);

/* mmu depending weak syscall entries */
cond_syscall(sys_mprotect);
cond_syscall(sys_msync);
cond_syscall(sys_mlock);
cond_syscall(sys_munlock);
cond_syscall(sys_mlockall);
cond_syscall(sys_munlockall);
cond_syscall(sys_mincore);
cond_syscall(sys_madvise);
cond_syscall(sys_mremap);
cond_syscall(sys_remap_file_pages);
cond_syscall(compat_sys_move_pages);
cond_syscall(compat_sys_migrate_pages);

/* block-layer dependent */
cond_syscall(sys_bdflush);
cond_syscall(sys_ioprio_set);
cond_syscall(sys_ioprio_get);

/* New file descriptors */
cond_syscall(sys_signalfd);
cond_syscall(sys_signalfd4);
cond_syscall(compat_sys_signalfd);
cond_syscall(compat_sys_signalfd4);
cond_syscall(sys_timerfd_create);
cond_syscall(sys_timerfd_settime);
cond_syscall(sys_timerfd_gettime);
cond_syscall(compat_sys_timerfd_settime);
cond_syscall(compat_sys_timerfd_gettime);
cond_syscall(sys_eventfd);
cond_syscall(sys_eventfd2);
