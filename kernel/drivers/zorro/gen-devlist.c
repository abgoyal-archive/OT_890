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
 *	Generate devlist.h from the Zorro ID file.
 *
 *	(c) 2000 Geert Uytterhoeven <geert@linux-m68k.org>
 *
 *	Based on the PCI version:
 *
 *	(c) 1999--2000 Martin Mares <mj@ucw.cz>
 */

#include <stdio.h>
#include <string.h>

#define MAX_NAME_SIZE 63

static void
pq(FILE *f, const char *c)
{
	while (*c) {
		if (*c == '"')
			fprintf(f, "\\\"");
		else
			fputc(*c, f);
		c++;
	}
}

int
main(void)
{
	char line[1024], *c, *bra, manuf[8];
	int manufs = 0;
	int mode = 0;
	int lino = 0;
	int manuf_len = 0;
	FILE *devf;

	devf = fopen("devlist.h", "w");
	if (!devf) {
		fprintf(stderr, "Cannot create output file!\n");
		return 1;
	}

	while (fgets(line, sizeof(line)-1, stdin)) {
		lino++;
		if ((c = strchr(line, '\n')))
			*c = 0;
		if (!line[0] || line[0] == '#')
			continue;
		if (line[0] == '\t') {
			switch (mode) {
			case 1:
				if (strlen(line) > 5 && line[5] == ' ') {
					c = line + 5;
					while (*c == ' ')
						*c++ = 0;
					if (manuf_len + strlen(c) + 1 > MAX_NAME_SIZE) {
						/* Too long, try cutting off long description */
						bra = strchr(c, '[');
						if (bra && bra > c && bra[-1] == ' ')
							bra[-1] = 0;
						if (manuf_len + strlen(c) + 1 > MAX_NAME_SIZE) {
							fprintf(stderr, "Line %d: Product name too long\n", lino);
							return 1;
						}
					}
					fprintf(devf, "\tPRODUCT(%s,%s,\"", manuf, line+1);
					pq(devf, c);
					fputs("\")\n", devf);
				} else goto err;
				break;
			default:
				goto err;
			}
		} else if (strlen(line) > 4 && line[4] == ' ') {
			c = line + 4;
			while (*c == ' ')
				*c++ = 0;
			if (manufs)
				fputs("ENDMANUF()\n\n", devf);
			manufs++;
			strcpy(manuf, line);
			manuf_len = strlen(c);
			if (manuf_len + 24 > MAX_NAME_SIZE) {
				fprintf(stderr, "Line %d: manufacturer name too long\n", lino);
				return 1;
			}
			fprintf(devf, "MANUF(%s,\"", manuf);
			pq(devf, c);
			fputs("\")\n", devf);
			mode = 1;
		} else {
		err:
			fprintf(stderr, "Line %d: Syntax error in mode %d: %s\n", lino, mode, line);
			return 1;
		}
	}
	fputs("ENDMANUF()\n\
\n\
#undef MANUF\n\
#undef PRODUCT\n\
#undef ENDMANUF\n", devf);

	fclose(devf);

	return 0;
}
