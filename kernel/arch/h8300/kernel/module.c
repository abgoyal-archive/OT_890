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

#include <linux/moduleloader.h>
#include <linux/elf.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/kernel.h>

#if 0
#define DEBUGP printk
#else
#define DEBUGP(fmt...)
#endif

void *module_alloc(unsigned long size)
{
	if (size == 0)
		return NULL;
	return vmalloc(size);
}


/* Free memory returned from module_alloc */
void module_free(struct module *mod, void *module_region)
{
	vfree(module_region);
	/* FIXME: If module_region == mod->init_region, trim exception
           table entries. */
}

/* We don't need anything special. */
int module_frob_arch_sections(Elf_Ehdr *hdr,
			      Elf_Shdr *sechdrs,
			      char *secstrings,
			      struct module *mod)
{
	return 0;
}

int apply_relocate(Elf32_Shdr *sechdrs,
		   const char *strtab,
		   unsigned int symindex,
		   unsigned int relsec,
		   struct module *me)
{
	printk(KERN_ERR "module %s: RELOCATION unsupported\n",
	       me->name);
	return -ENOEXEC;
}

int apply_relocate_add(Elf32_Shdr *sechdrs,
		       const char *strtab,
		       unsigned int symindex,
		       unsigned int relsec,
		       struct module *me)
{
	unsigned int i;
	Elf32_Rela *rela = (void *)sechdrs[relsec].sh_addr;

	DEBUGP("Applying relocate section %u to %u\n", relsec,
	       sechdrs[relsec].sh_info);
	for (i = 0; i < sechdrs[relsec].sh_size / sizeof(*rela); i++) {
		/* This is where to make the change */
		uint32_t *loc = (uint32_t *)(sechdrs[sechdrs[relsec].sh_info].sh_addr
					     + rela[i].r_offset);
		/* This is the symbol it is referring to.  Note that all
		   undefined symbols have been resolved.  */
		Elf32_Sym *sym = (Elf32_Sym *)sechdrs[symindex].sh_addr
			+ ELF32_R_SYM(rela[i].r_info);
		uint32_t v = sym->st_value + rela[i].r_addend;

		switch (ELF32_R_TYPE(rela[i].r_info)) {
		case R_H8_DIR24R8:
			loc = (uint32_t *)((uint32_t)loc - 1);
			*loc = (*loc & 0xff000000) | ((*loc & 0xffffff) + v);
			break;
		case R_H8_DIR24A8:
			if (ELF32_R_SYM(rela[i].r_info))
				*loc += v;
			break;
		case R_H8_DIR32:
		case R_H8_DIR32A16:
			*loc += v;
			break;
		case R_H8_PCREL16:
			v -= (unsigned long)loc + 2;
			if ((Elf32_Sword)v > 0x7fff || 
			    (Elf32_Sword)v < -(Elf32_Sword)0x8000)
				goto overflow;
			else 
				*(unsigned short *)loc = v;
			break;
		case R_H8_PCREL8:
			v -= (unsigned long)loc + 1;
			if ((Elf32_Sword)v > 0x7f || 
			    (Elf32_Sword)v < -(Elf32_Sword)0x80)
				goto overflow;
			else 
				*(unsigned char *)loc = v;
			break;
		default:
			printk(KERN_ERR "module %s: Unknown relocation: %u\n",
			       me->name, ELF32_R_TYPE(rela[i].r_info));
			return -ENOEXEC;
		}
	}
	return 0;
 overflow:
	printk(KERN_ERR "module %s: relocation offset overflow: %08x\n",
	       me->name, rela[i].r_offset);
	return -ENOEXEC;
}

int module_finalize(const Elf_Ehdr *hdr,
		    const Elf_Shdr *sechdrs,
		    struct module *me)
{
	return module_bug_finalize(hdr, sechdrs, me);
}

void module_arch_cleanup(struct module *mod)
{
	module_bug_cleanup(mod);
}
