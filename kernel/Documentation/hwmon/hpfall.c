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

/* Disk protection for HP machines.
 *
 * Copyright 2008 Eric Piel
 * Copyright 2009 Pavel Machek <pavel@suse.cz>
 *
 * GPLv2.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <signal.h>

void write_int(char *path, int i)
{
	char buf[1024];
	int fd = open(path, O_RDWR);
	if (fd < 0) {
		perror("open");
		exit(1);
	}
	sprintf(buf, "%d", i);
	if (write(fd, buf, strlen(buf)) != strlen(buf)) {
		perror("write");
		exit(1);
	}
	close(fd);
}

void set_led(int on)
{
	write_int("/sys/class/leds/hp::hddprotect/brightness", on);
}

void protect(int seconds)
{
	write_int("/sys/block/sda/device/unload_heads", seconds*1000);
}

int on_ac(void)
{
//	/sys/class/power_supply/AC0/online
}

int lid_open(void)
{
//	/proc/acpi/button/lid/LID/state
}

void ignore_me(void)
{
	protect(0);
	set_led(0);

}

int main(int argc, char* argv[])
{
       int fd, ret;

       fd = open("/dev/freefall", O_RDONLY);
       if (fd < 0) {
               perror("open");
               return EXIT_FAILURE;
       }

	signal(SIGALRM, ignore_me);

       for (;;) {
	       unsigned char count;

               ret = read(fd, &count, sizeof(count));
	       alarm(0);
	       if ((ret == -1) && (errno == EINTR)) {
		       /* Alarm expired, time to unpark the heads */
		       continue;
	       }

               if (ret != sizeof(count)) {
                       perror("read");
                       break;
               }

	       protect(21);
	       set_led(1);
	       if (1 || on_ac() || lid_open()) {
		       alarm(2);
	       } else {
		       alarm(20);
	       }
       }

       close(fd);
       return EXIT_SUCCESS;
}
